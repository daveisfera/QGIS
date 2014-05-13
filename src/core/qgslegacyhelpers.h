/***************************************************************************
    qgslegacyhelpers.h
     --------------------------------------
    Date                 : 13.5.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias dot kuhn at gmx dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSLEGACYHELPERS_H
#define QGSLEGACYHELPERS_H

#include <QString>

#include "qgsvectorlayer.h"

class QgsLegacyHelpers
{
  public:
    static const QString convertEditType(QgsVectorLayer::EditType editType, QgsEditorWidgetConfig& cfg, QgsVectorLayer* vl, const QString& name, const QDomElement editTypeElement = QDomElement() );

  private:
    QgsLegacyHelpers()
      {}
};

#endif // QGSLEGACYHELPERS_H
