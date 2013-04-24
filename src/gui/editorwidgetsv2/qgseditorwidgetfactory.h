/***************************************************************************
    qgseditorwidgetfactory.h
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

#ifndef QGSEDITORWIDGETFACTORY_H
#define QGSEDITORWIDGETFACTORY_H

#include <QMap>
#include <QString>
#include "qgseditorwidgetwrapper.h"

class QgsEditorConfigWidget;

/**
 * Every attribute editor widget wrapper needs a factory, which inherits this class
 * It provides metadata for the widgets such as the name, a configuration widget
 */
class QgsEditorWidgetFactory {
  public:
    virtual ~QgsEditorWidgetFactory() {}
    virtual QgsEditorWidgetWrapper* create( QObject* parent ) const = 0;
    virtual QString name() const = 0;
    virtual QgsEditorConfigWidget* configWidget( QWidget* parent ) = 0;
};

/**
 * This is a templated wrapper class, which inherits QgsEditWidgetFactory and does the boring work for you.
 */
template<typename F, typename G>
class QgsEditWidgetFactoryHelper : public QgsEditorWidgetFactory
{
  public:
    QgsEditWidgetFactoryHelper( QString name )
      : mName( name ) {}

    QgsEditorWidgetWrapper* create( QObject* parent ) const { return new F( parent ); }
    QString name() const { return mName; }
    QgsEditorConfigWidget* configWidget( QWidget* parent) { return new G( parent ); };

  private:
    QString mName;
};

#endif // QGSEDITORWIDGETFACTORY_H
