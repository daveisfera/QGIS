/***************************************************************************
    qgsvaluerelationwidgetfactory.h
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

#ifndef QGSVALUERELATIONWIDGETFACTORY_H
#define QGSVALUERELATIONWIDGETFACTORY_H

#include "qgseditorwidgetfactory.h"

class QgsValueRelationWidgetFactory : public QgsEditorWidgetFactory
{
  public:
    explicit QgsValueRelationWidgetFactory( const QString& name );
};

#endif // QGSVALUERELATIONWIDGETFACTORY_H
