/***************************************************************************
    qgscalendarconfigdlg.cpp
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

#include "qgscalendarconfigdlg.h"

QgsCalendarConfigDlg::QgsCalendarConfigDlg(QgsVectorLayer* vl, int fieldIdx, QWidget *parent )
  : QgsEditorConfigWidget( vl, fieldIdx, parent )
{
  setupUi( this );
}


QgsEditorWidgetConfig QgsCalendarConfigDlg::config()
{
  QgsEditorWidgetConfig cfg;

  cfg.insert( "DateFormat", leDateFormat->text() );

  return cfg;
}

void QgsCalendarConfigDlg::setConfig(const QgsEditorWidgetConfig& config)
{
  leDateFormat->setText( config.value( "DateFormat", "" ).toString() );
}
