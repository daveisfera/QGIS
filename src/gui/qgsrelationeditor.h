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

#include "qgsrelation.h"

class QgsDualView;
class QgsFeature;
class QgsGenericFeatureSelectionMgr;
class QgsVectorLayer;
class QgsVectorLayerTools;


class QgsRelationEditorWidget : public QgsCollapsibleGroupBox, private Ui::QgsRelationEditorWidgetBase
{
    Q_OBJECT

  public:
    QgsRelationEditorWidget( QgsVectorLayerTools* featureAction, const QgsRelation& relation, QgsFeature* feature, QWidget* parent = NULL );

    static QgsRelationEditorWidget* createRelationEditor( const QgsRelation& relation, QgsFeature* feature, QgsVectorLayerTools* featureAction, QWidget* parent = NULL );

  private slots:
    void onCollapsedStateChanged( bool state );
    void referencingLayerEditingToggled();

    void on_mPbnNew_clicked();
    void on_mPbnLink_clicked();
    void on_mPbnDelete_clicked();
    void on_mPbnUnlink_clicked();
    void on_mToggleEditingButton_clicked();

  private:
    QgsDualView* mDualView;
    QgsGenericFeatureSelectionMgr* mFeatureSelectionMgr;
    QgsVectorLayerTools* mVlTools;
    QgsRelation mRelation;
    QgsFeature* mFeature;

};

#endif // QGSRELATIONEDITOR_H
