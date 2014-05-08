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

// Alias as we can't use special characters in Q_FOREACH macro
typedef QPair < QVariant, QString > QVariantStringPair;

bool orderByKeyLessThan( const QVariantStringPair& p1, const QVariantStringPair& p2 )
{
  switch( p1.first.type() )
  {
    case QVariant::String:
      return p1.first.toString() < p2.first.toString();
      break;

    case QVariant::Double:
      return p1.first.toDouble() < p2.first.toDouble();
      break;

    default:
      return p1.first.toInt() < p2.first.toInt();
      break;
  }
}

bool orderByValueLessThan( const QVariantStringPair& p1, const QVariantStringPair& p2 )
{
  return p1.second < p2.second;
}

QgsValueRelationWidget::QgsValueRelationWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent )
    :  QgsEditorWidgetWrapper( vl, fieldIdx, editor, parent )
{
}


QVariant QgsValueRelationWidget::value()
{
  QVariant v;

  if ( mComboBox )
    v = mComboBox->itemData( mComboBox->currentIndex() );

  if ( mListWidget )
  {
    QStringList selection;
    for ( int i = 0; i < mListWidget->count(); ++i )
    {
      QListWidgetItem* item = mListWidget->item( i );
      if ( item->checkState() == Qt::Checked )
        selection << item->data( Qt::UserRole ).toString();
    }

    v = selection.join( "," ).prepend( "{" ).append( "}" );
  }

  return v;
}

QWidget* QgsValueRelationWidget::createWidget( QWidget* parent )
{
  if ( config( "AllowMulti" ).toBool() )
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

    Q_FOREACH( const QVariantStringPair& element, mMap )
    {
      mComboBox->addItem( element.second, element.first );
    }

    connect( mComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( valueChanged() ) );
  }
  else if ( mListWidget )
  {
    Q_FOREACH( const QVariantStringPair& element, mMap )
    {
      QListWidgetItem *item;
      item = new QListWidgetItem( element.second );
      item->setData( Qt::UserRole, element.first );

      mListWidget->addItem( item );
    }
    connect( mListWidget, SIGNAL( itemChanged( QListWidgetItem* ) ), this, SLOT( valueChanged() ) );
  }
}

void QgsValueRelationWidget::initCache()
{
  mLayer = qobject_cast<QgsVectorLayer*>( QgsMapLayerRegistry::instance()->mapLayer( config( "Layer" ).toString() ) );

  if ( mLayer )
  {
    int ki = mLayer->fieldNameIndex( config( "Key" ).toString() );
    int vi = mLayer->fieldNameIndex( config( "Value" ).toString() );

    QgsExpression *e = 0;
    if ( !config( "FilterExpression" ).toString().isEmpty() )
    {
      e = new QgsExpression( config( "FilterExpression" ).toString() );
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

        Q_FOREACH( const QString& field, e->referencedColumns() )
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

        mMap.append( QPair<QVariant, QString>( f.attribute( ki ), f.attribute( vi ).toString() ) );
      }

      if ( config( "OrderByValue" ).toBool() )
        qSort( mMap.begin(), mMap.end(), orderByValueLessThan );
      else
        qSort( mMap.begin(), mMap.end(), orderByKeyLessThan );
    }
  }
}

void QgsValueRelationWidget::setValue( const QVariant& value )
{
  if ( mListWidget )
  {
    QStringList checkList = value.toString().remove( QChar( '{' ) ).remove( QChar( '}' ) ).split( "," );

    for ( int i = 0; i < mListWidget->count(); ++i )
    {
      QListWidgetItem* item = mListWidget->item( i );
      if ( config( "OrderByValue" ).toBool() )
      {
        item->setCheckState( checkList.contains( item->data( Qt::UserRole ).toString() ) ? Qt::Checked : Qt::Unchecked );
      }
      else
      {
        item->setCheckState( checkList.contains( item->data( Qt::UserRole ).toString() ) ? Qt::Checked : Qt::Unchecked );
      }
    }
  }
  else if ( mComboBox )
  {
    mComboBox->setCurrentIndex( mComboBox->findData( value ) );
  }
}

