/***************************************************************************
    qgseditorwidgetregistry.h
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

#ifndef QGSEDITORWIDGETREGISTRY_H
#define QGSEDITORWIDGETREGISTRY_H

#include <QObject>
#include <QMap>

#include "qgseditorwidgetfactory.h"

class QgsMapLayer;
class QDomNode;

/**
 * This class manages all known edit widget factories
 */
class QgsEditorWidgetRegistry : public QObject
{
    Q_OBJECT

  public:
    /**
     * This class is a singleton and has therefore to be accessed with this method instead
     * of a constructor.
     *
     * @return
     */
    static QgsEditorWidgetRegistry* instance();
    ~QgsEditorWidgetRegistry();

    QgsEditorWidgetWrapper* create( const QString& widgetType, QgsVectorLayer* vl, int fieldIdx, const QgsEditorWidgetConfig& config, QWidget* editor, QWidget* parent );
    QgsEditorConfigWidget* createConfigWidget( const QString& widgetId, QgsVectorLayer* vl, int fieldIdx, QWidget* parent );
    QString name( const QString& widgetId );

    const QMap<QString, QgsEditorWidgetFactory*> factories();

    /**
     * The other part which does the boring work for you
     */
    template <class W, class C>
    void registerWidget( const QString& widgetType, const QString& name )
    {
      mWidgetFactories.insert( widgetType, new QgsEditWidgetFactoryHelper<W, C>( name ) );
    }

    /**
     * Register a new widgetfactory
     */
    void registerWidget( const QString& widgetType, QgsEditorWidgetFactory* widgetFactory );

  protected:
    QgsEditorWidgetRegistry();

    /**
     * Any widget types which are compiled with this library can be registered within
     * this method.
     */
    void initKnownTypes();

  private slots:
    void readMapLayer( QgsMapLayer* mapLayer , const QDomElement& layerElem );
    void writeMapLayer( QgsMapLayer* mapLayer , QDomElement& layerElem, QDomDocument& doc );

  private:
    QMap<QString, QgsEditorWidgetFactory*> mWidgetFactories;
    static QgsEditorWidgetRegistry* sInstance;
};


#endif // QGSEDITORWIDGETREGISTRY_H
