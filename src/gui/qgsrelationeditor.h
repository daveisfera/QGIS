/***************************************************************************
    qgsrelationeditor.h
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

#ifndef QGSRELATIONEDITOR_H
#define QGSRELATIONEDITOR_H

#include <QWidget>

class QgsVectorLayer;
class QgsRelation;

class QgsRelationEditor : public QWidget
{
  public:
    QgsRelationEditor( QWidget* parent = NULL );

    static QgsRelationEditor* createRelationEditor( QgsVectorLayer* vl, const QgsRelation &relation, QWidget* parent = NULL );
};

#endif // QGSRELATIONEDITOR_H
