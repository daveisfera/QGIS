/***************************************************************************
    qgsrelationreferencewidget.cpp
     --------------------------------------
    Date                 : 20.4.2013
    Copyright            : (C) 2013 Matthias Kuhn
    Email                : matthias dot kuhn at gmx dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsrelationreferencewidget.h"

#include <QDialog>
#include <QHBoxLayout>

#include "qgsattributedialog.h"
#include "qgsapplication.h"
#include "qgscollapsiblegroupbox.h"
#include "qgseditorwidgetfactory.h"
#include "qgsexpression.h"
#include "qgsfield.h"
#include "qgsmapcanvas.h"
#include "qgsrelreferenceconfigdlg.h"
#include "qgsvectorlayer.h"

QgsRelationReferenceWidget::QgsRelationReferenceWidget( QWidget* parent )
    : QWidget( parent )
    , mEditorContext( QgsAttributeEditorContext() )
    , mInitialValueAssigned( false )
    , mMapTool( NULL )
    , mParentAttributeDialog( NULL )
    , mReferencedAttributeDialog( NULL )
    , mReferencedLayer( NULL )
    , mEmbedForm( false )
    , mReadOnlySelector( false )
    , mAllowMapIdentification( false )
{
  mTopLayout = new QVBoxLayout( this );
  mTopLayout->setContentsMargins( 0, 0, 0, 0 );
  setLayout( mTopLayout );

  QHBoxLayout* editLayout = new QHBoxLayout();
  editLayout->setContentsMargins( 0, 0, 0, 0 );

  // combobox (for non-geometric relation)
  mComboBox = new QComboBox( this );
  editLayout->addWidget( mComboBox );

  // read-only line edit
  mLineEdit = new QLineEdit( this );
  mLineEdit->setReadOnly( true );
  editLayout->addWidget( mLineEdit );

  // action button
  mAttributeEditorButton = new QToolButton( this );
  mShowFormAction = new QAction( QgsApplication::getThemeIcon( "/mActionToggleEditing.svg" ), tr( "Open Form" ), this );
  // to be added pan to feature
  mAttributeEditorButton->addAction( mShowFormAction );
  mAttributeEditorButton->setDefaultAction( mShowFormAction );
  connect( mAttributeEditorButton, SIGNAL( triggered( QAction* ) ), this, SLOT( openForm() ) );
  editLayout->addWidget( mAttributeEditorButton );

  // map identification button
  mMapIdentificationButton = new QToolButton( this );
  mMapIdentificationAction = new QAction( QgsApplication::getThemeIcon( "/mActionMapIdentification.svg" ), tr( "Select on map" ), this );
  mMapIdentificationButton->addAction( mMapIdentificationAction );
  mMapIdentificationButton->setDefaultAction( mMapIdentificationAction );
  connect( mMapIdentificationButton, SIGNAL( triggered( QAction* ) ), this, SLOT( mapIdentification() ) );
  editLayout->addWidget( mMapIdentificationButton );

  // spacer
  editLayout->addItem( new QSpacerItem( 0, 0, QSizePolicy::Expanding ) );

  // add line to top layout
  mTopLayout->addLayout( editLayout );

  // embed form
  mAttributeEditorFrame = new QgsCollapsibleGroupBox( this );
  mAttributeEditorFrame->setCollapsed( true );
  mAttributeEditorLayout = new QVBoxLayout( mAttributeEditorFrame );
  mAttributeEditorFrame->setLayout( mAttributeEditorLayout );
  mTopLayout->addWidget( mAttributeEditorFrame );

  // default mode is combobox, non geometric relation and no embed form
  mLineEdit->hide();
  mMapIdentificationButton->hide();

  // get if the widget is shown in attribute dialog
  QWidget* pwdg = parent;
  while ( pwdg )
  {
    if ( pwdg->objectName() == "QgsAttributeDialog" )
    {
      mParentAttributeDialog = qobject_cast<QDialog*>( pwdg );
      break;
    }
    pwdg = pwdg->parentWidget();
  }
}

void QgsRelationReferenceWidget::setRelation( QgsRelation relation, bool allowNullValue )
{
  if ( relation.isValid() )
  {
    mReferencedLayer = relation.referencedLayer();
    int refFieldIdx = mReferencedLayer->fieldNameIndex( relation.fieldPairs().first().second );

    QgsFeatureIterator fit = mReferencedLayer->getFeatures( QgsFeatureRequest() );

    QgsExpression exp( mReferencedLayer->displayExpression() );
    exp.prepare( mReferencedLayer->pendingFields() );

    QgsFeature f;
    while ( fit.nextFeature( f ) )
    {
      QString txt = exp.evaluate( &f ).toString();

      mComboBox->addItem( txt, f.id() );

      mFidFkMap.insert( f.id(), f.attribute( refFieldIdx ) );
    }

    if ( allowNullValue )
    {
      const QString nullValue = QSettings().value( "qgis/nullValue", "NULL" ).toString();
      mComboBox->addItem( nullValue );
      mComboBox->setItemData( mComboBox->count() - 1, Qt::gray, Qt::ForegroundRole );
    }

    // Only connect after iterating, to have only one iterator on the referenced table at once
    connect( mComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( referenceChanged( int ) ) );
  }
  else
  {
    QLabel* lbl = new QLabel( tr( "The relation is not valid. Please make sure your relation definitions are ok." ) );
    QFont font = lbl->font();
    font.setItalic( true );
    lbl->setStyleSheet( "QLabel { color: red; } " );
    lbl->setFont( font );
    mTopLayout->addWidget( lbl, 1, 0 );
  }
}

void QgsRelationReferenceWidget::setRelationEditable( bool editable )
{
  mLineEdit->setEnabled( editable );
  mComboBox->setEnabled( editable );
  mMapIdentificationButton->setVisible( editable );
}

void QgsRelationReferenceWidget::setRelatedFeature( const QVariant& value )
{
  QgsFeatureId fid = mFidFkMap.key( value );
  if ( mReferencedLayer )
    setRelatedFeature( fid );
}

void QgsRelationReferenceWidget::setRelatedFeature( const QgsFeatureId& fid )
{
  int oldIdx = mComboBox->currentIndex();
  int newIdx = mComboBox->findData( fid );
  mComboBox->setCurrentIndex( newIdx );

  if ( !mInitialValueAssigned )
  {
    // In case the default-selected item (first) is the actual item
    // then no referenceChanged event was triggered automatically:
    // Do it!
    if ( oldIdx == mComboBox->currentIndex() )
      referenceChanged( mComboBox->currentIndex() );
    mInitialValueAssigned = true;
  }

  // update line edit
  mLineEdit->setText( mFidFkMap.value( fid ).toString() );
}

void QgsRelationReferenceWidget::mapToolDeactivated()
{
  if ( mParentAttributeDialog )
  {
    mParentAttributeDialog->show();
  }
}

QVariant QgsRelationReferenceWidget::relatedFeature()
{
  QVariant varFid = mComboBox->itemData( mComboBox->currentIndex() );
  if ( varFid.isNull() )
  {
    return QVariant();
  }
  else
  {
    return mFidFkMap.value( varFid.value<QgsFeatureId>() );
  }
}

void QgsRelationReferenceWidget::setEditorContext( QgsAttributeEditorContext context )
{
  mEditorContext = context;
}

void QgsRelationReferenceWidget::setEmbedForm( bool display )
{
  mAttributeEditorFrame->setVisible( display );
}

void QgsRelationReferenceWidget::setReadOnlySelector( bool readOnly )
{
  mComboBox->setHidden( readOnly );
  mLineEdit->setVisible( readOnly );
}

void QgsRelationReferenceWidget::setAllowMapIdentification( bool allowMapIdentification )
{
  mMapIdentificationButton->setVisible( allowMapIdentification );
}

void QgsRelationReferenceWidget::openForm()
{
  QgsFeatureId fid = mComboBox->itemData( mComboBox->currentIndex() ).value<QgsFeatureId>();

  QgsFeature feat;

  if ( !mReferencedLayer )
    return;

  mReferencedLayer->getFeatures( QgsFeatureRequest().setFilterFid( fid ) ).nextFeature( feat );

  if ( !feat.isValid() )
    return;

  // TODO: Get a proper QgsDistanceArea thingie
  mReferencedAttributeDialog = new QgsAttributeDialog( mReferencedLayer, new QgsFeature( feat ), true, this, true, mEditorContext );
  mReferencedAttributeDialog->exec();
  delete mReferencedAttributeDialog;
}

void QgsRelationReferenceWidget::mapIdentification()
{
  QgsVectorLayerTools* tools = mEditorContext.vectorLayerTools();
  if ( !tools )
    return;

  mMapTool = tools->identifySingleFeature( mReferencedLayer );
  mMapTool->canvas()->setMapTool( mMapTool );
  connect( mMapTool, SIGNAL( featureIdentified( QgsFeatureId ) ), this, SLOT( featureIdentified( QgsFeatureId ) ) );
  connect( mMapTool, SIGNAL( deactivated() ), this, SLOT( mapToolDeactivated() ) );

  if ( mParentAttributeDialog )
  {
    mParentAttributeDialog->hide();
  }
}

void QgsRelationReferenceWidget::referenceChanged( int index )
{
  QgsFeatureId fid = mComboBox->itemData( index ).value<QgsFeatureId>();

  emit relatedFeatureChanged( mFidFkMap.value( fid ) );

  // Check if we're running with an embedded frame we need to update
  if ( mAttributeEditorFrame )
  {
    QgsFeature feat;

    mReferencedLayer->getFeatures( QgsFeatureRequest().setFilterFid( fid ) ).nextFeature( feat );

    if ( feat.isValid() )
    {
      // Backup old dialog and delete only after creating the new dialog, so we can "hot-swap" the contained QgsFeature
      QgsAttributeDialog* oldDialog = mReferencedAttributeDialog;

      if ( mReferencedAttributeDialog && mReferencedAttributeDialog->dialog() )
      {
        mAttributeEditorLayout->removeWidget( mReferencedAttributeDialog->dialog() );
      }

      // TODO: Get a proper QgsDistanceArea thingie
      mReferencedAttributeDialog = new QgsAttributeDialog( mReferencedLayer, new QgsFeature( feat ), true, mAttributeEditorFrame, false, mEditorContext );
      QWidget* attrDialog = mReferencedAttributeDialog->dialog();
      attrDialog->setWindowFlags( Qt::Widget ); // Embed instead of opening as window
      mAttributeEditorLayout->addWidget( attrDialog );
      attrDialog->show();

      delete oldDialog;
    }
  }
}

void QgsRelationReferenceWidget::featureIdentified( const QgsFeatureId& fid )
{
  setRelatedFeature( fid );

  // deactivate map tool if activate
  if ( mMapTool )
  {
    QgsMapCanvas* canvas = mMapTool->canvas();
    canvas->unsetMapTool( mMapTool );
  }
}
