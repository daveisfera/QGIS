/***************************************************************************
    qgseditorconfigwidget.h
     --------------------------------------
    Date                 : 24.4.2013
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

#ifndef QGSEDITORCONFIGWIDGET_H
#define QGSEDITORCONFIGWIDGET_H

#include <QWidget>

#include "qgseditorwidgetwrapper.h"

class QgsVectorLayer;

class GUI_EXPORT QgsEditorConfigWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit QgsEditorConfigWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* parent );

    int field();

    QgsVectorLayer* layer();

    virtual ~QgsEditorConfigWidget() {}

    virtual QgsEditorWidgetConfig config() = 0;
    virtual void setConfig( const QgsEditorWidgetConfig& config ) = 0;

  private:
    QgsVectorLayer* mLayer;
    int mField;
};

#endif // QGSEDITORCONFIGWIDGET_H
