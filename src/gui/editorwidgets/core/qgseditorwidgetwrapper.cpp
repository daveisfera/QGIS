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
#include "qgsvectorlayer.h"
#include "qgsfield.h"

#include <QWidget>

QgsEditorWidgetWrapper::QgsEditorWidgetWrapper( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent )
    : QObject( parent )
    , mWidget( editor )
    , mParent( parent )
    , mLayer( vl )
{
  mFieldIdx = fieldIdx;
}

QWidget* QgsEditorWidgetWrapper::widget()
{
  if ( !mWidget )
  {
    mWidget = createWidget( mParent );
    mWidget->setProperty( "EWV2Wrapper", QVariant::fromValue( this ) );
    initWidget( mWidget );
  }

  return mWidget;
}

void QgsEditorWidgetWrapper::setConfig( const QgsEditorWidgetConfig& config )
{
  mConfig = config;
  // If an editor widget was supplied, we can initialize this now
  if ( mWidget )
  {
    mWidget->setProperty( "EWV2Wrapper", QVariant::fromValue( this ) );
    initWidget( mWidget );
  }
}

QVariant QgsEditorWidgetWrapper::config( QString key, QVariant defaultVal )
{
  if ( mConfig.contains( key ) )
  {
    return mConfig[key];
  }
  return defaultVal;
}

const QgsEditorWidgetConfig QgsEditorWidgetWrapper::config()
{
  return mConfig;
}

QgsVectorLayer* QgsEditorWidgetWrapper::layer()
{
  return mLayer;
}

int QgsEditorWidgetWrapper::fieldIdx()
{
  return mFieldIdx;
}

QgsField QgsEditorWidgetWrapper::field()
{
  return mLayer->pendingFields()[mFieldIdx];
}

QgsEditorWidgetWrapper* QgsEditorWidgetWrapper::fromWidget( QWidget* widget )
{
  QVariant w = widget->property( "EWV2Wrapper" );

  if ( w.isNull() )
  {
    return NULL;
  }

  return w.value<QgsEditorWidgetWrapper*>();
}

void QgsEditorWidgetWrapper::initWidget( QWidget* editor )
{
  Q_UNUSED( editor )
}

void QgsEditorWidgetWrapper::setEnabled( bool enabled )
{
  if ( mWidget )
  {
    mWidget->setEnabled( enabled );
  }
}
