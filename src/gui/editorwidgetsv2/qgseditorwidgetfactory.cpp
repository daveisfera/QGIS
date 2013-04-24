/***************************************************************************
    qgseditorwidgetfactory.cpp
     --------------------------------------
    Date                 : 21.4.2013
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

#include "qgseditorwidgetfactory.h"
#include "qgseditorwidgetregistry.h"
#include "qgsrelationreferencewidget.h"
#include "qgsrelreferenceconfigdlg.h"

QgsEditorWidgetRegistry*  QgsEditorWidgetRegistry::mInstance = 0;

QgsEditorWidgetRegistry* QgsEditorWidgetRegistry::instance()
{
  if ( !mInstance )
    mInstance = new QgsEditorWidgetRegistry();

  return mInstance;
}

void QgsEditorWidgetRegistry::initKnownTypes()
{
  // The widget for related features with FK on the current feature
  registerWidget<QgsRelationReferenceWidget, QgsRelReferenceConfigDlg >(
        "RelationReference",
        tr( "Relation Reference" ) );
}

QgsEditorWidgetWrapper* QgsEditorWidgetRegistry::create( const QString& widgetType, QObject* parent )
{
  if ( mWidgetFactories.contains( widgetType ) )
  {
    return mWidgetFactories[widgetType]->create( parent );
  }
  return 0;
}

QgsEditorConfigWidget* QgsEditorWidgetRegistry::createConfigWidget( const QString& widgetId, QWidget* parent )
{
  if ( mWidgetFactories.contains( widgetId ) )
  {
    return mWidgetFactories[widgetId]->configWidget( parent );
  }
  return 0;
}
