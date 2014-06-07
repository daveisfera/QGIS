/***************************************************************************
    qgsmaptoolidentifyfeature.h
     --------------------------------------
    Date                 : 22.5.2014
    Copyright            : (C) 2014 Denis Rouzaud
    Email                : denis.rouzaud@gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSMAPTOOLIDENTIFYFEATURE_H
#define QGSMAPTOOLIDENTIFYFEATURE_H

#include "qgsmaptoolidentify.h"

class GUI_EXPORT QgsMapToolIdentifyFeature : public QgsMapToolIdentify
{
    Q_OBJECT

  public:
    QgsMapToolIdentifyFeature( QgsVectorLayer* vl, QgsMapCanvas* canvas );

    void canvasReleaseEvent( QMouseEvent* e );

    void deactivate();

  signals:
    void featureIdentified( const QgsFeature& );
    void featureIdentified( QgsFeatureId );
    void deactivated();

  private:
    QgsVectorLayer* mLayer;
    QgsMapCanvas* mCanvas;
};

#endif // QGSMAPTOOLIDENTIFYFEATURE_H
