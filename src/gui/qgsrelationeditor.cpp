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

#include "qgsrelation.h"

#include <QHBoxLayout>
#include <QLabel>

QgsRelationEditor::QgsRelationEditor( QWidget* parent )
    : QWidget( parent )
{
}

QgsRelationEditor* QgsRelationEditor::createRelationEditor( QgsVectorLayer* vl, const QgsRelation& relation, QWidget* parent )
{
  QgsRelationEditor* ed = new QgsRelationEditor( parent );
  ed->setLayout( new QHBoxLayout( ed ) );
  ed->layout()->addWidget( new QLabel( relation.name(), ed ) );
  return ed;
}
