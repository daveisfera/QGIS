/***************************************************************************
    qgseditorwidgetregistry.cpp
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

#include "qgseditorwidgetregistry.h"

#include "qgseditorwidgetfactory.h"
#include "qgsproject.h"
#include "qgsvectorlayer.h"
#include "qgsmessagelog.h"

QgsEditorWidgetRegistry* QgsEditorWidgetRegistry::instance()
{
  static QgsEditorWidgetRegistry sInstance;
  return &sInstance;
}

QgsEditorWidgetRegistry::QgsEditorWidgetRegistry()
{
  connect( QgsProject::instance(), SIGNAL( readMapLayer( QgsMapLayer*, const QDomElement& ) ), this, SLOT( readMapLayer( QgsMapLayer*, const QDomElement& ) ) );
  connect( QgsProject::instance(), SIGNAL( writeMapLayer( QgsMapLayer*, QDomElement&, QDomDocument& ) ), this, SLOT( writeMapLayer( QgsMapLayer*, QDomElement&, QDomDocument& ) ) );
}

QgsEditorWidgetRegistry::~QgsEditorWidgetRegistry()
{
  qDeleteAll( mWidgetFactories.values() );
}

QgsEditorWidgetWrapper* QgsEditorWidgetRegistry::create( const QString& widgetId, QgsVectorLayer* vl, int fieldIdx, const QgsEditorWidgetConfig& config, QWidget* editor, QWidget* parent )
{
  if ( mWidgetFactories.contains( widgetId ) )
  {
    QgsEditorWidgetWrapper* ww = mWidgetFactories[widgetId]->create( vl, fieldIdx, editor, parent );
    if ( ww )
    {
      ww->setConfig( config );
      return ww;
    }
  }
  return 0;
}

QgsEditorConfigWidget* QgsEditorWidgetRegistry::createConfigWidget( const QString& widgetId, QgsVectorLayer* vl, int fieldIdx, QWidget* parent )
{
  if ( mWidgetFactories.contains( widgetId ) )
  {
    return mWidgetFactories[widgetId]->configWidget( vl, fieldIdx, parent );
  }
  return 0;
}

QString QgsEditorWidgetRegistry::name( const QString& widgetId )
{
  if ( mWidgetFactories.contains( widgetId ) )
  {
    return mWidgetFactories[widgetId]->name();
  }

  return QString();
}

const QMap<QString, QgsEditorWidgetFactory*> QgsEditorWidgetRegistry::factories()
{
  return mWidgetFactories;
}

bool QgsEditorWidgetRegistry::registerWidget( const QString& widgetId, QgsEditorWidgetFactory* widgetFactory )
{
  if ( !widgetFactory )
  {
    QgsMessageLog::instance()->logMessage( "QgsEditorWidgetRegistry: Factory not valid." );
    return false;
  }
  else if ( mWidgetFactories.contains( widgetId ) )
  {
    QgsMessageLog::instance()->logMessage( QString( "QgsEditorWidgetRegistry: Factory with id %1 already registered." ).arg( widgetId ) );
    return false;
  }
  else
  {
    mWidgetFactories.insert( widgetId, widgetFactory );
    return true;
  }
}

void QgsEditorWidgetRegistry::readMapLayer( QgsMapLayer* mapLayer, const QDomElement& layerElem )
{
  if ( mapLayer->type() != QgsMapLayer::VectorLayer )
  {
    return;
  }

  QgsVectorLayer* vectorLayer = qobject_cast<QgsVectorLayer*>( mapLayer );
  Q_ASSERT( vectorLayer );

  QDomNodeList editTypeNodes = layerElem.namedItem( "edittypes" ).childNodes();

  for ( int i = 0; i < editTypeNodes.size(); i++ )
  {
    QDomNode editTypeNode = editTypeNodes.at( i );
    QDomElement editTypeElement = editTypeNode.toElement();

    QString name = editTypeElement.attribute( "name" );

    int idx = vectorLayer->fieldNameIndex( name );

    QgsVectorLayer::EditType editType =
      ( QgsVectorLayer::EditType ) editTypeElement.attribute( "type" ).toInt();

    QString ewv2Type;
    if ( editType != QgsVectorLayer::EditorWidgetV2 )
      ewv2Type = readLegacyConfig( vectorLayer, editTypeElement );
    else
      ewv2Type = editTypeElement.attribute( "widgetv2type" );

    if ( mWidgetFactories.contains( ewv2Type ) )
    {
      vectorLayer->setEditorWidgetV2( idx, ewv2Type );
      QDomElement ewv2CfgElem = editTypeElement.namedItem( "widgetv2config" ).toElement();

      if ( !ewv2CfgElem.isNull() )
      {
        QMap<QString, QVariant> cfg = mWidgetFactories[ewv2Type]->readEditorConfig( ewv2CfgElem, vectorLayer, idx );
        vectorLayer->setEditorWidgetV2Config( idx, cfg );
      }
    }
    else
    {
      QgsMessageLog::logMessage( tr( "Unknown attribute editor widget '%1'" ).arg( ewv2Type ) );
    }
  }
}

QString QgsEditorWidgetRegistry::readLegacyConfig( QgsVectorLayer* vl, const QDomElement& editTypeElement )
{
  QString name = editTypeElement.attribute( "name" );

  QgsVectorLayer::EditType editType = ( QgsVectorLayer::EditType ) editTypeElement.attribute( "type" ).toInt();

  QString widgetType = "LineEdit"; // Fallback

  switch ( editType )
  {
    case QgsVectorLayer::ValueMap:
      widgetType = "ValueMap";
#if 0
      if ( editTypeNode.hasChildNodes() )
      {
        mValueMaps.insert( name, QMap<QString, QVariant>() );

        QDomNodeList valueMapNodes = editTypeNode.childNodes();
        for ( int j = 0; j < valueMapNodes.size(); j++ )
        {
          QDomElement value = valueMapNodes.at( j ).toElement();
          mValueMaps[ name ].insert( value.attribute( "key" ), value.attribute( "value" ) );
        }
      }
#endif
      break;

    case QgsVectorLayer::EditRange:
      widgetType = "EditRange";
      break;
    case QgsVectorLayer::SliderRange:
      widgetType = "SliderRange";
      break;
    case QgsVectorLayer::DialRange:
      widgetType = "DialRange";
      break;
#if 0
    {
      QVariant min = editTypeElement.attribute( "min" );
      QVariant max = editTypeElement.attribute( "max" );
      QVariant step = editTypeElement.attribute( "step" );

      mRanges[ name ] = RangeData( min, max, step );
    }
#endif
    break;

    case QgsVectorLayer::CheckBox:
      widgetType = "CheckBox";
      break;
#if 0
      mCheckedStates[ name ] = QPair<QString, QString>( editTypeElement.attribute( "checked" ), editTypeElement.attribute( "unchecked" ) );
      break;
#endif
    case QgsVectorLayer::ValueRelation:
      widgetType = "ValueRelation";
      break;
#if 0
    {
      QString id = editTypeElement.attribute( "layer" );
      QString key = editTypeElement.attribute( "key" );
      QString value = editTypeElement.attribute( "value" );
      bool allowNull = editTypeElement.attribute( "allowNull" ) == "true";
      bool orderByValue = editTypeElement.attribute( "orderByValue" ) == "true";
      bool allowMulti = editTypeElement.attribute( "allowMulti", "false" ) == "true";

      QString filterExpression;
      if ( editTypeElement.hasAttribute( "filterAttributeColumn" ) &&
           editTypeElement.hasAttribute( "filterAttributeValue" ) )
      {
        filterExpression = QString( "\"%1\"='%2'" )
                           .arg( editTypeElement.attribute( "filterAttributeColumn" ) )
                           .arg( editTypeElement.attribute( "filterAttributeValue" ) );
      }
      else
      {
        filterExpression  = editTypeElement.attribute( "filterExpression", QString::null );
      }

      mValueRelations[ name ] = ValueRelationData( id, key, value, allowNull, orderByValue, allowMulti, filterExpression );
    }
    break;
#endif
    case QgsVectorLayer::Calendar:
      widgetType = "Calendar";
#if 0
      mDateFormats[ name ] = editTypeElement.attribute( "dateFormat" );
#endif
      break;

    case QgsVectorLayer::Photo:
      widgetType = "Photo";
      break;
    case QgsVectorLayer::WebView:
      widgetType = "WebView";
      break;
#if 0
      mWidgetSize[ name ] = QSize( editTypeElement.attribute( "widgetWidth" ).toInt(), editTypeElement.attribute( "widgetHeight" ).toInt() );
      break;
#endif
    case QgsVectorLayer::Classification:
      widgetType = "Classification";
      break;

    case QgsVectorLayer::FileName:
      widgetType = "FileName";
      break;

    case QgsVectorLayer::Immutable:
      widgetType = "Immutable";
      break;

    case QgsVectorLayer::Hidden:
      widgetType = "Hidden";
      break;

    case QgsVectorLayer::LineEdit:
      widgetType = "LineEdit";
      break;

    case QgsVectorLayer::TextEdit:
      widgetType = "TextEdit";
      break;

    case QgsVectorLayer::Enumeration:
      widgetType = "Enumeration";
      break;

    case QgsVectorLayer::UniqueValues:
      widgetType = "UniqueValues";
      break;

    case QgsVectorLayer::UniqueValuesEditable:
      widgetType = "UniqueValuesEditable";
      break;

    case QgsVectorLayer::UuidGenerator:
      widgetType = "UuidGenerator";
      break;

    case QgsVectorLayer::Color:
      widgetType = "Color";
      break;

    case QgsVectorLayer::EditorWidgetV2: // Should not land here
      break;
  }

  return widgetType;
}

void QgsEditorWidgetRegistry::writeMapLayer( QgsMapLayer* mapLayer, QDomElement& layerElem, QDomDocument& doc )
{
  if ( mapLayer->type() != QgsMapLayer::VectorLayer )
  {
    return;
  }

  QgsVectorLayer* vectorLayer = qobject_cast<QgsVectorLayer*>( mapLayer );
  if ( !vectorLayer )
  {
    return;
  }

  for ( int idx = 0; idx < vectorLayer->pendingFields().count(); ++idx )
  {
    if ( vectorLayer->editType( idx ) != QgsVectorLayer::EditorWidgetV2 )
    {
      continue;
    }

    const QString& widgetType = vectorLayer->editorWidgetV2( idx );
    if ( !mWidgetFactories.contains( widgetType ) )
    {
      QgsMessageLog::logMessage( tr( "Could not save unknown editor widget type '%1'." ).arg( widgetType ) );
      continue;
    }

    QDomNodeList editTypeNodes = layerElem.namedItem( "edittypes" ).childNodes();

    for ( int i = 0; i < editTypeNodes.size(); i++ )
    {
      QDomElement editTypeElement = editTypeNodes.at( i ).toElement();

      QString name = editTypeElement.attribute( "name" );

      if ( vectorLayer->fieldNameIndex( name ) != idx )
        continue;

      QgsVectorLayer::EditType editType =
        ( QgsVectorLayer::EditType ) editTypeElement.attribute( "type" ).toInt();

      if ( editType != QgsVectorLayer::EditorWidgetV2 )
        continue;

      editTypeElement.setAttribute( "widgetv2type", widgetType );

      if ( mWidgetFactories.contains( widgetType ) )
      {
        QDomElement ewv2CfgElem = doc.createElement( "widgetv2config" );

        mWidgetFactories[widgetType]->writeConfig( vectorLayer->editorWidgetV2Config( idx ), ewv2CfgElem, doc, vectorLayer, idx );

        editTypeElement.appendChild( ewv2CfgElem );
      }
      else
      {
        QgsMessageLog::logMessage( tr( "Unknown attribute editor widget '%1'" ).arg( widgetType ) );
      }
    }
  }
}
