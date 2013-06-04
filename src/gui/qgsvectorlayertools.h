/***************************************************************************
    QgsAbstractFeatureAction.h
     --------------------------------------
    Date                 : 29.5.2013
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

#ifndef QGSVECTORLAYERTOOLS_H
#define QGSVECTORLAYERTOOLS_H

#include "qgsfeature.h"
#include "qgsgeometry.h"

class QgsVectorLayer;

class QgsVectorLayerTools
{
  public:
    virtual bool addFeature( QgsVectorLayer* layer, QgsAttributeMap defaultValues = QgsAttributeMap(), const QgsGeometry& defaultGeometry = QgsGeometry() ) = 0;
};

#endif // QGSVECTORLAYERTOOLS_H
