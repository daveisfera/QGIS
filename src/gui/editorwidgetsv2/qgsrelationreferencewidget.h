/***************************************************************************
    qgsrelationreferencewidget.h
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

#ifndef QGSRELATIONREFERENCEWIDGET_H
#define QGSRELATIONREFERENCEWIDGET_H

#include "qgseditorwidgetwrapper.h"

class QgsRelationReferenceWidget : public QgsEditorWidgetWrapper
{
    Q_OBJECT
  public:
    explicit QgsRelationReferenceWidget( QObject *parent = 0 );
    virtual QWidget* widget( QWidget* parent );
    virtual const QVariant& value();

  protected:
  signals:
    void valueChanged( const QVariant& value );

  public slots:
    virtual void setValue( const QVariant& value );
};

#endif // QGSRELATIONREFERENCEWIDGET_H
