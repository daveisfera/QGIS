/***************************************************************************
    qgsrelationreferencewidget.cpp
     --------------------------------------
    Date                 : 20.4.2013
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

#include <QPushButton>

#include "qgsrelationreferencewidget.h"
#include "qgsrelreferenceconfigdlg.h"
#include "qgseditorwidgetfactory.h"

QgsRelationReferenceWidget::QgsRelationReferenceWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* parent ) :
  QgsEditorWidgetWrapper( vl, fieldIdx, parent )
{
}

QWidget* QgsRelationReferenceWidget::createWidget( QWidget* parent )
{
  return new QPushButton( config( "A" ).toString(), parent );
}

const QVariant& QgsRelationReferenceWidget::value()
{

}

void QgsRelationReferenceWidget::setValue( const QVariant& value )
{

}

template <>
QMap<QString, QVariant> QgsEditWidgetFactoryHelper<QgsRelationReferenceWidget, QgsRelReferenceConfigDlg>::readConfig( const QDomElement& configElement, QgsVectorLayer* layer, int fieldIdx )
{
  QMap<QString, QVariant> cfg;

  cfg.insert( "AllowNULL", configElement.attribute( "AllowNULL" ) == "1" );
  cfg.insert( "ShowForm", configElement.attribute( "ShowForm" ) == "1" );
  cfg.insert( "DisplayField", configElement.attribute( "DisplayField" ) );

  return cfg;
}

template <>
void QgsEditWidgetFactoryHelper<QgsRelationReferenceWidget, QgsRelReferenceConfigDlg>::writeConfig( QMap<QString, QVariant>config, QDomElement& configElement, const QDomDocument& doc, const QgsVectorLayer* layer, int fieldIdx )
{
  configElement.setAttribute( "AllowNULL", config["AllowNULL"].toBool() );
  configElement.setAttribute( "ShowForm", config["ShowForm"].toBool() );
  configElement.setAttribute( "DisplayField", config["DisplayField"].toString() );
}
