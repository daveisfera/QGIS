/***************************************************************************
    qgswebviewwidgetfactory.h
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

#ifndef QGSWEBVIEWWIDGETFACTORY_H
#define QGSWEBVIEWWIDGETFACTORY_H

#include "qgseditorwidgetfactory.h"

class QgsWebViewWidgetFactory : public QgsEditorWidgetFactory
{
  public:
    explicit QgsWebViewWidgetFactory( const QString& name );
};

#endif // QGSWEBVIEWWIDGETFACTORY_H
