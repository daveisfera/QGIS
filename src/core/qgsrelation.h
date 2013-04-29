/***************************************************************************
    qgsrelation.h
     --------------------------------------
    Date                 : 29.4.2013
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

#ifndef QGSRELATION_H
#define QGSRELATION_H

#include <QList>
#include <QDomNode>
#include <QPair>

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
    QgsVectorLayer* referencingLayer() const;
    QString referencedLayerId() const;
    QgsVectorLayer* referencedLayer() const;
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

#endif // QGSRELATION_H
