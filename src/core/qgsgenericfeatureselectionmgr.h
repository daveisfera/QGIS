/***************************************************************************
    qgsgenericfeatureselectionmgr.h
     --------------------------------------
    Date                 : 11.6.2013
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

#ifndef QGSGENERICFEATURESELECTIONMGR_H
#define QGSGENERICFEATURESELECTIONMGR_H

#include "qgsfeature.h"
#include "qgsifeatureselection.h"

class QgsGenericFeatureSelectionMgr : public QgsIFeatureSelection
{
    Q_OBJECT

  public:
    explicit QgsGenericFeatureSelectionMgr( QObject *parent = NULL );
    QgsGenericFeatureSelectionMgr( const QgsFeatureIds& initialSelection, QObject *parent = NULL );

    // QgsIFeatureSelection interface
    virtual int selectedFeatureCount();
    virtual void select( const QgsFeatureIds& ids );
    virtual void deselect( const QgsFeatureIds& ids );
    virtual void setSelectedFeatures( const QgsFeatureIds& ids );
    virtual const QgsFeatureIds& selectedFeaturesIds() const;

  private:
    QgsFeatureIds mSelectedFeatures;
};

#endif // QGSGENERICFEATURESELECTIONMGR_H
