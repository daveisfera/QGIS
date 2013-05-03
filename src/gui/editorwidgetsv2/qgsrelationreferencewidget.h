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

#include <QComboBox>
#include <QVBoxLayout>

class QgsAttributeDialog;

class GUI_EXPORT QgsRelationReferenceWidget : public QgsEditorWidgetWrapper
{
    Q_OBJECT
  public:
    explicit QgsRelationReferenceWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent = 0 );
    virtual QWidget* createWidget( QWidget* parent );
    virtual void initWidget( QWidget* editor );
    virtual QVariant value();

  signals:
    void valueChanged( const QVariant& value );

  public slots:
    virtual void setValue( const QVariant& value );
    void referenceChanged( int index );

  private:
    QComboBox* mComboBox;
    QFrame* mAttributeEditorFrame;
    QVBoxLayout* mAttributeEditorLayout;
    QgsVectorLayer* mReferencedLayer;
    QVariant mCurrentValue;
    QgsAttributeDialog* mAttributeDialog;
};

#endif // QGSRELATIONREFERENCEWIDGET_H
