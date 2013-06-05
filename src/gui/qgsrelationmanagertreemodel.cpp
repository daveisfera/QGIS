/***************************************************************************
    qgsrelationmanagertreemodel.cpp
     --------------------------------------
    Date                 : 4.3.2013
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

#include <QMessageBox>

#include <QDebug>
#include "qgsrelationmanagertreemodel.h"
#include "qgsmaplayerregistry.h"
#include "qgsmaplayer.h"

QgsRelationManagerTreeModel::QgsRelationManagerTreeModel( QTreeView *parent ) :
    QAbstractItemModel( parent )
    , mTreeView( parent )
    , mIndex( 0 )
{

}

QList< QgsRelation > QgsRelationManagerTreeModel::relations()
{
  QList< QgsRelation > relations;

  foreach ( RelationTreeItemLayer* layerIt, mLayerItems )
  {
    relations.append( layerIt->relations() );
  }

  return relations;
}

int QgsRelationManagerTreeModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED( parent )
  return 2;
}

QVariant QgsRelationManagerTreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if ( role == Qt::DisplayRole && orientation == Qt::Horizontal )
  {
    if ( 0 == section )
    {
      return tr( "Referencing field" );
    }
    if ( 1 == section )
    {
      return tr( "Referenced field" );
    }
  }

  return QAbstractItemModel::headerData( section, orientation, role );
}

int QgsRelationManagerTreeModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
  {
    // first level items: layers
    return mLayerItems.size();
  }
  else
  {
    qDebug() << "rowCount " << parent.internalId();

    if ( !mItems.contains( parent.internalId() ) )
    {
      return 0;
    }
    mTreeView->setFirstColumnSpanned( parent.row(), parent.parent(), mItems[ parent.internalId()]->spanColumn() );
    return mItems[ parent.internalId()]->rowCount();
  }
}

QModelIndex QgsRelationManagerTreeModel::index( int row, int column, const QModelIndex& parent ) const
{
  if ( !parent.isValid() )
  {
    if ( row < 0 || row > mLayerItems.size() )
    {
      return QModelIndex();
    }
    return createIndex( row, column, mLayerItems[row]->id() );
  }
  else
  {
    return createIndex( row, column, mItems[ parent.internalId()]->rowId( row ) );
  }
}

QModelIndex QgsRelationManagerTreeModel::parent( const QModelIndex& child ) const
{
  // TODO: Change to assert
  if ( !mItems.contains( child.internalId() ) )
  {
    qDebug() << "oops";
    return QModelIndex();
  }

  RelationTreeItem* parentItem = mItems[ child.internalId()]->parent();

  if ( !parentItem )
  {
    return QModelIndex();
  }
  else
  {
    return createIndex( 0, 0, parentItem->id() );
  }
}

QVariant QgsRelationManagerTreeModel::data( const QModelIndex& index, int role ) const
{
  switch ( role )
  {
    case Qt::DisplayRole:
    case Qt::BackgroundColorRole:
      qDebug() << "data " << index.internalId();
      if ( !mItems.contains( index.internalId() ) )
      {
        return tr( "No data" );
      }
      return mItems[ index.internalId()]->data( index, role );
      break;

    default:
      return QVariant();
      break;
  }
}

QModelIndex QgsRelationManagerTreeModel::indexFromItem( QgsRelationManagerTreeModel::RelationTreeItem *item ) const
{
  if ( !item )
  {
    return QModelIndex();
  }

  int row;
  RelationTreeItem* parent = item->parent();

  if ( !parent )
  {
    row = mLayerItems.indexOf( dynamic_cast< RelationTreeItemLayer* >( item ) );
  }
  else
  {
    row = parent->itemToRow( item );
  }

  return createIndex( row , 0, item->id() );
}

void QgsRelationManagerTreeModel::addRelation( const QgsRelation& relation )
{
  RelationTreeItemLayer* layerItem = NULL;

  foreach ( RelationTreeItemLayer* item, mLayerItems )
  {
    if ( item->mLayerId == relation.referencingLayerId() )
    {
      layerItem = item;
    }
  }

  if ( !layerItem )
  {
    int itemId = getId();
    layerItem = new RelationTreeItemLayer( itemId, this );
    layerItem->mLayerId = relation.referencingLayerId();
    QgsMapLayer* layer = QgsMapLayerRegistry::instance()->mapLayer( relation.referencingLayerId() );
    if ( layer )
    {
      layerItem->mLayerName = layer->name();
    }
    else
    {
      layerItem->mLayerName = relation.referencingLayerId();
    }
    registerItem( itemId, layerItem );
    beginInsertRows( QModelIndex(), mLayerItems.size(), mLayerItems.size() );
    mLayerItems.append( layerItem );
    endInsertRows();
  }

  layerItem->addRelation( relation );
}

void QgsRelationManagerTreeModel::removeRelation( const QModelIndex &index )
{
  if ( mItems.contains( index.internalId() ) )
  {
    RelationTreeItem* clickedItem = mItems[ index.internalId()];
    RelationTreeItemRelation* relationItem = dynamic_cast< RelationTreeItemRelation* >( clickedItem );
    if ( !relationItem )
    {
      RelationTreeItemReference* referenceItem = dynamic_cast< RelationTreeItemReference* >( clickedItem );
      if ( !referenceItem ) // Probably a top level (layer) item
        return;

      relationItem = dynamic_cast< RelationTreeItemRelation* >( referenceItem->parent() );
    }

    if ( relationItem )
    {
      RelationTreeItemLayer* layerItem = dynamic_cast< RelationTreeItemLayer* >( relationItem->parent() );
      if ( layerItem )
      {
        layerItem->removeRelation( relationItem );
      }
    }
    else
    {
      QMessageBox::information( mTreeView, tr( "Choose a relation" ),
                                tr( "You did not choose a relation.\n"
                                    "Please choose the relation you want to delete." ),
                                QMessageBox::Ok );
    }
  }
}

void QgsRelationManagerTreeModel::registerItem( int itemId, QgsRelationManagerTreeModel::RelationTreeItem* item )
{
  mItems.insert( itemId, item );
}

void QgsRelationManagerTreeModel::unregisterItem( int itemId )
{
  mItems.remove( itemId );
}

//////////////////////////

void QgsRelationManagerTreeModel::RelationTreeItemLayer::addRelation( const QgsRelation& relation )
{
  int itemId = model()->getId();
  QgsRelationManagerTreeModel::RelationTreeItemRelation* relationItem = new QgsRelationManagerTreeModel::RelationTreeItemRelation( itemId, model(), this );
  relationItem->mRelationName = relation.name();
  relationItem->mReferencedLayerId = relation.referencedLayerId();
  QgsMapLayer* layer = QgsMapLayerRegistry::instance()->mapLayer( relation.referencedLayerId() );
  if ( layer )
  {
    relationItem->mReferencedLayerName = layer->name();
  }
  else
  {
    relationItem->mReferencedLayerName = relation.referencedLayerId();
  }
  relationItem->setFieldPairs( relation.fieldPairs() );
  model()->beginInsertRows( model()->indexFromItem( this ), mRelations.count(), mRelations.count() );
  mRelations.append( relationItem );
  model()->registerItem( itemId, relationItem );
  model()->endInsertRows();
}

void QgsRelationManagerTreeModel::RelationTreeItemLayer::removeRelation( RelationTreeItemRelation* relItem )
{
  int row = mRelations.indexOf( relItem );
  model()->beginRemoveRows( model()->indexFromItem( relItem->parent() ), row, row );
  mRelations.removeOne( relItem );
  model()->endRemoveRows();
  delete( relItem );
}

QVariant QgsRelationManagerTreeModel::RelationTreeItemLayer::data( const QModelIndex& index, int role ) const
{
  Q_UNUSED( index )
  Q_UNUSED( role )

  if ( role == Qt::DisplayRole )
  {
    return mLayerName;
  }
  else
  {
    return QVariant();
  }
}

QList< QgsRelation > QgsRelationManagerTreeModel::RelationTreeItemLayer::relations()
{
  QList < QgsRelation > relations;

  foreach ( RelationTreeItemRelation* relation, mRelations )
  {
    QgsRelation rel = relation->relation();
    rel.setReferencingLayer( mLayerId );
    relations.append( rel );
  }

  return relations;
}

QgsRelationManagerTreeModel::RelationTreeItemRelation::~RelationTreeItemRelation()
{
  model()->unregisterItem( id() );
  foreach ( RelationTreeItemReference* ref, mReferences )
  {
    delete ref;
  }
}

QVariant QgsRelationManagerTreeModel::RelationTreeItemRelation::data( const QModelIndex& index, int role ) const
{
  Q_UNUSED( index )

  switch ( role )
  {
    case Qt::DisplayRole:
      return mRelationName + " ( " + mReferencedLayerName + " )";
      break;

    case Qt::BackgroundColorRole:
      return QColor( 0xDD, 0xDD, 0xDD );
      break;

    default:
      break;
  }

  return QVariant();
}

void QgsRelationManagerTreeModel::RelationTreeItemRelation::setFieldPairs( QList<QgsRelation::FieldPair> fieldPairs )
{
  // TODO: Remove old

  foreach ( QgsRelation::FieldPair fieldPair, fieldPairs )
  {
    int itemId = model()->getId();
    QgsRelationManagerTreeModel::RelationTreeItemReference* referenceItem = new QgsRelationManagerTreeModel::RelationTreeItemReference( itemId, model(), this );
    referenceItem->mFieldPair = fieldPair;
    mReferences.append( referenceItem );
    model()->registerItem( itemId, referenceItem );
  }
}

QgsRelation QgsRelationManagerTreeModel::RelationTreeItemRelation::relation()
{
  QgsRelation relation;

  relation.setRelationName( mRelationName );
  relation.setReferencedLayer( mReferencedLayerId );

  foreach ( RelationTreeItemReference* reference, mReferences )
  {
    relation.addFieldPair( reference->fieldPair() );
  }

  return relation;
}


QgsRelationManagerTreeModel::RelationTreeItemReference::~RelationTreeItemReference()
{
  model()->unregisterItem( this->id() );
}

int QgsRelationManagerTreeModel::RelationTreeItemReference::itemToRow( QgsRelationManagerTreeModel::RelationTreeItem* item )
{
  Q_UNUSED( item )
  return -1;
}

QVariant QgsRelationManagerTreeModel::RelationTreeItemReference::data( const QModelIndex& index, int role ) const
{
  switch ( role )
  {
    case Qt::DisplayRole:
      if ( index.column() == 0 )
      {
        return mFieldPair.referencingField();
      }
      else if ( index.column() == 1 )
      {
        return mFieldPair.referencedField();
      }
      break;

    case Qt::BackgroundColorRole:
      return QColor( 0xBB, 0xBB, 0xBB );
      break;

    default:
      break;
  }

  return QVariant();
}

int QgsRelationManagerTreeModel::RelationTreeItem::rowId( int row )
{
  Q_UNUSED( row )
  return -1;
}


void QgsRelationManagerItemDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
  QStyleOptionViewItem myOption = option;
  myOption.palette.setBrush( QPalette::Background, index.data( Qt::BackgroundColorRole ).value<QColor>() );
  QItemDelegate::paint( painter, myOption, index );
}

