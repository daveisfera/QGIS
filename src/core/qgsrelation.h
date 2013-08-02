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
#include "qgsfeatureiterator.h"

class QgsVectorLayer;

class CORE_EXPORT QgsRelation
{
  public:
    class FieldPair : public QPair< QString, QString >
    {
      public:
        // Default constructor: Empty strings
        FieldPair()
            : QPair< QString, QString >() {}

        // Constructor which takes two fields
        FieldPair( QString referencingField, QString referencedField )
            : QPair< QString, QString >( referencingField, referencedField ) {}

        // Aliases
        QString referencingField() const { return first; }
        QString referencedField() const { return second; }
    };

    QgsRelation();

    static QgsRelation createFromXML( const QDomNode& node );
    void writeXML( QDomNode& node, QDomDocument& doc ) const;

    void setRelationName( QString name );
    void setReferencingLayer( QString id );
    void setReferencedLayer( QString id );
    void addFieldPair( QString referencingField, QString referencedField );
    void addFieldPair( FieldPair fieldPair );

    /**
     * Creates an iterator which returns all the features on the referencing (child) layer
     * which have a foreign key pointing to the provided feature.
     *
     * @param feat A feature from the referenced (parent) layer
     *
     * @return An iterator with all the referenced features
     */
    QgsFeatureIterator getRelatedFeatures( const QgsFeature& feature ) const;

    /**
     * Creates a request to return all the features on the referencing (child) layer
     * which have a foreign key pointing to the provided feature.
     *
     * @param feat A feature from the referenced (parent) layer
     *
     * @return An request for all the referenced features
     */
    QgsFeatureRequest getRelatedFeaturesRequest( const QgsFeature& feature ) const;

    QString name() const;
    QString referencingLayerId() const;
    QgsVectorLayer* referencingLayer() const;
    QString referencedLayerId() const;
    QgsVectorLayer* referencedLayer() const;
    QList< FieldPair > fieldPairs() const;

    bool isValid() const;

  protected:
    void updateRelationStatus();
    void runChecks();

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

    bool mValid;
};

#endif // QGSRELATION_H
