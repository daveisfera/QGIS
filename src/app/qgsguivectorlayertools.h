/***************************************************************************
    qgsfeaturefactory.h
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

#ifndef QGSGUIVECTORLAYERTOOLS_H
#define QGSGUIVECTORLAYERTOOLS_H

#include "qgsvectorlayertools.h"

class QgsGuiVectorLayerTools : public QgsVectorLayerTools, public QObject
{
  public:
    QgsGuiVectorLayerTools();

    static QgsGuiVectorLayerTools* instance();

    bool addFeature( QgsVectorLayer *layer, QgsAttributeMap defaultValues, const QgsGeometry &defaultGeometry );
};

#endif // QGSGUIVECTORLAYERTOOLS_H
