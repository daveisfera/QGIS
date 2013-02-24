/***************************************************************************
    qgsrelationmanagertreemodel.h
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

#ifndef QGSRELATIONMANAGERTREEMODEL_H
#define QGSRELATIONMANAGERTREEMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QList>
#include <QString>
#include <QTreeView>

#include "qgsrelationmanager.h"

class QgsRelation;

class QgsRelationManagerTreeModel : public QAbstractItemModel
{
    Q_OBJECT

  class RelationTreeItemRelation;
  class RelationTreeItemReference;

  class RelationTreeItem
  {
    public:
      RelationTreeItem( int id, QgsRelationManagerTreeModel* model, RelationTreeItem* parent )
        : mId( id )
        , mModel( model )
        , mParent( parent ) {}
      virtual int rowCount() = 0;
      virtual int rowId( int row ) { return -1; }
      virtual int itemToRow( RelationTreeItem* item ) = 0;
      virtual QVariant data( const QModelIndex& index, int role ) const = 0;
      virtual bool spanColumn() { return false; }

      int id() { return mId; }
      RelationTreeItem* parent() { return mParent; }

    protected:
      QgsRelationManagerTreeModel* model() { return mModel; }

    private:
      int mId;
      QgsRelationManagerTreeModel* mModel;
      RelationTreeItem* mParent;
  };

  class RelationTreeItemLayer : public RelationTreeItem
  {
    public:
      RelationTreeItemLayer( int id, QgsRelationManagerTreeModel* model )
        : RelationTreeItem( id, model, NULL ) {}
      void addRelation( const QgsRelation& relation );
      void removeRelation( RelationTreeItemRelation* relItem  );
      virtual int rowCount() { return mRelations.size(); }
      virtual int rowId( int row ) { RelationTreeItemRelation* relItem = mRelations.at( row ); return relItem ? relItem->id() : -1; }
      virtual int itemToRow( RelationTreeItem* item ) { return mRelations.indexOf( dynamic_cast< RelationTreeItemRelation* > ( item ) ); }
      virtual QVariant data( const QModelIndex& index, int role ) const { return mLayerId; }
      virtual bool spanColumn() { return true; }

      QList < QgsRelation > relations();

      QString mLayerId;
      QgsVectorLayer* mLayer;
      QList< RelationTreeItemRelation* > mRelations;
  };

  class RelationTreeItemRelation : public RelationTreeItem
  {
    public:
      RelationTreeItemRelation( int id, QgsRelationManagerTreeModel* model, RelationTreeItemLayer* parent )
        : RelationTreeItem( id, model, parent ) {}
      ~RelationTreeItemRelation();
      virtual int rowCount()  { return mReferences.size(); }
      virtual QVariant data( const QModelIndex& index, int role ) const;
      virtual bool spanColumn() { return true; }
      virtual int rowId( int row ) { RelationTreeItemReference* refItem = mReferences.at( row ); return refItem ? refItem->id() : -1; }
      virtual int itemToRow( RelationTreeItem* item ) { return mReferences.indexOf( dynamic_cast< RelationTreeItemReference* > ( item ) ); }
      void setFieldPairs( QList< QgsRelation::FieldPair > fieldPairs );
      QgsRelation relation();

      QString mRelationName;
      QString mReferencedLayerId;
      QList < RelationTreeItemReference* > mReferences;
  };

  class RelationTreeItemReference : public RelationTreeItem
  {
    public:
      RelationTreeItemReference( int id, QgsRelationManagerTreeModel* model, RelationTreeItemRelation* parent )
        : RelationTreeItem( id, model, parent ) {}
      ~RelationTreeItemReference();
      virtual int rowCount()  { return 0; }
      virtual int itemToRow( RelationTreeItem* item ) { return -1; }
      virtual QVariant data( const QModelIndex& index, int role ) const
      {
        if ( index.column() == 0 )
        {
          return mFieldPair.first.name();
        }
        else if ( index.column() == 1 )
        {
          return mFieldPair.second.name();
        }
        return QVariant();
      }

      QgsRelation::FieldPair fieldPair() { return mFieldPair; }

      QgsRelation::FieldPair mFieldPair;
      RelationTreeItemRelation* mParentItem;
  };

  public:
    explicit QgsRelationManagerTreeModel( QTreeView* parent = 0 );

    QList< QgsRelation > relations ();
    void addRelation( const QgsRelation& relation );
    void removeRelation( const QModelIndex& index );
    void updateRelation( const QModelIndex& index, const QgsRelation& relation );

    void registerItem( int itemId, RelationTreeItem* item );
    void unregisterItem( int itemId );
    inline int getId() { return ++mIndex; }

    int columnCount( const QModelIndex& parent ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    int rowCount( const QModelIndex &parent ) const;
    QModelIndex index( int row, int column, const QModelIndex& parent ) const;
    QModelIndex parent( const QModelIndex& child ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    QModelIndex indexFromItem( RelationTreeItem* item ) const;

  signals:
    
  public slots:

  private:
    QTreeView* mTreeView;
    QList<QgsVectorLayer*> mLayers;
    QList< RelationTreeItemLayer* > mLayerItems;
    QMap< int, RelationTreeItem* > mItems;
    int mIndex; // The next item id to be delivered from getId
};

#endif // QGSRELATIONMANAGERTREEMODEL_H
