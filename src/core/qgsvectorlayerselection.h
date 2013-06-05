/***************************************************************************
    qgsvectorlayerselection.h
     --------------------------------------
    Date                 : 6.6.2013
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

#ifndef QGSVECTORLAYERSELECTION_H
#define QGSVECTORLAYERSELECTION_H

#include "qgsifeatureselection.h"

class QgsVectorLayerSelection : public QgsIFeatureSelection
{
    Q_OBJECT

  public:
    QgsVectorLayerSelection();
};

#endif // QGSVECTORLAYERSELECTION_H
