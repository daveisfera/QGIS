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

#include <QWidget>

QgsEditorWidgetWrapper::QgsEditorWidgetWrapper( QgsVectorLayer* vl, int fieldIdx, QWidget* parent )
  : QObject( parent )
  , mWidget( NULL )
  , mParent( parent )
  , mLayer( vl )
{
  mField = fieldIdx;
}

QWidget* QgsEditorWidgetWrapper::widget()
{
  if ( !mWidget )
  {
    mWidget = createWidget( mParent );
    mWidget->setProperty( "Wrapper", QVariant( QMetaType::QObjectStar, this ) );
  }
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

QgsVectorLayer*QgsEditorWidgetWrapper::layer()
{
  mLayer;
}

int QgsEditorWidgetWrapper::field()
{
  return mField;
}
