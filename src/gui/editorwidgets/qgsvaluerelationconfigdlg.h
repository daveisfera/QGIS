/***************************************************************************
    qgsvaluerelationconfigdlg.h
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

#ifndef QGSVALUERELATIONCONFIGDLG_H
#define QGSVALUERELATIONCONFIGDLG_H

#include "ui_qgsvaluerelationconfigdlgbase.h"

class QgsValueRelationConfigDlgBase : public QWidget, private Ui::QgsValueRelationConfigDlgBase
{
    Q_OBJECT

  public:
    explicit QgsValueRelationConfigDlgBase( QWidget *parent = 0 );

  protected:
    void changeEvent( QEvent *e );
};

#endif // QGSVALUERELATIONCONFIGDLG_H
