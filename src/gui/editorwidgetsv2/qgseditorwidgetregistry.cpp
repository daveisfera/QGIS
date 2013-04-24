/***************************************************************************
    qgseditorwidgetregistry.cpp
     --------------------------------------
    Date                 : 24.4.2013
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

#include "qgseditorwidgetregistry.h"

const QMap<QString, QgsEditorWidgetFactory*> QgsEditorWidgetRegistry::factories()
{
  return mWidgetFactories;
}

void QgsEditorWidgetRegistry::registerWidget(const QString& widgetType, QgsEditorWidgetFactory* widgetFactory)
{
  mWidgetFactories.insert( widgetType, widgetFactory );
}

QgsEditorWidgetRegistry::QgsEditorWidgetRegistry()
{
  initKnownTypes();
}

QgsEditorWidgetRegistry::~QgsEditorWidgetRegistry()
{
  qDeleteAll( mWidgetFactories.values() );
}
