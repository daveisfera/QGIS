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

#include "qgseditorwidgetwrapper.h"
#include "qgsapplication.h"

#include <QDomNode>
#include <QMap>
#include <QString>

class QgsEditorConfigWidget;

/**
 * Every attribute editor widget wrapper needs a factory, which inherits this class
 * It provides metadata for the widgets such as the name, a configuration widget
 */
class QgsEditorWidgetFactory {
  public:
    virtual ~QgsEditorWidgetFactory();
    virtual QgsEditorWidgetWrapper* create( QgsVectorLayer* vl, int fieldIdx, QWidget* parent ) const = 0;
    virtual QString name() const = 0;
    virtual QgsEditorConfigWidget* configWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* parent ) = 0;
    virtual QgsEditorWidgetConfig readConfig( const QDomElement& configElement, QgsVectorLayer* layer, int fieldIdx );
    virtual void writeConfig( const QgsEditorWidgetConfig& config, QDomElement& configElement, const QDomDocument& doc, const QgsVectorLayer* layer, int fieldIdx );
};

/**
 * This is a templated wrapper class, which inherits QgsEditWidgetFactory and does the boring work for you.
 * C++ only
 */
template<typename F, typename G>
class QgsEditWidgetFactoryHelper : public QgsEditorWidgetFactory
{
  public:
    QgsEditWidgetFactoryHelper( QString name )
      : mName( name ) {}

    QgsEditorWidgetWrapper* create( QgsVectorLayer* vl, int fieldIdx, QWidget* parent ) const
    {
      return new F( vl, fieldIdx, parent );
    }

    QString name() const
    {
      return mName;
    }

    QgsEditorConfigWidget* configWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* parent)
    {
      return new G( vl, fieldIdx, parent );
    }

    /**
     * Implement this method yourself somewhere with the class template parameters
     * specified. To keep things clean, every implementation of this class should be placed
     * next to the associated widget factory implementation.
     *
     * @param layer
     * @param configNode
     */

    virtual QgsEditorWidgetConfig readConfig( const QDomElement& configElement, QgsVectorLayer* layer, int fieldIdx );
    virtual void writeConfig( const QgsEditorWidgetConfig& config, QDomElement& configElement, const QDomDocument& doc, const QgsVectorLayer* layer, int fieldIdx );

  private:
    QString mName;
};

#endif // QGSEDITORWIDGETFACTORY_H
