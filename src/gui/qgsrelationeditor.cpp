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

#include <QHBoxLayout>
#include <QLabel>

QgsRelationEditorWidget::QgsRelationEditorWidget( QWidget* parent )
    : QWidget( parent )
{
  setupUi( this );
}

QgsRelationEditorWidget* QgsRelationEditorWidget::createRelationEditor( QgsVectorLayer* vl, const QgsRelation& relation, QWidget* parent )
{
  QgsRelationEditorWidget* editor = new QgsRelationEditorWidget( parent );

  QgsDualView* dualView = new QgsDualView( editor );

  editor->mBrowserWidget->layout()->addWidget( dualView );
  return editor;
}
