/***************************************************************************
    qgsenumerationconfigdlg.h
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

#ifndef QGSENUMERATIONCONFIGDLG_H
#define QGSENUMERATIONCONFIGDLG_H

#include "ui_qgsenumerationconfigdlgbase.h"

class QgsEnumerationConfigDlgBase : public QWidget, private Ui::QgsEnumerationConfigDlgBase
{
    Q_OBJECT

  public:
    explicit QgsEnumerationConfigDlgBase( QWidget *parent = 0 );

  protected:
    void changeEvent( QEvent *e );
};

#endif // QGSENUMERATIONCONFIGDLG_H
