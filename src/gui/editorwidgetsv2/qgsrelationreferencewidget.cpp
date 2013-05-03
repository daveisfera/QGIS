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

#include <QPushButton>
#include <QDialog>

#include "qgsattributedialog.h"
#include "qgseditorwidgetfactory.h"
#include "qgsexpression.h"
#include "qgsfield.h"
#include "qgsrelreferenceconfigdlg.h"
#include "qgsrelationmanager.h"
#include "qgsvectorlayer.h"

QgsRelationReferenceWidget::QgsRelationReferenceWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent )
    : QgsEditorWidgetWrapper( vl, fieldIdx, editor, parent )
    , mComboBox( NULL )
    , mAttributeEditorFrame( NULL )
    , mAttributeEditorLayout( NULL )
    , mReferencedLayer( NULL )
    , mAttributeDialog( NULL )

{
}

QWidget* QgsRelationReferenceWidget::createWidget( QWidget* parent )
{
  return new QWidget( parent );
}

void QgsRelationReferenceWidget::initWidget( QWidget* editor )
{
  QGridLayout* layout = new QGridLayout( editor );
  editor->setLayout( layout );

  mComboBox = new QComboBox( editor );
  mAttributeEditorFrame = new QFrame( editor );
  mAttributeEditorLayout = new QVBoxLayout( mAttributeEditorFrame );
  mAttributeEditorFrame->setLayout( mAttributeEditorLayout );

  layout->addWidget( mComboBox );
  layout->addWidget( mAttributeEditorFrame );

  QgsRelation relation = QgsRelationManager::instance()->relation( config( "Relation" ).toString() );

  if ( relation.isValid() )
  {
    mReferencedLayer = relation.referencedLayer();
    int refFieldIdx = mReferencedLayer->fieldNameIndex( relation.fieldPairs().first().second.name() );

    QgsFeatureIterator fit = mReferencedLayer->getFeatures( QgsFeatureRequest() );

    QgsExpression exp ( mReferencedLayer->displayExpression() );
    exp.prepare( mReferencedLayer->pendingFields() );

    QgsFeature f;
    while ( fit.nextFeature( f ) )
    {
      QString txt = exp.evaluate( &f ).toString();

      mComboBox->addItem( txt, f.id() );
    }

    if ( config( "AllowNULL" ).toBool() )
    {
      mComboBox->addItem( "[NULL]" );
    }

    // Only connect after iterating, because there will be yet another request
    connect( mComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( referenceChanged(int) ) );
  }
}

QVariant QgsRelationReferenceWidget::value()
{
  return mComboBox->itemData( mComboBox->currentIndex() );
}

void QgsRelationReferenceWidget::setValue( const QVariant& value )
{
  mComboBox->setCurrentIndex( mComboBox->findData( value ) );
}

void QgsRelationReferenceWidget::referenceChanged( int index )
{
  QgsFeatureId fid = mComboBox->itemData( index ).toInt();

  QgsFeature feat;

  mReferencedLayer->getFeatures( QgsFeatureRequest().setFilterFid( fid ) ).nextFeature( feat );

  if ( !feat.isValid() )
    return;

  // Backup old dialog and delete only after creating the new dialog, so we can "hot-swap" the contained QgsFeature
  QgsAttributeDialog* oldDialog = mAttributeDialog;

  if ( mAttributeDialog && mAttributeDialog->dialog() )
  {
    mAttributeEditorLayout->removeWidget( mAttributeDialog->dialog() );
  }

  // TODO: Get a proper QgsDistanceArea thingie
  mAttributeDialog = new QgsAttributeDialog( mReferencedLayer, new QgsFeature( feat ), true, QgsDistanceArea(), mAttributeEditorFrame, false );
  QWidget* attrDialog = mAttributeDialog->dialog();
  attrDialog->setWindowFlags( Qt::Widget );
  mAttributeEditorLayout->addWidget( attrDialog );
  attrDialog->show();

  delete oldDialog;
}

template <>
QMap<QString, QVariant> QgsEditWidgetFactoryHelper<QgsRelationReferenceWidget, QgsRelReferenceConfigDlg>::readConfig( const QDomElement& configElement, QgsVectorLayer* layer, int fieldIdx )
{
  Q_UNUSED( layer );
  Q_UNUSED( fieldIdx );
  QMap<QString, QVariant> cfg;

  cfg.insert( "AllowNULL", configElement.attribute( "AllowNULL" ) == "1" );
  cfg.insert( "ShowForm", configElement.attribute( "ShowForm" ) == "1" );
  cfg.insert( "Relation", configElement.attribute( "Relation" ) );

  return cfg;
}

template <>
void QgsEditWidgetFactoryHelper<QgsRelationReferenceWidget, QgsRelReferenceConfigDlg>::writeConfig( const QgsEditorWidgetConfig& config, QDomElement& configElement, const QDomDocument& doc, const QgsVectorLayer* layer, int fieldIdx )
{
  Q_UNUSED( doc );
  Q_UNUSED( layer );
  Q_UNUSED( fieldIdx );

  configElement.setAttribute( "AllowNULL", config["AllowNULL"].toBool() );
  configElement.setAttribute( "ShowForm", config["ShowForm"].toBool() );
  configElement.setAttribute( "Relation", config["Relation"].toString() );
}
