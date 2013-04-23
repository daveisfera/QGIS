/***************************************************************************
    qgseditorwidgetwrapper.cpp
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

#include "qgseditorwidgetwrapper.h"

QgsEditorWidgetWrapper::QgsEditorWidgetWrapper( QObject* parent ) :
  QObject( parent )
{
}

void QgsEditorWidgetWrapper::setConfig(QMap<QString, QVariant> config)
{
  mConfig = config;
}

QVariant QgsEditorWidgetWrapper::config( QString key )
{
  if ( mConfig.contains( key ) )
  {
    return mConfig[key];
  }
  else
  {
    return QVariant();
  }
}
