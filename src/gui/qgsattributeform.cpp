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

#include "qgseditorwidgetregistry.h"
#include "qgsattributeeditor.h"

#include <QDir>
#include <QFileInfo>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QTabWidget>
#include <QUiLoader>

QgsAttributeForm::QgsAttributeForm( QgsVectorLayer* vl, const QgsFeature feature, QgsAttributeEditorContext context, QWidget* parent )
  : QWidget( parent )
  , mLayer( vl )
  , mContext( context )
{
  init();
  setFeature( feature );
}

void QgsAttributeForm::changeAttribute( const QString& field, const QVariant& value )
{
  Q_FOREACH( QgsEditorWidgetWrapper* eww, mWidgets )
  {
    if( eww->field().name() == field )
    {
      eww->setValue( value );
    }
  }
}


void QgsAttributeForm::setFeature( const QgsFeature& feature )
{
  mFeature = feature;

  Q_FOREACH( QgsEditorWidgetWrapper* eww, mWidgets )
  {
    if ( feature.isValid() )
    {
      eww->setEnabled( true );
      eww->setValue( feature.attribute( eww->field().name() ) );
    }
    else
    {
      eww->setValue( QVariant( QVariant::Int ) );
      eww->setEnabled( false );
    }
  }
}

bool QgsAttributeForm::save()
{
  if ( mFeature.isValid() )
  {
    bool doUpdate = false;

    QgsAttributes src = mFeature.attributes();
    QgsAttributes dst = mFeature.attributes();

    Q_FOREACH( QgsEditorWidgetWrapper* eww, mWidgets )
    {
      if ( src[eww->fieldIdx()] != eww->value() )
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
        if ( dst[i] == src[i] )
          continue;

        mLayer->changeAttributeValue( mFeature.id(), i, dst[i], src[i] );
      }

      mLayer->endEditCommand();
    }
  }

  return true;
}

void QgsAttributeForm::onAttributeChanged( QVariant& value )
{
  QgsEditorWidgetWrapper* eww = qobject_cast<QgsEditorWidgetWrapper*>( sender() );

  Q_ASSERT ( eww );

  emit attributeChanged( eww->field().name(), value );
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
      initWrappers();
    }
  }

  // Tab layout
  if ( !formWidget && mLayer->editorLayout() == QgsVectorLayer::TabLayout )
  {
    QTabWidget* tabWidget = new QTabWidget( this );
    layout()->addWidget( tabWidget );

    Q_FOREACH ( const QgsAttributeEditorElement *widgDef, mLayer->attributeEditorElements() )
    {
      QWidget* tabPage = new QWidget( tabWidget );

      tabWidget->addTab( tabPage, widgDef->name() );
      QGridLayout *tabPageLayout = new QGridLayout( tabPage );

      if ( widgDef->type() == QgsAttributeEditorElement::AeTypeContainer )
      {
        QString dummy1;
        bool dummy2;
        tabPageLayout->addWidget( QgsAttributeEditor::createWidgetFromDef( widgDef, tabPage, mLayer, mFeature, mContext, dummy1, dummy2 ) );
      }
      else
      {
        QgsDebugMsg( "No support for fields in attribute editor on top level" );
      }
    }
    initWrappers();
    formWidget = tabWidget;
  }

  // Autogenerate Layout
  // If there is still no layout loaded (defined as autogenerate or other methods failed)
  if ( !formWidget )
  {
    formWidget = new QWidget( this );
    QFormLayout* formLayout = new QFormLayout( this );
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
}

void QgsAttributeForm::initWrappers()
{
  Q_FOREACH( const QgsField& field, mLayer->pendingFields().toList() )
  {
    int idx = mLayer->pendingFields().fieldNameIndex( field.name() );

    const QString widgetType = mLayer->editorWidgetV2( idx );
    const QgsEditorWidgetConfig widgetConfig = mLayer->editorWidgetV2Config( idx );

    QList<QWidget*> editors = findChildren<QWidget*>( field.name() );
    Q_FOREACH( QWidget* editor, editors)
    {
      QgsEditorWidgetWrapper* eww = QgsEditorWidgetRegistry::instance()->create( widgetType, mLayer, idx, widgetConfig, editor, this );
      connect( eww, SIGNAL( valueChanged( const QVariant& ) ), this, SLOT( onAttributeChanged( const QVariant& ) ) );
      mWidgets.append( eww );
    }
  }
}

