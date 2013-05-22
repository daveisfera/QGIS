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

#include "ui_qgsrelationeditorwidgetbase.h"

class QgsDualView;
class QgsVectorLayer;
class QgsRelation;
class QgsFeature;

class QgsRelationEditorWidget : public QgsCollapsibleGroupBox, private Ui::QgsRelationEditorWidgetBase
{
    Q_OBJECT

  public:
    QgsRelationEditorWidget( QWidget* parent = NULL );

    static QgsRelationEditorWidget* createRelationEditor( const QgsRelation& relation, QgsFeature* feature, QWidget* parent = NULL );

  private slots:
    void onCollapsedStateChanged( bool state );

  private:
    QgsDualView* mDualView;
};

#endif // QGSRELATIONEDITOR_H
