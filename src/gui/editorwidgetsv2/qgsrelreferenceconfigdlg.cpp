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
#include "qgseditorwidgetfactory.h"

#include "qgsvectorlayer.h"

QgsRelReferenceConfigDlg::QgsRelReferenceConfigDlg( QgsVectorLayer* vl, int fieldIdx, QWidget* parent )
  : QgsEditorConfigWidget( vl, fieldIdx, parent )
{
  setupUi(this);

  const QgsFields& fields = vl->pendingFields();
  for ( int i = 0; i < fields.count(); ++i )
  {
    mComboDisplayField->addItem( fields[i].name(), fields[i].name() );
  }
}

void QgsRelReferenceConfigDlg::setConfig( const QMap<QString, QVariant>& config )
{
  if ( config.contains( "AllowNULL" ) )
  {
    mCbxAllowNull->setChecked( config[ "AllowNULL" ].toBool() );
  }

  if ( config.contains( "ShowForm" ) )
  {
    mCbxShowForm->setChecked( config[ "ShowForm" ].toBool() );
  }

  if ( config.contains( "DisplayField" ) )
  {
    mComboDisplayField->setCurrentIndex( mComboDisplayField->findData( config["DisplayField"].toString() ) );
  }
}

QMap<QString, QVariant> QgsRelReferenceConfigDlg::config()
{
  QMap<QString, QVariant> myConfig;
  myConfig.insert( "AllowNULL", mCbxAllowNull->isChecked() );
  myConfig.insert( "ShowForm", mCbxShowForm->isChecked() );
  myConfig.insert( "DisplayField", mComboDisplayField->currentText() );

  return myConfig;
}
