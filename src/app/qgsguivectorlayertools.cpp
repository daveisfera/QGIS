/***************************************************************************
    qgsfeaturefactory.cpp
     --------------------------------------
    Date                 : 30.5.2013
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

#include "qgsguivectorlayertools.h"
#include "qgsfeatureaction.h"

QgsGuiVectorLayerTools* QgsGuiVectorLayerTools::sInstance = new QgsGuiVectorLayerTools();

QgsGuiVectorLayerTools*QgsGuiVectorLayerTools::instance()
{
  return sInstance;
}

bool QgsGuiVectorLayerTools::addFeature( QgsVectorLayer* layer, QgsAttributeMap defaultValues, const QgsGeometry& defaultGeometry )
{
  QgsFeature f;
  f.setGeometry( defaultGeometry );
  QgsFeatureAction a( tr( "Add feature" ), f, layer );
  return a.addFeature( defaultValues );
}

QgsGuiVectorLayerTools::QgsGuiVectorLayerTools()
    : QObject( NULL )
{
}
