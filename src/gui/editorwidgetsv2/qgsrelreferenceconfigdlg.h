/***************************************************************************
    qgsrelreferenceconfigdlgbase.h
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

#ifndef QGSRELREFERENCECONFIGDLGBASE_H
#define QGSRELREFERENCECONFIGDLGBASE_H

#include "ui_qgsrelreferenceconfigdlgbase.h"
#include "qgseditorconfigwidget.h"

class QgsRelReferenceConfigDlg : public QgsEditorConfigWidget, private Ui::QgsRelReferenceConfigDlgBase
{
    Q_OBJECT
    
  public:
    explicit QgsRelReferenceConfigDlg( QgsVectorLayer* vl, int fieldIdx, QWidget* parent );
    virtual QMap<QString, QVariant> config();
    virtual void setConfig( const QMap<QString, QVariant>& config );
};

#endif // QGSRELREFERENCECONFIGDLGBASE_H
