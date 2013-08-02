/***************************************************************************
    qgsrelationeditor.cpp
     --------------------------------------
    Date                 : 17.5.2013
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

#include "qgsrelationeditor.h"

#include "attributetable/qgsdualview.h"

#include "qgsdistancearea.h"
#include "qgsvectordataprovider.h"
#include "qgsexpression.h"
#include "qgsfeature.h"
#include "qgsfeatureselectiondlg.h"
#include "qgsgenericfeatureselectionmgr.h"
#include "qgsrelation.h"
#include "qgsvectorlayertools.h"

#include <QHBoxLayout>
#include <QLabel>

QgsRelationEditorWidget::QgsRelationEditorWidget( const QgsRelation& relation, const QgsFeature& feature, QgsAttributeEditorContext context, QWidget* parent )
    : QgsCollapsibleGroupBox( parent )
    , mDualView( NULL )
    , mEditorContext( context )
    , mRelation( relation )
    , mFeature( feature )
{
  setupUi( this );

  connect( relation.referencingLayer(), SIGNAL( editingStarted() ), this, SLOT( referencingLayerEditingToggled() ) );
  connect( relation.referencingLayer(), SIGNAL( editingStopped() ), this, SLOT( referencingLayerEditingToggled() ) );
  connect( this, SIGNAL( collapsedStateChanged( bool ) ), this, SIGNAL( onCollapsedStateChanged( bool ) ) );

  // Set initial state for add/remove etc. buttons
  referencingLayerEditingToggled();
}

QgsRelationEditorWidget* QgsRelationEditorWidget::createRelationEditor( const QgsRelation& relation, const QgsFeature& feature,  QgsAttributeEditorContext context, QWidget* parent )
{
  QgsRelationEditorWidget* editor = new QgsRelationEditorWidget( relation, feature, context, parent );

  QgsDualView* dualView = new QgsDualView( editor );
  QgsVectorLayer* lyr = relation.referencingLayer();

  bool canChangeAttributes = lyr->dataProvider()->capabilities() & QgsVectorDataProvider::ChangeAttributeValues;

  editor->mToggleEditingButton->setEnabled( canChangeAttributes && !lyr->isReadOnly() );

  editor->mFeatureSelectionMgr = new QgsGenericFeatureSelectionMgr( dualView );
  dualView->setFeatureSelectionManager( editor->mFeatureSelectionMgr );

  editor->mBrowserWidget->layout()->addWidget( dualView );

  QgsFeatureRequest myRequest = relation.getRelatedFeaturesRequest( feature );

  dualView->init( relation.referencingLayer(), NULL, myRequest, context );

  editor->mDualView = dualView;

  return editor;
}

void QgsRelationEditorWidget::onCollapsedStateChanged( bool state )
{
  if ( state && !mDualView->masterModel() )
  {
    // TODO: Lazy init dual view if collapsed on init
  }
}

void QgsRelationEditorWidget::referencingLayerEditingToggled()
{
  bool editable = mRelation.referencingLayer()->isEditable();

  mPbnNew->setEnabled( editable );
  mPbnLink->setEnabled( editable );
  mPbnDelete->setEnabled( editable );
  mPbnUnlink->setEnabled( editable );
  mToggleEditingButton->setChecked( editable );
}

void QgsRelationEditorWidget::on_mPbnNew_clicked()
{
  QgsAttributeMap keyAttrs;

  QgsFields fields = mRelation.referencingLayer()->pendingFields();

  foreach ( QgsRelation::FieldPair fieldPair, mRelation.fieldPairs() )
  {
    keyAttrs.insert( fields.indexFromName( fieldPair.referencingField() ), mFeature.attribute( fieldPair.referencedField() ) );
  }

  mEditorContext.vectorLayerTools()->addFeature( mDualView->masterModel()->layer(), keyAttrs );
}

void QgsRelationEditorWidget::on_mPbnLink_clicked()
{
  QgsFeatureSelectionDlg selectionDlg( mRelation.referencingLayer(), this );

  if ( selectionDlg.exec() )
  {
    QMap<int, QVariant> keys;
    foreach ( const QgsRelation::FieldPair fieldPair, mRelation.fieldPairs() )
    {
      int idx = mRelation.referencingLayer()->fieldNameIndex( fieldPair.referencingField() );
      QVariant val = mFeature.attribute( fieldPair.referencedField() );
      keys.insert( idx, val );
    }

    foreach ( QgsFeatureId fid, selectionDlg.selectedFeatures() )
    {
      QMapIterator<int, QVariant> it( keys );
      while ( it.hasNext() )
      {
        it.next();
        mRelation.referencingLayer()->changeAttributeValue( fid, it.key(), it.value() );
      }
    }

    mDualView->masterModel()->loadLayer();
  }
}

void QgsRelationEditorWidget::on_mPbnDelete_clicked()
{
  foreach ( QgsFeatureId fid, mFeatureSelectionMgr->selectedFeaturesIds() )
  {
    mRelation.referencingLayer()->deleteFeature( fid );
  }
}

void QgsRelationEditorWidget::on_mPbnUnlink_clicked()
{
  QMap<int, QgsField> keyFields;
  foreach ( const QgsRelation::FieldPair fieldPair, mRelation.fieldPairs() )
  {
    int idx = mRelation.referencingLayer()->fieldNameIndex( fieldPair.referencingField() );
    QgsField fld = mRelation.referencingLayer()->pendingFields().at( idx );
    keyFields.insert( idx, fld );
  }

  foreach ( QgsFeatureId fid, mFeatureSelectionMgr->selectedFeaturesIds() )
  {
    QMapIterator<int, QgsField> it( keyFields );
    while ( it.hasNext() )
    {
      it.next();
      mRelation.referencingLayer()->changeAttributeValue( fid, it.key(), QVariant( it.value().type() ) );
    }
  }

  mDualView->masterModel()->loadLayer();
}

void QgsRelationEditorWidget::on_mToggleEditingButton_toggled( bool state )
{
  if ( state )
  {
    mRelation.referencingLayer()->startEditing();
  }
  else
  {
    mRelation.referencingLayer()->commitChanges();
  }
}
