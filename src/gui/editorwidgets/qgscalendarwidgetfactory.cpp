/***************************************************************************
    qgscalendarwidgetfactory.cpp
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

#include "qgscalendarwidgetfactory.h"

#include "qgscalendarwidget.h"
#include "qgscalendarconfigdlg.h"

QgsCalendarWidgetFactory::QgsCalendarWidgetFactory( QString name )
    : QgsEditorWidgetFactory( name )
{
}

QgsEditorWidgetWrapper* QgsCalendarWidgetFactory::create(QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent) const
{
  return new QgsCalendarWidget( vl, fieldIdx, editor, parent );
}

QgsEditorConfigWidget* QgsCalendarWidgetFactory::configWidget(QgsVectorLayer* vl, int fieldIdx, QWidget* parent) const
{
  return new QgsCalendarConfigDlg( vl, fieldIdx, parent );
}

QgsEditorWidgetConfig QgsCalendarWidgetFactory::readConfig(const QDomElement& configElement, QgsVectorLayer* layer, int fieldIdx)
{
  QgsEditorWidgetConfig cfg;
  cfg.insert( "DateFormat", configElement.attribute( "DateFormat", "" ) );
  return cfg;
}

void QgsCalendarWidgetFactory::writeConfig(const QgsEditorWidgetConfig& config, QDomElement& configElement, QDomDocument& doc, const QgsVectorLayer* layer, int fieldIdx)
{
  configElement.setAttribute( "DateFormat", config.value( "DateFormat", "" ).toString() );
}
