/***************************************************************************
    qgsattributeform.cpp
     --------------------------------------
    Date                 : 3.5.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias dot kuhn at gmx dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsattributeform.h"

#include "qgsattributeeditor.h"
#include "qgseditorwidgetregistry.h"
#include "qgspythonrunner.h"
#include "qgsrelationeditor.h"

#include <QDir>
#include <QFileInfo>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QUiLoader>

int QgsAttributeForm::sFormCounter = 0;

QgsAttributeForm::QgsAttributeForm( QgsVectorLayer* vl, const QgsFeature feature, QgsAttributeEditorContext context, QWidget* parent )
    : QWidget( parent )
    , mLayer( vl )
    , mContext( context )
    , mFormNr( sFormCounter++ )
    , mIsSaving( false )
{
  init();
  setFeature( feature );
}

QgsAttributeForm::~QgsAttributeForm()
{
  cleanPython();
}

void QgsAttributeForm::hideButtonBox()
{
  mButtonBox->hide();
}

void QgsAttributeForm::showButtonBox()
{
  mButtonBox->show();
}

void QgsAttributeForm::changeAttribute( const QString& field, const QVariant& value )
{
  Q_FOREACH( QgsEditorWidgetWrapper* eww, mWidgets )
  {
    if ( eww->field().name() == field )
    {
      eww->setValue( value );
    }
  }
}


void QgsAttributeForm::setFeature( const QgsFeature& feature )
{
  mFeature = feature;

  resetValues();

  synchronizeEnabledState();

  initPython();
}

bool QgsAttributeForm::save()
{
  if ( mIsSaving )
    return true;

  mIsSaving = true;

  bool success = true;

  emit beforeSave( success );

  // Somebody wants to prevent this form from saving
  if ( !success )
    return false;

  if ( mFeature.isValid() )
  {
    bool doUpdate = false;

    QgsAttributes src = mFeature.attributes();
    QgsAttributes dst = mFeature.attributes();

    Q_FOREACH( QgsEditorWidgetWrapper* eww, mWidgets )
    {
      QVariant dstVar = dst[eww->fieldIdx()];
      QVariant srcVar = eww->value();
      if ( dstVar != srcVar && srcVar.isValid() )
      {
        dst[eww->fieldIdx()] = eww->value();

        doUpdate = true;
      }
    }

    if ( doUpdate )
    {
      mLayer->beginEditCommand( tr( "Attributes changed" ) );

      for ( int i = 0; i < dst.count(); ++i )
      {
        if ( dst[i] == src[i] || !src[i].isValid() )
          continue;

        success &= mLayer->changeAttributeValue( mFeature.id(), i, dst[i], src[i] );
      }

      if ( success )
      {
        mLayer->endEditCommand();
        mFeature.setAttributes( dst );
      }
      else
      {
        mLayer->destroyEditCommand();
      }
    }
  }

  mIsSaving = false;

  return success;
}

void QgsAttributeForm::accept()
{
  save();
}

void QgsAttributeForm::resetValues()
{
  Q_FOREACH( QgsEditorWidgetWrapper* eww, mWidgets )
  {
    if ( mFeature.isValid() )
    {
      eww->setValue( mFeature.attribute( eww->field().name() ) );
    }
    else
    {
      eww->setValue( QVariant( QVariant::String ) );
    }
  }
}

void QgsAttributeForm::onAttributeChanged( const QVariant& value )
{
  QgsEditorWidgetWrapper* eww = qobject_cast<QgsEditorWidgetWrapper*>( sender() );

  Q_ASSERT( eww );

  emit attributeChanged( eww->field().name(), value );
}

void QgsAttributeForm::synchronizeEnabledState()
{
  Q_FOREACH( QgsEditorWidgetWrapper* eww, mWidgets )
  {
    if ( mFeature.isValid() && mLayer->isEditable() )
    {
      eww->setEnabled( true );
    }
    else
    {
      eww->setEnabled( false );
    }
  }

  QPushButton* okButton = mButtonBox->button( QDialogButtonBox::Ok );
  if ( okButton )
    okButton->setEnabled( mFeature.isValid() && mLayer->isEditable() );
}

void QgsAttributeForm::init()
{
  QWidget* formWidget = 0;

  // Todo: Test if calling init twice works...
  delete this->layout();

  qDeleteAll( mWidgets );
  mWidgets.clear();

  // Get a layout
  setLayout( new QGridLayout( this ) );

  // Try to load Ui-File for layout
  if ( mLayer->editorLayout() == QgsVectorLayer::UiFileLayout && !mLayer->editForm().isEmpty() )
  {
    QFile file( mLayer->editForm() );

    if ( file.open( QFile::ReadOnly ) )
    {
      QUiLoader loader;

      QFileInfo fi( mLayer->editForm() );
      loader.setWorkingDirectory( fi.dir() );
      formWidget = loader.load( &file, this );
      layout()->addWidget( formWidget );
      file.close();
      createWrappers();
    }
  }

  // Tab layout
  if ( !formWidget && mLayer->editorLayout() == QgsVectorLayer::TabLayout )
  {
    QTabWidget* tabWidget = new QTabWidget( this );
    layout()->addWidget( tabWidget );

    Q_FOREACH( const QgsAttributeEditorElement *widgDef, mLayer->attributeEditorElements() )
    {
      QWidget* tabPage = new QWidget( tabWidget );

      tabWidget->addTab( tabPage, widgDef->name() );
      QGridLayout *tabPageLayout = new QGridLayout( tabPage );

      if ( widgDef->type() == QgsAttributeEditorElement::AeTypeContainer )
      {
        QString dummy1;
        bool dummy2;
        tabPageLayout->addWidget( createWidgetFromDef( widgDef, tabPage, mLayer, mContext, dummy1, dummy2 ) );
      }
      else
      {
        QgsDebugMsg( "No support for fields in attribute editor on top level" );
      }
    }
    formWidget = tabWidget;
  }

  // Autogenerate Layout
  // If there is still no layout loaded (defined as autogenerate or other methods failed)
  if ( !formWidget )
  {
    formWidget = new QWidget( this );
    QFormLayout* formLayout = new QFormLayout( formWidget );
    formWidget->setLayout( formLayout );
    layout()->addWidget( formWidget );

    Q_FOREACH( const QgsField& field, mLayer->pendingFields().toList() )
    {
      int idx = mLayer->fieldNameIndex( field.name() );
      //show attribute alias if available
      QString fieldName = mLayer->attributeDisplayName( idx );

      const QString widgetType = mLayer->editorWidgetV2( idx );
      const QgsEditorWidgetConfig widgetConfig = mLayer->editorWidgetV2Config( idx );

      // This will also create the widget
      QgsEditorWidgetWrapper* eww = QgsEditorWidgetRegistry::instance()->create( widgetType, mLayer, idx, widgetConfig, 0, this );
      if ( eww )
      {
        mWidgets.append( eww );
        formLayout->addRow( new QLabel( fieldName ), eww->widget() );
      }
      else
      {
        formLayout->addRow( new QLabel( fieldName ), new QLabel( QString( "<p style=\"color: red; font-style: italic;\">Failed to create widget with type '%1'</p>" ).arg( widgetType ) ) );
      }
    }
  }

  mButtonBox = findChild<QDialogButtonBox*>();

  if ( !mButtonBox )
  {
    mButtonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    layout()->addWidget( mButtonBox );
  }

  connectWrappers();

  connect( mButtonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
  connect( mButtonBox, SIGNAL( rejected() ), this, SLOT( resetValues() ) );

  connect( mLayer, SIGNAL( beforeModifiedCheck() ), this, SLOT( save() ) );
  connect( mLayer, SIGNAL( editingStarted() ), this, SLOT( synchronizeEnabledState() ) );
  connect( mLayer, SIGNAL( editingStopped() ), this, SLOT( synchronizeEnabledState() ) );
}

void QgsAttributeForm::cleanPython()
{
  QString expr = QString( "if locals().has_key('_qgis_featureform_%1'): del _qgis_featureform_%1\n" ).arg( mFormNr );
  QgsPythonRunner::run( expr );

  if ( !mPyFormVarName.isEmpty() )
  {
    QString expr = QString( "if locals().has_key('%1'): del %1\n" ).arg( mPyFormVarName );
    QgsPythonRunner::run( expr );
  }
}

void QgsAttributeForm::initPython()
{
  cleanPython();

  // Init Python
  if ( !mLayer->editFormInit().isEmpty() )
  {
    QString module = mLayer->editFormInit();

    int pos = module.lastIndexOf( "." );
    if ( pos >= 0 )
    {
      QgsPythonRunner::run( QString( "import %1" ).arg( module.left( pos ) ) );
    }

    /* Reload the module if the DEBUGMODE switch has been set in the module.
    If set to False you have to reload QGIS to reset it to True due to Python
    module caching */
    QString reload = QString( "if hasattr(%1,'DEBUGMODE') and %1.DEBUGMODE:"
                              " reload(%1)" ).arg( module.left( pos ) );

    QgsPythonRunner::run( reload );

    QString form = QString( "_qgis_featureform_%1 = sip.wrapinstance( %2, qgis.gui.QgsAttributeForm )" )
                   .arg( mFormNr )
                   .arg(( unsigned long ) this );

    QString layer = QString( "_qgis_layer_%1 = sip.wrapinstance( %2, qgis.core.QgsVectorLayer )" )
                    .arg( mLayer->id() )
                    .arg(( unsigned long ) mLayer );

    // Generate the unique ID of this feature. We used to use feature ID but some providers
    // return a ID that is an invalid python variable when we have new unsaved features.
    QDateTime dt = QDateTime::currentDateTime();
    QString featurevarname = QString( "_qgis_feature_%1" ).arg( dt.toString( "yyyyMMddhhmmsszzz" ) );
    QString feature = QString( "%1 = sip.wrapinstance( %2, qgis.core.QgsFeature )" )
                      .arg( featurevarname )
                      .arg(( unsigned long ) & mFeature );

    QgsPythonRunner::run( form );
    QgsPythonRunner::run( feature );
    QgsPythonRunner::run( layer );

    mPyFormVarName = QString( "_qgis_feature_form_%1" ).arg( dt.toString( "yyyyMMddhhmmsszzz" ) );
    QString expr = QString( "%5 = %1(_qgis_featureform_%2, _qgis_layer_%3, %4)" )
                   .arg( mLayer->editFormInit() )
                   .arg( mFormNr )
                   .arg( mLayer->id() )
                   .arg( featurevarname )
                   .arg( mPyFormVarName );

    QgsDebugMsg( QString( "running featureForm init: %1" ).arg( expr ) );
    QgsPythonRunner::run( expr );
  }
}

