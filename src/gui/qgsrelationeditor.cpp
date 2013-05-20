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
#include "qgsrelation.h"
#include "qgsdistancearea.h"
#include "qgsexpression.h"
#include "qgsfeature.h"

#include <QHBoxLayout>
#include <QLabel>

QgsRelationEditorWidget::QgsRelationEditorWidget( QWidget* parent )
    : QWidget( parent )
{
  setupUi( this );
}

QgsRelationEditorWidget* QgsRelationEditorWidget::createRelationEditor( const QgsRelation& relation, QgsFeature* feature, QWidget* parent )
{
  QgsRelationEditorWidget* editor = new QgsRelationEditorWidget( parent );

  QgsDualView* dualView = new QgsDualView( editor );

  editor->mBrowserWidget->layout()->addWidget( dualView );

  QStringList conditions;

  foreach ( QgsRelation::FieldPair fieldPair, relation.fieldPairs() )
  {
    conditions << QString( "\"%1\" = '%2'" ).arg( fieldPair.first.name(), feature->attribute( fieldPair.second.name() ).toString() );
  }

  QgsFeatureRequest myRequest;

  QgsDebugMsg( QString( "Filter conditions: '%1'" ).arg( conditions.join( " AND " ) ) );

  myRequest.setFilterExpression( conditions.join( " AND " ) );

  // TODO: Proper QgsDistanceArea, proper mapcanvas
  dualView->init( relation.referencingLayer(), NULL, QgsDistanceArea(), myRequest );

  return editor;
}
