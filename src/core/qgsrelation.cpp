/***************************************************************************
    qgsrelation.cpp
     --------------------------------------
    Date                 : 29.4.2013
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

#include "qgsrelation.h"

#include "qgsapplication.h"
#include "qgslogger.h"
#include "qgsmaplayerregistry.h"
#include "qgsvectorlayer.h"

QgsRelation::QgsRelation()
  : mReferencingLayer( NULL )
  , mReferencedLayer( NULL )
  , mValid( false )
{
}

QgsRelation QgsRelation::createFromXML( const QDomNode &node )
{
  QDomElement elem = node.toElement();

  if ( elem.tagName() != "relation" )
  {
    QgsLogger::warning( QApplication::translate( "QgsRelation", "Cannot create relation. Unexpected tag '%1'" ).arg( elem.tagName() ) );
  }

  QgsRelation relation;

  QString referencingLayerId = elem.attribute( "referencingLayer" );
  QString referencedLayerId = elem.attribute( "referencedLayer" );
  QString name = elem.attribute( "name" );

  QMap<QString, QgsMapLayer*> mapLayers = QgsMapLayerRegistry::instance()->mapLayers();

  QgsMapLayer* referencingLayer = mapLayers[referencingLayerId];
  QgsMapLayer* referencedLayer = mapLayers[referencedLayerId];;

  if ( NULL == referencingLayer )
  {
    QgsLogger::warning( QApplication::translate( "QgsRelation", "Relation defined for layer '%1' which does not exist." ).arg( referencingLayerId ) );
  }

  if ( NULL == referencedLayer )
  {
    QgsLogger::warning( QApplication::translate( "QgsRelation", "Relation defined for layer '%1' which does not exist." ).arg( referencedLayerId ) );
  }

  if ( QgsMapLayer::VectorLayer  != referencingLayer->type() )
  {
    QgsLogger::warning( QApplication::translate( "QgsRelation", "Relation defined for layer '%1' which is not of type VectorLayer." ).arg( referencingLayerId ) );
  }

  if ( QgsMapLayer::VectorLayer  != referencedLayer->type() )
  {
    QgsLogger::warning( QApplication::translate( "QgsRelation", "Relation defined for layer '%1' which is not of type VectorLayer." ).arg( referencedLayerId ) );
  }

  relation.mReferencingLayerId = referencingLayerId;
  relation.mReferencingLayer = qobject_cast<QgsVectorLayer*>( referencingLayer );
  relation.mReferencedLayerId = referencedLayerId;
  relation.mReferencedLayer = qobject_cast<QgsVectorLayer*>( referencedLayer );
  relation.mRelationName = name;

  QDomNodeList references = elem.elementsByTagName( "fieldRef" );
  for ( int i = 0; i < references.size(); ++i )
  {
    QDomElement refEl = references.at( i ).toElement();

    QString referencingField = refEl.attribute( "referencingField" );
    QString referencedField = refEl.attribute( "referencedField" );

    relation.addFieldPair( referencingField, referencedField );
  }

  relation.mValid = true;

  return relation;
}

void QgsRelation::writeXML( QDomNode &node, QDomDocument &doc ) const
{
  QDomElement elem = doc.createElement( "relation" );
  elem.setAttribute( "name", mRelationName );
  elem.setAttribute( "referencingLayer", mReferencingLayerId );
  elem.setAttribute( "referencedLayer", mReferencedLayerId );

  foreach( FieldPair fields, mFieldPairs )
  {
    QDomElement referenceElem = doc.createElement( "fieldRef" );
    referenceElem.setAttribute( "referencingField", fields.first.name() );
    referenceElem.setAttribute( "referencedField", fields.second.name() );
    elem.appendChild( referenceElem );
  }

  node.appendChild( elem );
}

void QgsRelation::setRelationName( QString name )
{
  mRelationName = name;
}

void QgsRelation::setReferencingLayer( QString id )
{
  mReferencingLayerId = id;
}

void QgsRelation::setReferencedLayer( QString id )
{
  mReferencedLayerId = id;
}

void QgsRelation::addFieldPair(QString referencingField, QString referencedField)
{
  mFieldPairs << FieldPair( referencingField, referencedField );
}

void QgsRelation::addFieldPair( QgsRelation::FieldPair fieldPair )
{
  mFieldPairs << fieldPair;
}

QString QgsRelation::name() const
{
  return mRelationName;
}

QString QgsRelation::referencingLayerId() const
{
  return mReferencingLayerId;
}

QgsVectorLayer* QgsRelation::referencingLayer() const
{
  return mReferencingLayer;
}

QString QgsRelation::referencedLayerId() const
{
  return mReferencedLayerId;
}

QgsVectorLayer* QgsRelation::referencedLayer() const
{
  return mReferencedLayer;
}

QList<QgsRelation::FieldPair> QgsRelation::fieldPairs() const
{
  return mFieldPairs;
}

bool QgsRelation::isValid() const
{
  return mValid;
}