QWidget* QgsAttributeForm::createWidgetFromDef( const QgsAttributeEditorElement* widgetDef, QWidget* parent, QgsVectorLayer* vl, QgsAttributeEditorContext& context, QString& labelText, bool& labelOnTop )
{
  QWidget *newWidget = 0;

  switch ( widgetDef->type() )
  {
    case QgsAttributeEditorElement::AeTypeField:
    {
      const QgsAttributeEditorField* fieldDef = dynamic_cast<const QgsAttributeEditorField*>( widgetDef );
      int fldIdx = fieldDef->idx();
      if ( fldIdx < vl->pendingFields().count() && fldIdx >= 0 )
      {
        const QString widgetType = mLayer->editorWidgetV2( fldIdx );
        const QgsEditorWidgetConfig widgetConfig = mLayer->editorWidgetV2Config( fldIdx );

        QgsEditorWidgetWrapper* eww = QgsEditorWidgetRegistry::instance()->create( widgetType, mLayer, fldIdx, widgetConfig, 0, this );
        newWidget = eww->widget();
        mWidgets.append( eww );
      }

      labelOnTop = mLayer->labelOnTop( fieldDef->idx() );
      labelText = mLayer->attributeDisplayName( fieldDef->idx() );
      break;
    }

    case QgsAttributeEditorElement::AeTypeRelation:
    {
      const QgsAttributeEditorRelation* relDef = dynamic_cast<const QgsAttributeEditorRelation*>( widgetDef );

      // TODO: Make relationeditor newstyle
      // newWidget = QgsRelationEditorWidget::createRelationEditor( relDef->relation(), feat, context );
      labelText = QString::null;
      labelOnTop = true;
      break;
    }

    case QgsAttributeEditorElement::AeTypeContainer:
    {
      const QgsAttributeEditorContainer* container = dynamic_cast<const QgsAttributeEditorContainer*>( widgetDef );
      QWidget* myContainer;

      if ( container->isGroupBox() )
      {
        QGroupBox* groupBox = new QGroupBox( parent );
        groupBox->setTitle( container->name() );
        myContainer = groupBox;
        newWidget = myContainer;
      }
      else
      {
        QScrollArea *scrollArea = new QScrollArea( parent );

        myContainer = new QWidget( scrollArea );

        scrollArea->setWidget( myContainer );
        scrollArea->setWidgetResizable( true );
        scrollArea->setFrameShape( QFrame::NoFrame );

        newWidget = scrollArea;
      }

      QGridLayout* gbLayout = new QGridLayout( myContainer );
      myContainer->setLayout( gbLayout );

      int index = 0;

      QList<QgsAttributeEditorElement*> children = container->children();

      Q_FOREACH( QgsAttributeEditorElement* childDef, children )
      {
        QString labelText;
        bool labelOnTop;
        QWidget* editor = createWidgetFromDef( childDef, myContainer, vl, context, labelText, labelOnTop );

        if ( labelText == QString::null )
        {
          gbLayout->addWidget( editor, index, 0, 1, 2 );
        }
        else
        {
          QLabel* mypLabel = new QLabel( labelText );
          if ( labelOnTop )
          {
            gbLayout->addWidget( mypLabel, index, 0, 1, 2 );
            ++index;
            gbLayout->addWidget( editor, index, 0, 1 , 2 );
          }
          else
          {
            gbLayout->addWidget( mypLabel, index, 0 );
            gbLayout->addWidget( editor, index, 1 );
          }
        }

        ++index;
      }
      gbLayout->addItem( new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding ), index , 0 );

      labelText = QString::null;
      labelOnTop = true;
      break;
    }

    default:
      QgsDebugMsg( "Unknown attribute editor widget type encountered..." );
      break;
  }

  return newWidget;
}

void QgsAttributeForm::createWrappers()
{
  Q_FOREACH( const QgsField& field, mLayer->pendingFields().toList() )
  {
    int idx = mLayer->pendingFields().fieldNameIndex( field.name() );

    const QString widgetType = mLayer->editorWidgetV2( idx );
    const QgsEditorWidgetConfig widgetConfig = mLayer->editorWidgetV2Config( idx );

    QList<QWidget*> editors = findChildren<QWidget*>( field.name() );
    Q_FOREACH( QWidget* editor, editors )
    {
      QgsEditorWidgetWrapper* eww = QgsEditorWidgetRegistry::instance()->create( widgetType, mLayer, idx, widgetConfig, editor, this );
      mWidgets.append( eww );
    }
  }
}

void QgsAttributeForm::connectWrappers()
{
  Q_FOREACH( QgsEditorWidgetWrapper* eww, mWidgets )
  connect( eww, SIGNAL( valueChanged( const QVariant& ) ), this, SLOT( onAttributeChanged( const QVariant& ) ) );
}
