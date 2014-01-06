/***************************************************************************
    qgsuniquevaluesconfigdlg.h
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

#ifndef QGSUNIQUEVALUESCONFIGDLG_H
#define QGSUNIQUEVALUESCONFIGDLG_H

#include "ui_qgsuniquevaluesconfigdlgbase.h"

class QgsUniqueValuesConfigDlgBase : public QWidget, private Ui::QgsUniqueValuesConfigDlgBase
{
    Q_OBJECT

  public:
    explicit QgsUniqueValuesConfigDlgBase( QWidget *parent = 0 );

  protected:
    void changeEvent( QEvent *e );
};

#endif // QGSUNIQUEVALUESCONFIGDLG_H
