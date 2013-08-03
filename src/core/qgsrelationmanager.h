/***************************************************************************
    qgsrelationmanager.h
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

#ifndef QGSRELATIONMANAGER_H
#define QGSRELATIONMANAGER_H

#include <QObject>
#include <QPair>
#include <QDomNode>
#include <QDomDocument>

#include "qgsrelation.h"

class QgsVectorLayer;

/**
 * This class manages a set of relations between layers.
 */
class CORE_EXPORT QgsRelationManager : public QObject
{
    Q_OBJECT

  public:
    explicit QgsRelationManager();

    void setRelations( const QList<QgsRelation>& relations );
    const QMap<QString, QgsRelation>& relations();
    void addRelation( const QgsRelation& relation );
    QgsRelation relation( const QString& id );

    QList<QgsRelation> referencingRelations( QgsVectorLayer* layer = NULL, int fieldIdx = -2 );
    QList<QgsRelation> referencedRelations( QgsVectorLayer* layer = NULL );

  signals:
    void relationsLoaded();

  public slots:

  private slots:
    void readProject( const QDomDocument &doc );
    void writeProject( QDomDocument &doc );

  private:
    /** The references */
    QMap<QString, QgsRelation> mRelations;
};

#endif // QGSRELATIONMANAGER_H
