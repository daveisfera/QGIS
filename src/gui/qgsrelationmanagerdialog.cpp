/***************************************************************************
    qgsrelationmanagerdialog.cpp
     --------------------------------------
    Date                 : 23.2.2013
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

#include "qgsrelationmanager.h"
#include "qgsrelationmanagerdialog.h"
#include "qgsrelationadddlg.h"
#include "qgsvectorlayer.h"
#include "qgsrelationmanagertreemodel.h"

QgsRelationManagerDialog::QgsRelationManagerDialog( QgsRelationManager* relationMgr, QWidget *parent ) :
    QWidget( parent ),
    Ui::QgsRelationManagerDialogBase(),
    mRelationManager( relationMgr )
{
  setupUi( this );
  mRelationTreeModel = new QgsRelationManagerTreeModel( mRelationsTreeWdg );
  mRelationsTreeWdg->setItemDelegate( new QgsRelationManagerItemDelegate( mRelationsTreeWdg ) );
  mRelationsTreeWdg->setModel( mRelationTreeModel );
  mRelationsTreeWdg->setSelectionMode( QAbstractItemView::SingleSelection );
  mRelationsTreeWdg->setSelectionBehavior( QAbstractItemView::SelectRows );
}

QgsRelationManagerDialog::~QgsRelationManagerDialog()
{
}

void QgsRelationManagerDialog::setLayers( QList< QgsVectorLayer* > layers )
{
  mLayers = layers;

  const QList<QgsRelation>& relations = mRelationManager->relations().values();

  foreach ( const QgsRelation rel, relations )
  {
    addRelation( rel );
  }
}

void QgsRelationManagerDialog::addRelation( const QgsRelation &rel )
{
  mRelationTreeModel->addRelation( rel );
}

void QgsRelationManagerDialog::on_mBtnAddRelation_clicked()
{
  QgsRelationAddDlg addDlg;
  addDlg.addLayers( mLayers );

  if ( addDlg.exec() )
  {
    QgsRelation relation;

    relation.setReferencingLayer( addDlg.referencingLayerId() );
    relation.setReferencedLayer( addDlg.referencedLayerId() );
    relation.setRelationName( addDlg.relationName() );
    relation.addFieldPair( addDlg.references().first().first, addDlg.references().first().second );

    mRelationTreeModel->addRelation( relation );
  }
}

void QgsRelationManagerDialog::on_mBtnRemoveRelation_clicked()
{
  mRelationTreeModel->removeRelation( mRelationsTreeWdg->currentIndex() );
}

QList< QgsRelation > QgsRelationManagerDialog::relations()
{
  return mRelationTreeModel->relations();
}

#if 0
QTreeWidgetItem* QgsRelationManagerDialog::findLayerItem( QString id )
{
  int count = mRelationsTreeWdg->topLevelItemCount();

  for ( int i = 0; i < count; ++i )
  {
    QTreeWidgetItem* item = mRelationsTreeWdg->topLevelItem( i );
    if ( item->data( 0, Qt::UserRole ).toString() == id )
    {
      return item;
    }
  }

  return NULL;
}
#endif
