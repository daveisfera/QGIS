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

#include "qgsrelationmanager.h"

#include "qgsapplication.h"
#include "qgslogger.h"
#include "qgsproject.h"

QgsRelationManager* QgsRelationManager::mInstance = NULL;

QgsRelationManager::QgsRelationManager() :
  QObject( QgsApplication::instance() )
{
  QgsProject* project = QgsProject::instance();
  connect( project, SIGNAL( readProject( const QDomDocument& ) ), SLOT ( readProject( const QDomDocument& ) ) );
  connect( project, SIGNAL( writeProject( QDomDocument& ) ), SLOT ( writeProject( QDomDocument& ) ) );
}

QgsRelationManager* QgsRelationManager::instance()
{
  if ( !mInstance )
    mInstance = new QgsRelationManager();

  return mInstance;
}

void QgsRelationManager::setRelations( const QList<QgsRelation> &relations )
{
  mRelations = relations;
}

const QList<QgsRelation>& QgsRelationManager::relations()
{
  return mRelations;
}

QList<QgsRelation> QgsRelationManager::referencingRelations( QgsVectorLayer* layer, int fieldIdx )
{
  QList<QgsRelation> relations;

  foreach ( const QgsRelation& rel, mRelations )
  {
    if ( rel.referencedLayerId() )
  }
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
      mRelations.append( QgsRelation::createFromXML( relationNodes.at(i) ) );
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
