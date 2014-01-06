/***************************************************************************
    qgslineeditwidgetfactory.h
     --------------------------------------
    Date                 : 5.1.2014
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

#ifndef QGSLINEEDITWIDGETFACTORY_H
#define QGSLINEEDITWIDGETFACTORY_H

#include "qgseditorwidgetfactory.h"

class QgsLineEditWidgetFactory : public QgsEditorWidgetFactory
{
  public:
    QgsLineEditWidgetFactory( const QString& name );
};

#endif // QGSLINEEDITWIDGETFACTORY_H
