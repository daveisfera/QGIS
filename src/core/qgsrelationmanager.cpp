/***************************************************************************
    qgsrelationmanager.cpp
     --------------------------------------
    Date                 : 1.3.2013
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

#include <QDebug>

#include "qgsexception.h"
#include "qgslogger.h"
#include "qgsproject.h"
#include "qgsrelationmanager.h"
#include "qgsmaplayerregistry.h"
#include "qgsvectorlayer.h"

QgsRelationManager::QgsRelationManager(QObject *parent) :
  QObject(parent)
{
  QgsProject* project = QgsProject::instance();
  connect( project, SIGNAL( readProject( const QDomDocument& ) ), SLOT ( readProject( const QDomDocument& ) ) );
  connect( project, SIGNAL( writeProject( QDomDocument& ) ), SLOT ( writeProject( QDomDocument& ) ) );
}

void QgsRelationManager::setRelations( const QList<QgsRelation> &relations )
{
  mRelations = relations;
}

const QList<QgsRelation>& QgsRelationManager::relations()
{
  return mRelations;
}

void QgsRelationManager::readProject( const QDomDocument & doc )
{
  mRelations.clear();

  QDomNodeList nodes = doc.elementsByTagName( "relations" );
  if ( nodes.count() )
  {
    QDomNode node = nodes.item( 0 );
    QDomNodeList relationNodes = node.childNodes();
    int relCount = relationNodes.count();
    for ( int i = 0; i < relCount; ++i )
    {
      try
      {
        mRelations.append( QgsRelation::createFromXML( relationNodes.at(i) ) );
      }
      catch ( QgsException e )
      {
        qDebug() << e.what();
      }
    }
  }
  else
  {
    QgsDebugMsg( "No relations data present in this document" );
  }
}

void QgsRelationManager::writeProject( QDomDocument & doc )
{
  QDomNodeList nl = doc.elementsByTagName( "qgis" );
  if ( !nl.count() )
  {
    QgsDebugMsg( "Unable to find qgis element in project file" );
    return;
  }
  QDomNode qgisNode = nl.item( 0 );  // there should only be one

  QDomElement relationsNode = doc.createElement( "relations" );
  qgisNode.appendChild( relationsNode );

  foreach ( const QgsRelation& relation, mRelations )
  {
    relation.writeXML( relationsNode, doc );
  }
}

QgsRelation QgsRelation::createFromXML( const QDomNode &node )
{
  QDomElement elem = node.toElement();

  if ( elem.tagName() != "relation" )
  {
    throw QgsException( QString( "Cannot create relation. Unexpected tag '%1'" ).arg( elem.tagName() ) );
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
    throw QgsException( QString( "Relation defined for layer '%1' which does not exist." ).arg( referencingLayerId ) );
  }

  if ( NULL == referencedLayer )
  {
    throw QgsException( QString( "Relation defined for layer '%1' which does not exist." ).arg( referencedLayerId ) );
  }

  if ( QgsMapLayer::VectorLayer  != referencingLayer->type() )
  {
    throw QgsException( QString( "Relation defined for layer '%1' which is not of type VectorLayer." ).arg( referencingLayerId ) );
  }

  if ( QgsMapLayer::VectorLayer  != referencedLayer->type() )
  {
    throw QgsException( QString( "Relation defined for layer '%1' which is not of type VectorLayer." ).arg( referencedLayerId ) );
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

QString QgsRelation::relationName() const
{
  return mRelationName;
}

QString QgsRelation::referencingLayerId() const
{
  return mReferencingLayerId;
}

QString QgsRelation::referencedLayerId() const
{
  return mReferencedLayerId;
}

QList<QgsRelation::FieldPair> QgsRelation::fieldPairs() const
{
  return mFieldPairs;
}

