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

/**
 * Holds a set of configuration parameters for a editor widget wrapper.
 * It's basically a set of key => value pairs.
 *
 * If you need more advanced structures than a simple key => value pair,
 * you can use a value to hold any structure a QVariant can handle (and that's
 * about anything you get through your compiler)
 *
 * These are the user configurable options in the field properties tab of the
 * vector layer properties. They are saved in the project file per layer and field.
 * You get these passed, for every new widget wrapper.
 */
typedef QMap<QString, QVariant> QgsEditorWidgetConfig;

/**
 * Manages an editor widget
 * Widget and wrapper share the same parent
 *
 * You need
 */
class GUI_EXPORT QgsEditorWidgetWrapper : public QObject
{
    Q_OBJECT
  public:
    explicit QgsEditorWidgetWrapper( QgsVectorLayer* vl, int fieldIdx, QWidget* editor = 0, QWidget* parent = 0 );

    /**
     * Will be used to access the widget's value. Read the value from the widget and
     * return it properly formatted to be saved in the attribute.
     */
    virtual QVariant value() = 0;

    /**
     * @brief Access the widget managed by this wrapper
     * @return The widget
     */
    QWidget* widget();

		template <class T>
		T* widget() { return dynamic_cast<T>( mWidget ); }

    /**
     * Will set the config of this wrapper to the specified config.
     *
     * @param config The config for this wrapper
     */
    void setConfig( QgsEditorWidgetConfig config );

    /**
     * Use this inside your overriden classes to access the configuration.
     *
     * @param key         The configuration option you want to load
     * @param defaultVal  Default value
     *
     * @return the value assigned to this configuration option
     */
    QVariant config( QString key, QVariant defaultVal = QVariant() );

    /**
     * Returns the whole config
     *
     * @return The configuration
     */
    const QgsEditorWidgetConfig config();

    /**
     * Access the QgsVectorLayer, you are working on
     *
     * @return The layer
     *
     * @see field()
     */
    QgsVectorLayer* layer();

    /**
     * Access the field index.
     *
     * @return The index of the field you are working on
     *
     * @see layer()
     */
    int field();

    /**
     * Will return a wrapper for a given widget
     * @param widget The widget which was created by a wrapper
     * @return The wrapper for the widget or NULL
     */
    static QgsEditorWidgetWrapper* fromWidget( QWidget* widget );

  protected:
    /**
     * This method should create a new widget with the provided parent. This will only be called
     * if the form did not already provide a widget, so it is not guaranteed to be called!
     * You should not do initialisation stuff, which also has to be done for custom editor
     * widgets inside this method. Things like filling comboboxes and assigning other data which
     * will also be used to make widgets on forms created in the QtDesigner usable should be assigned
     * in {@link initWidget(QWidget*)}.
     *
     * @param parent You should set this parent on the created widget.
     * @return A new widget
     */
    virtual QWidget* createWidget( QWidget* parent ) = 0;

    /**
     * This method should initialize the editor widget with runtime data. Fill your comboboxes here.
     *
     * @param editor The widget which will represent this attribute editor in a form.
     */
    virtual void initWidget( QWidget* editor );

  signals:
    /**
     * Emit this signal, whenever the value changed.
     *
     * @param value The new value
     */
    void valueChanged( const QVariant& value );

  public slots:
    /**
     * Is called, when the value of the widget needs to be changed. Update the widget representation
     * to reflect the new value.
     *
     * @param value The new value of the attribute
     */
    virtual void setValue( const QVariant& value ) = 0;

    /**
     * Is used to enable or disable the edit functionality of the managed widget.
     * By default this will enable or disable the whole widget
     *
     * @param enabled  Enable or disable?
     */
    virtual void setEnabled( bool enabled );

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
