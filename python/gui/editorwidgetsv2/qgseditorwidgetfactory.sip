/***************************************************************************
    qgseditorwidgetfactory.sip
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

/**
 * Every attribute editor widget needs a factory, which inherits this class
 */
class QgsEditWidgetFactory {
%TypeHeaderCode
#include <qgseditorwidgetfactory.h>
%End

  public:
    virtual QgsEditorWidgetWrapper* create( QObject* parent ) const = 0;
    virtual QString name() const = 0;
    virtual QgsEditorConfigWidget* configWidget( QWidget* parent ) = 0;
};

/**
 * This class manages all known edit widget factories
 */
class QgsEditorWidgetRegistry : public QObject {
%TypeHeaderCode
#include <qgseditorwidgetfactory.h>
%End

  public:
    static QgsEditorWidgetRegistry* instance();
    void QgsEditorWidgetRegistry::registerWidget( const QString& widgetType, QgsEditWidgetFactory* widgetFactory );
    QgsEditorWidgetWrapper* create(const QString& widgetType , QObject* parent);
    QgsEditorConfigWidget* createConfigWidget( const QString& widgetId, QWidget* parent );

    const QMap<QString, QgsEditWidgetFactory*> factories();
};
