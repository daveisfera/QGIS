/***************************************************************************
    qgsvaluerelationwidget.cpp
     --------------------------------------
    Date                 : 5.1.2014
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

#include "qgsvaluerelationwidget.h"

#include "qgsmaplayerregistry.h"
#include "qgsvectorlayer.h"

QgsValueRelationWidget::QgsValueRelationWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent )
    :  QgsEditorWidgetWrapper( vl, fieldIdx, editor, parent )
{
}


QVariant QgsValueRelationWidget::value()
{
  QVariant v;

  if( mComboBox )
    v = mComboBox->itemData( mComboBox->currentIndex() );

  if ( mListWidget )
  {
    QStringList selection;
    Q_FOREACH( const QListWidgetItem* item, mListWidget->selectedItems() )
    {
      selection << item->data( Qt::UserRole ).toString();
    }

    v = selection.join( ",").prepend( "{" ).append( "}" );
  }

  return v;
}

QWidget* QgsValueRelationWidget::createWidget(QWidget* parent)
{
  if ( config("AllowMulti").toBool() )
  {
    return new QListWidget( parent );
  }
  else
  {
    return new QComboBox( parent );
  }
}

void QgsValueRelationWidget::initWidget( QWidget* editor )
{
  initCache();

  mComboBox = qobject_cast<QComboBox*>( editor );
  mListWidget = qobject_cast<QListWidget*>( editor );

  if ( mComboBox )
  {
    if ( config( "AllowNull" ).toBool() )
    {
      mComboBox->addItem( tr( "(no selection)" ), QVariant( field().type() ) );
    }

    for ( QMap<QString, QString>::ConstIterator it = mMap.begin(); it != mMap.end(); it++ )
    {
      if ( config( "OrderByValue" ).toBool() )
        mComboBox->addItem( it.key(), it.value() );
      else
        mComboBox->addItem( it.value(), it.key() );
    }
  }
}

void QgsValueRelationWidget::initCache()
{
  const QgsVectorLayer::ValueRelationData &data = layer()->valueRelation( fieldIdx() );

  mLayer = qobject_cast<QgsVectorLayer*>( QgsMapLayerRegistry::instance()->mapLayer( data.mLayer ) );

  if ( mLayer )
  {
    int ki = mLayer->fieldNameIndex( data.mOrderByValue ? data.mValue : data.mKey );
    int vi = mLayer->fieldNameIndex( data.mOrderByValue ? data.mKey : data.mValue );

    QgsExpression *e = 0;
    if ( !config( "FilterExpression").toString().isEmpty() )
    {
      e = new QgsExpression( config( "FilterExpression").toString() );
      if ( e->hasParserError() || !e->prepare( mLayer->pendingFields() ) )
        ki = -1;
    }

    if ( ki >= 0 && vi >= 0 )
    {
      QSet<int> attributes;
      attributes << ki << vi;

      QgsFeatureRequest::Flags flags = QgsFeatureRequest::NoGeometry;

      if ( e )
      {
        if ( e->needsGeometry() )
          flags |= QgsFeatureRequest::NoGeometry;

        Q_FOREACH ( const QString &field, e->referencedColumns() )
        {
          int idx = mLayer->fieldNameIndex( field );
          if ( idx < 0 )
            continue;
          attributes << idx;
        }
      }

      QgsFeatureIterator fit = mLayer->getFeatures(
                                 QgsFeatureRequest()
                                 .setFlags( flags )
                                 .setSubsetOfAttributes( attributes.toList() )
                               );
      QgsFeature f;
      while ( fit.nextFeature( f ) )
      {
        if ( e && !e->evaluate( &f ).toBool() )
          continue;

        mMap.insert( f.attribute( ki ).toString(), f.attribute( vi ).toString() );
      }
    }
  }
}

void QgsValueRelationWidget::setValue(const QVariant& value)
{
  if ( mListWidget )
  {
    QStringList checkList = value.toString().remove( QChar( '{' ) ).remove( QChar( '}' ) ).split( "," );

    for ( QMap<QString, QString>::ConstIterator it = mMap.begin(); it != mMap.end(); it++ )
    {
      QListWidgetItem *item;
      if ( config( "OrderByValue" ).toBool() )
      {
        item = new QListWidgetItem( it.key() );
        item->setData( Qt::UserRole, it.value() );
        item->setCheckState( checkList.contains( it.value() ) ? Qt::Checked : Qt::Unchecked );
      }
      else
      {
        item = new QListWidgetItem( it.value() );
        item->setData( Qt::UserRole, it.key() );
        item->setCheckState( checkList.contains( it.key() ) ? Qt::Checked : Qt::Unchecked );
      }
      mListWidget->addItem( item );
    }
  }
}
