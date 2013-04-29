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

#include "qgsfield.h"

class QgsVectorLayer;

class CORE_EXPORT QgsRelation
{
  public:
    typedef QPair< QgsField, QgsField > FieldPair;

    QgsRelation() {}

    static QgsRelation createFromXML( const QDomNode& node );
    void writeXML( QDomNode& node, QDomDocument& doc ) const;

    void setRelationName( QString name );
    void setReferencingLayer( QString id );
    void setReferencedLayer( QString id );
    void addFieldPair( QString referencingField, QString referencedField );
    void addFieldPair( FieldPair fieldPair );

    QString relationName() const;
    QString referencingLayerId() const;
    QString referencedLayerId() const;
    QList< FieldPair > fieldPairs() const;

  private:
    QString mRelationName;
    QString mReferencingLayerId;
    /** The child layer */
    QgsVectorLayer* mReferencingLayer;
    QString mReferencedLayerId;
    /** The parent layer */
    QgsVectorLayer* mReferencedLayer;
    /** A list of fields which define the relation.
     *  In most cases there will be only one value, but multiple values
     *  are supported for composited foreign keys.
     *  The first field is on the referencing layer, the second on the referenced */
    QList< FieldPair > mFieldPairs;
};

/**
 * This class manages a set of relations between layers.
 *
 * @note This class is a singleton. Call QgsRelationManager::instance() to get access to the instance.
 */
class CORE_EXPORT QgsRelationManager : public QObject
{
    Q_OBJECT

  private:
    explicit QgsRelationManager();

  public:
    static QgsRelationManager* instance();
    void setRelations( const QList<QgsRelation>& relations );
    const QList<QgsRelation>& relations();

  signals:

  public slots:

  private slots:
    void readProject( const QDomDocument &doc );
    void writeProject( QDomDocument &doc );
    
  private:
    /** The references */
    QList<QgsRelation> mRelations;
    static QgsRelationManager* mInstance;
};

#endif // QGSRELATIONMANAGER_H
