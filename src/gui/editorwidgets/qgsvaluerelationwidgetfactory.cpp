/***************************************************************************
    qgsvaluerelationwidgetfactory.cpp
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

#include "qgsvaluerelationwidgetfactory.h"

#include "qgsvaluerelationwidget.h"
#include "qgsvaluerelationconfigdlg.h"

QgsValueRelationWidgetFactory::QgsValueRelationWidgetFactory( const QString& name )
    :  QgsEditorWidgetFactory( name )
{
}

QgsEditorWidgetWrapper* QgsValueRelationWidgetFactory::create(QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent) const
{
  return new QgsValueRelationWidget( vl, fieldIdx, editor, parent );
}

QgsEditorConfigWidget* QgsValueRelationWidgetFactory::configWidget(QgsVectorLayer* vl, int fieldIdx, QWidget* parent) const
{
  return new QgsValueRelationConfigDlg( vl, fieldIdx, parent );
}

QgsEditorWidgetConfig QgsValueRelationWidgetFactory::readConfig(const QDomElement& configElement, QgsVectorLayer* layer, int fieldIdx)
{
  QgsEditorWidgetConfig cfg;

  cfg.insert( "Layer", configElement.attribute( "Layer" ) );
  cfg.insert( "Key", configElement.attribute( "Key" ) );
  cfg.insert( "Value", configElement.attribute( "Value" ) );
  cfg.insert( "FilterExpression", configElement.attribute( "FilterExpression" ) );
  cfg.insert( "OrderByValue", configElement.attribute( "OrderByValue" ) );
  cfg.insert( "AllowMulti", configElement.attribute( "AllowMulti" ) );

  return cfg;
}

void QgsValueRelationWidgetFactory::writeConfig(const QgsEditorWidgetConfig& config, QDomElement& configElement, QDomDocument& doc, const QgsVectorLayer* layer, int fieldIdx)
{
  configElement.setAttribute( "Layer", config.value( "Layer" ).toString() );
  configElement.setAttribute( "Key", config.value( "Key" ).toString() );
  configElement.setAttribute( "Value", config.value( "Value" ).toString() );
  configElement.setAttribute( "FilterExpression", config.value( "FilterExpression" ).toString() );
  configElement.setAttribute( "OrderByValue", config.value( "OrderByValue" ).toBool() );
  configElement.setAttribute( "AllowMulti", config.value( "AllowMulti" ).toBool() );
}
