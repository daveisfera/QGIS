/***************************************************************************
    qgsgenericfeatureselectionmgr.cpp
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

#include "qgsgenericfeatureselectionmgr.h"

QgsGenericFeatureSelectionMgr::QgsGenericFeatureSelectionMgr( QObject *parent )
    : QgsIFeatureSelection( parent )
{
}

QgsGenericFeatureSelectionMgr::QgsGenericFeatureSelectionMgr( const QgsFeatureIds& initialSelection, QObject* parent )
    : QgsIFeatureSelection( parent )
    , mSelectedFeatures( initialSelection )
{
}

int QgsGenericFeatureSelectionMgr::selectedFeatureCount()
{
  return mSelectedFeatures.size();
}

void QgsGenericFeatureSelectionMgr::select( const QgsFeatureIds& ids )
{
  mSelectedFeatures += ids;
  emit selectionChanged( ids, QgsFeatureIds(), false );
}

void QgsGenericFeatureSelectionMgr::deselect( const QgsFeatureIds& ids )
{
  mSelectedFeatures -= ids;
  emit selectionChanged( QgsFeatureIds(), ids, false );
}

void QgsGenericFeatureSelectionMgr::setSelectedFeatures( const QgsFeatureIds& ids )
{
  QgsFeatureIds selected = mSelectedFeatures - ids;
  QgsFeatureIds deselected = ids - mSelectedFeatures;

  mSelectedFeatures = ids;
  emit selectionChanged( selected, deselected, true );
}

const QgsFeatureIds& QgsGenericFeatureSelectionMgr::selectedFeaturesIds() const
{
  return mSelectedFeatures;
}
