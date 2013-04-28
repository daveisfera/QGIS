/***************************************************************************
    qgseditorwidgetwrapper.h
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

#ifndef QGSEDITORWIDGETWRAPPER_H
#define QGSEDITORWIDGETWRAPPER_H

#include <QObject>
#include <QMap>
#include <QVariant>

class QgsVectorLayer;

typedef QMap<QString, QVariant> QgsEditorWidgetConfig;

/**
 * One wrapper per edit widget.
 * Widget and wrapper share the same parent
 */
class QgsEditorWidgetWrapper : public QObject
{
    Q_OBJECT
  public:
    explicit QgsEditorWidgetWrapper(QgsVectorLayer* vl, int fieldIdx, QWidget* parent = 0 );
    QWidget* widget();
    virtual void setConfig( QgsEditorWidgetConfig config );
    virtual QVariant value() = 0;
    QVariant config( QString key );

    QgsVectorLayer* layer();
    int field();

    /**
     * Will return a wrapper for a given widget
     * @param widget The widget which was created by a wrapper
     * @return The wrapper for the widget or NULL
     */
    static QgsEditorWidgetWrapper* fromWidget( QWidget* widget );

  protected:
    virtual QWidget* createWidget( QWidget* parent ) = 0;

  signals:
    void valueChanged( const QVariant& value );
    
  public slots:
    virtual void setValue( const QVariant& value ) = 0;

  private:
    QgsEditorWidgetConfig mConfig;
    QWidget* mWidget;
    QWidget* mParent;
    QgsVectorLayer* mLayer;
    int mField;
};

// We'll use this class inside a QVariant in the widgets properties
Q_DECLARE_METATYPE( QgsEditorWidgetWrapper* )

#endif // QGSEDITORWIDGETWRAPPER_H
