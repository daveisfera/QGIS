/***************************************************************************
    qgsfeatureselectiondlg.cpp
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

#include "qgsfeatureselectiondlg.h"

#include "qgsgenericfeatureselectionmgr.h"

QgsFeatureSelectionDlg::QgsFeatureSelectionDlg( QgsVectorLayer* vl, QWidget *parent )
    : QDialog( parent )
    , mVectorLayer( vl )
{
  setupUi( this );

  mFeatureSelection = new QgsGenericFeatureSelectionMgr( mDualView );

  mDualView->setFeatureSelectionManager( mFeatureSelection );
}

const QgsFeatureIds& QgsFeatureSelectionDlg::selectedFeatures()
{
  return mFeatureSelection->selectedFeaturesIds();
}


