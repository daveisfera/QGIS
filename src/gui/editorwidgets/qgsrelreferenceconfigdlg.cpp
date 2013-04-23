/***************************************************************************
    qgsrelreferenceconfigdlg.cpp
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

#include "qgsrelreferenceconfigdlg.h"

QgsRelReferenceConfigDlg::QgsRelReferenceConfigDlg( QWidget *parent ) :
  QgsEditorConfigWidget( parent )
{
  setupUi(this);
}

QMap<QString, QVariant> QgsRelReferenceConfigDlg::config()
{
  QMap<QString, QVariant> myConfig;
  myConfig.insert( "A", "B" );

  return myConfig;
}
