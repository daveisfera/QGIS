/***************************************************************************
                         qgsattributetypedialog.cpp  -  description
                             -------------------
    begin                : June 2009
    copyright            : (C) 2000 by Richard Kostecky
    email                : cSf.Kostej@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsattributetypedialog.h"
#include "qgsattributetypeloaddialog.h"
#include "qgsvectordataprovider.h"
#include "qgsmaplayerregistry.h"
#include "qgsmapcanvas.h"
#include "qgsexpressionbuilderdialog.h"
#include "qgisapp.h"
#include "qgsproject.h"
#include "qgslogger.h"
#include "qgseditorwidgetfactory.h"
#include "qgseditorwidgetregistry.h"

#include <QTableWidgetItem>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include <climits>
#include <cfloat>

QgsAttributeTypeDialog::QgsAttributeTypeDialog( QgsVectorLayer *vl )
    : QDialog()
    , mLayer( vl )
{
  setupUi( this );
  connect( selectionListWidget, SIGNAL( currentRowChanged( int ) ), this, SLOT( setStackPage( int ) ) );
  connect( valueRelationEditExpression, SIGNAL( clicked() ), this, SLOT( editValueRelationExpression() ) );

  QMapIterator<QString, QgsEditorWidgetFactory*> i( QgsEditorWidgetRegistry::instance()->factories() );
  while ( i.hasNext() )
  {
    i.next();
    QListWidgetItem* item = new QListWidgetItem( selectionListWidget );
    item->setText( i.value()->name() );
    item->setData( Qt::UserRole, i.key() );
    selectionListWidget->addItem( item );
  }

  valueRelationLayer->clear();
  foreach ( QgsMapLayer *l, QgsMapLayerRegistry::instance()->mapLayers() )
  {
    QgsVectorLayer *vl = qobject_cast< QgsVectorLayer * >( l );
    if ( vl )
      valueRelationLayer->addItem( vl->name(), vl->id() );
  }

  connect( valueRelationLayer, SIGNAL( currentIndexChanged( int ) ), this, SLOT( updateLayerColumns( int ) ) );
  valueRelationLayer->setCurrentIndex( -1 );
}

QgsAttributeTypeDialog::~QgsAttributeTypeDialog()
{

}

QgsVectorLayer::EditType QgsAttributeTypeDialog::editType()
{
  return mEditType;
}

const QString QgsAttributeTypeDialog::editorWidgetV2Type()
{
  QListWidgetItem* item = selectionListWidget->currentItem();
  if ( item )
  {
    return item->data( Qt::UserRole ).toString();
  }
  else
  {
    return QString();
  }
}

const QString QgsAttributeTypeDialog::editorWidgetV2Text()
{
  QListWidgetItem* item = selectionListWidget->currentItem();
  if ( item )
  {
    return item->text();
  }
  else
  {
    return QString();
  }
}

const QMap<QString, QVariant> QgsAttributeTypeDialog::editorWidgetV2Config()
{
  QListWidgetItem* item = selectionListWidget->currentItem();
  if ( item )
  {
    QString widgetType = item->data( Qt::UserRole ).toString();
    QgsEditorConfigWidget* cfgWdg = mEditorConfigWidgets[ widgetType ];
    if ( cfgWdg )
    {
      return cfgWdg->config();
    }
  }

  return QMap<QString, QVariant>();
}

void QgsAttributeTypeDialog::setWidgetV2Config( const QMap<QString, QVariant>& config )
{
  mWidgetV2Config = config;
}

QgsVectorLayer::RangeData QgsAttributeTypeDialog::rangeData()
{
  return mRangeData;
}

QgsVectorLayer::ValueRelationData QgsAttributeTypeDialog::valueRelationData()
{
  return mValueRelationData;
}

QString QgsAttributeTypeDialog::dateFormat()
{
  return mDateFormat;
}

QSize QgsAttributeTypeDialog::widgetSize()
{
  return mWidgetSize;
}

QMap<QString, QVariant> &QgsAttributeTypeDialog::valueMap()
{
  return mValueMap;
}

bool QgsAttributeTypeDialog::fieldEditable()
{
  return isFieldEditableCheckBox->isChecked();
}

bool QgsAttributeTypeDialog::labelOnTop()
{
  return labelOnTopCheckBox->isChecked();
}

void QgsAttributeTypeDialog::setFieldEditable( bool editable )
{
  isFieldEditableCheckBox->setChecked( editable );
}

void QgsAttributeTypeDialog::setFieldEditableEnabled( bool enabled )
{
  isFieldEditableCheckBox->setEnabled( enabled );
}

void QgsAttributeTypeDialog::setLabelOnTop( bool onTop )
{
  labelOnTopCheckBox->setChecked( onTop );
}

QPair<QString, QString> QgsAttributeTypeDialog::checkedState()
{
  return QPair<QString, QString>( leCheckedState->text(), leUncheckedState->text() );
}

void QgsAttributeTypeDialog::setCheckedState( QString checked, QString unchecked )
{
  leCheckedState->setText( checked );
  leUncheckedState->setText( unchecked );
}

void QgsAttributeTypeDialog::editValueRelationExpression()
{
  QString id = valueRelationLayer->itemData( valueRelationLayer->currentIndex() ).toString();

  QgsVectorLayer *vl = qobject_cast< QgsVectorLayer *>( QgsMapLayerRegistry::instance()->mapLayer( id ) );
  if ( !vl )
    return;

  QgsExpressionBuilderDialog dlg( vl, valueRelationFilterExpression->toPlainText(), this );
  dlg.setWindowTitle( tr( "Edit filter expression" ) );

  QgsDistanceArea myDa;
  myDa.setSourceCrs( vl->crs().srsid() );
  myDa.setEllipsoidalMode( QgisApp::instance()->mapCanvas()->mapSettings().hasCrsTransformEnabled() );
  myDa.setEllipsoid( QgsProject::instance()->readEntry( "Measure", "/Ellipsoid", GEO_NONE ) );
  dlg.setGeomCalculator( myDa );

  if ( dlg.exec() == QDialog::Accepted )
  {
    valueRelationFilterExpression->setText( dlg.expressionBuilder()->expressionText() );
  }
}

void QgsAttributeTypeDialog::setPageForEditType( QgsVectorLayer::EditType editType )
{
  switch ( editType )
  {
    case QgsVectorLayer::LineEdit:
      setPage( 0 );
      break;

    case QgsVectorLayer::Classification:
      setPage( 1 );
      break;
#if 0
    case QgsVectorLayer::EditRange:
    case QgsVectorLayer::SliderRange:
    case QgsVectorLayer::DialRange:
      setPage( 2 );
      break;
#endif
    case QgsVectorLayer::UniqueValues:
    case QgsVectorLayer::UniqueValuesEditable:
      setPage( 3 );
      break;

    case QgsVectorLayer::FileName:
      setPage( 4 );
      break;

    case QgsVectorLayer::ValueMap:
      setPage( 5 );
      break;

    case QgsVectorLayer::Enumeration:
      setPage( 6 );
      break;

    case QgsVectorLayer::Immutable:
      setPage( 7 );
      break;

    case QgsVectorLayer::Hidden:
      setPage( 8 );
      break;

    case QgsVectorLayer::CheckBox:
      setPage( 9 );
      break;

    case QgsVectorLayer::TextEdit:
      setPage( 10 );
      break;

    case QgsVectorLayer::Calendar:
      setPage( 11 );
      break;

    case QgsVectorLayer::ValueRelation:
      setPage( 12 );
      break;

    case QgsVectorLayer::UuidGenerator:
      setPage( 13 );
      break;

    case QgsVectorLayer::Photo:
      setPage( 14 );
      break;

    case QgsVectorLayer::WebView:
      setPage( 15 );
      break;

    case QgsVectorLayer::Color:
      setPage( 16 );
      break;

    case QgsVectorLayer::EditorWidgetV2:
      setPage( 17 );
      break;
  }
}

void QgsAttributeTypeDialog::setValueMap( QMap<QString, QVariant> valueMap )
{
  mValueMap = valueMap;
}

void QgsAttributeTypeDialog::setRange( QgsVectorLayer::RangeData range )
{
  mRangeData = range;
}

void QgsAttributeTypeDialog::setValueRelation( QgsVectorLayer::ValueRelationData valueRelation )
{
  mValueRelationData = valueRelation;
}

void QgsAttributeTypeDialog::setDateFormat( QString dateFormat )
{
  mDateFormat = dateFormat;
}

void QgsAttributeTypeDialog::setWidgetSize( QSize widgetSize )
{
  mWidgetSize = widgetSize;
}

void QgsAttributeTypeDialog::setIndex( int index, QgsVectorLayer::EditType editType )
{
  mIndex = index;
  //need to set index for combobox

  setWindowTitle( defaultWindowTitle() + " \"" + mLayer->pendingFields()[index].name() + "\"" );
  QgsAttributeList attributeList = QgsAttributeList();
  attributeList.append( index );

  setPageForEditType( editType );

  switch ( editType )
  {
    case QgsVectorLayer::ValueMap:
    {

    }
    break;
#if 0
    case QgsVectorLayer::EditRange:
    case QgsVectorLayer::SliderRange:
    case QgsVectorLayer::DialRange:
    {
      if ( mLayer->pendingFields()[mIndex].type() != QVariant::Int
           || mLayer->pendingFields()[mIndex].type() != QVariant::LongLong )
      {
        minimumSpinBox->setValue( mRangeData.mMin.toInt() );
        maximumSpinBox->setValue( mRangeData.mMax.toInt() );
        stepSpinBox->setValue( mRangeData.mStep.toInt() );
      }
      else if ( mLayer->pendingFields()[mIndex].type() == QVariant::Double )
      {
        minimumDoubleSpinBox->setValue( mRangeData.mMin.toDouble() );
        maximumDoubleSpinBox->setValue( mRangeData.mMax.toDouble() );
        stepDoubleSpinBox->setValue( mRangeData.mStep.toDouble() );
      }

      if ( editType == QgsVectorLayer::EditRange )
      {
        rangeWidget->setCurrentIndex( 0 );
      }
      else if ( editType == QgsVectorLayer::SliderRange )
      {
        rangeWidget->setCurrentIndex( 1 );
      }
      else
      {
        rangeWidget->setCurrentIndex( 2 );
      }
    }
    break;
#endif
    case QgsVectorLayer::UniqueValuesEditable:
      editableUniqueValues->setChecked( true );
      break;

    case QgsVectorLayer::ValueRelation:
      valueRelationLayer->setCurrentIndex( valueRelationLayer->findData( mValueRelationData.mLayer ) );
      valueRelationKeyColumn->setCurrentIndex( valueRelationKeyColumn->findText( mValueRelationData.mKey ) );
      valueRelationValueColumn->setCurrentIndex( valueRelationValueColumn->findText( mValueRelationData.mValue ) );
      valueRelationAllowNull->setChecked( mValueRelationData.mAllowNull );
      valueRelationOrderByValue->setChecked( mValueRelationData.mOrderByValue );
      valueRelationAllowMulti->setChecked( mValueRelationData.mAllowMulti );
      valueRelationFilterExpression->setText( mValueRelationData.mFilterExpression );
      break;

    case QgsVectorLayer::Calendar:
      leDateFormat->setText( mDateFormat );
      break;

    case QgsVectorLayer::Photo:
    case QgsVectorLayer::WebView:
      sbWidgetWidth->setValue( mWidgetSize.width() );
      sbWidgetHeight->setValue( mWidgetSize.height() );
      break;

    case QgsVectorLayer::LineEdit:
    case QgsVectorLayer::UniqueValues:
    case QgsVectorLayer::Classification:
    case QgsVectorLayer::CheckBox:
    case QgsVectorLayer::FileName:
    case QgsVectorLayer::Enumeration:
    case QgsVectorLayer::Immutable:
    case QgsVectorLayer::Hidden:
    case QgsVectorLayer::TextEdit:
    case QgsVectorLayer::UuidGenerator:
    case QgsVectorLayer::Color:
    case QgsVectorLayer::EditorWidgetV2:
      break;
  }
}

void QgsAttributeTypeDialog::setPage( int index )
{
  selectionListWidget->setCurrentRow( index );
  setStackPage( index );
}

void QgsAttributeTypeDialog::setStackPage( int index )
{
  bool okDisabled = false;

  switch ( index )
  {
    case 2:
      stackedWidget->setCurrentIndex( 2 );
      break;
    case 6:
    {
      QStringList list;
      mLayer->dataProvider()->enumValues( mIndex, list );
      if ( list.size() == 0 )
      {
        okDisabled = true;
        enumerationWarningLabel->setText( tr( "Enumeration is not available for this attribute" ) );
      }
      else
      {
        enumerationWarningLabel->setText( "" );
      }
      stackedWidget->setCurrentIndex( 6 );
    }
    break;
    case 14:
      pictureOrUrlLabel->setText( tr( "Field contains a filename for a picture" ) );
      stackedWidget->setCurrentIndex( 14 );
      break;
    case 15:
      pictureOrUrlLabel->setText( tr( "Field contains an URL" ) );
      stackedWidget->setCurrentIndex( 14 );
      break;
    case 16:
      stackedWidget->setCurrentIndex( 15 );
      break;
    default:
      if ( selectionListWidget->item( index )->data( Qt::UserRole ).isNull() )
      {
        stackedWidget->setCurrentIndex( index );
      }
      else
      {
        QString factoryId = selectionListWidget->item( index )->data( Qt::UserRole ).toString();

        // Set to (empty) editor widget page
        stackedWidget->setCurrentIndex( 16 );

        // hide any other config widget
        Q_FOREACH( QgsEditorConfigWidget* wdg, mEditorConfigWidgets.values() )
        {
          wdg->hide();
        }

        if ( mEditorConfigWidgets.contains( factoryId ) )
        {
          mEditorConfigWidgets[factoryId]->show();
        }
        else
        {
          QgsEditorConfigWidget* cfgWdg = QgsEditorWidgetRegistry::instance()->createConfigWidget( factoryId, mLayer, mIndex, this );

          if ( cfgWdg )
          {
            cfgWdg->setConfig( mWidgetV2Config );
            pageEditorWidget->layout()->addWidget( cfgWdg );

            mEditorConfigWidgets.insert( factoryId, cfgWdg );
          }
        }
      }
      break;
  }

  stackedWidget->currentWidget()->setDisabled( okDisabled );
  buttonBox->button( QDialogButtonBox::Ok )->setDisabled( okDisabled );
}

void QgsAttributeTypeDialog::accept()
{
  //store data to output variables
  mFieldEditable = isFieldEditableCheckBox->isChecked();

  switch ( selectionListWidget->currentRow() )
  {
    default:
    case 0:
      mEditType = QgsVectorLayer::LineEdit;
      break;
    case 1:
      mEditType = QgsVectorLayer::Classification;
      break;
    case 2:
    case 3:
      if ( editableUniqueValues->isChecked() )
      {
        mEditType = QgsVectorLayer::UniqueValuesEditable;
      }
      else
      {
        mEditType = QgsVectorLayer::UniqueValues;
      }
      break;
    case 4:
      mEditType = QgsVectorLayer::FileName;
      break;
    case 5:
      mEditType = QgsVectorLayer::ValueMap;
      break;
    case 6:
      mEditType = QgsVectorLayer::Enumeration;
      break;
    case 7:
      mEditType = QgsVectorLayer::Immutable;
      break;
    case 8:
      mEditType = QgsVectorLayer::Hidden;
      break;
    case 9:
      mEditType = QgsVectorLayer::CheckBox;
      break;
    case 10:
      mEditType = QgsVectorLayer::TextEdit;
      break;
    case 11:
      mEditType = QgsVectorLayer::Calendar;
      mDateFormat = leDateFormat->text();
      break;
    case 12:
      mEditType = QgsVectorLayer::ValueRelation;
      mValueRelationData.mLayer = valueRelationLayer->itemData( valueRelationLayer->currentIndex() ).toString();
      mValueRelationData.mKey = valueRelationKeyColumn->currentText();
      mValueRelationData.mValue = valueRelationValueColumn->currentText();
      mValueRelationData.mAllowNull = valueRelationAllowNull->isChecked();
      mValueRelationData.mOrderByValue = valueRelationOrderByValue->isChecked();
      mValueRelationData.mAllowMulti = valueRelationAllowMulti->isChecked();
      mValueRelationData.mFilterExpression = valueRelationFilterExpression->toPlainText();
      break;
    case 13:
      mEditType = QgsVectorLayer::UuidGenerator;
      break;
    case 14:
      mEditType = QgsVectorLayer::Photo;
      mWidgetSize = QSize( sbWidgetWidth->value(), sbWidgetHeight->value() );
      break;
    case 15:
      mEditType = QgsVectorLayer::WebView;
      mWidgetSize = QSize( sbWidgetWidth->value(), sbWidgetHeight->value() );
      break;
    case 16:
      mEditType = QgsVectorLayer::Color;
      break;
    case 17:
      mEditType = QgsVectorLayer::EditorWidgetV2;
      break;
  }

  QDialog::accept();
}

QString QgsAttributeTypeDialog::defaultWindowTitle()
{
  return tr( "Attribute Edit Dialog" );
}

void QgsAttributeTypeDialog::updateLayerColumns( int idx )
{
  valueRelationKeyColumn->clear();
  valueRelationValueColumn->clear();

  QString id = valueRelationLayer->itemData( idx ).toString();

  QgsVectorLayer *vl = qobject_cast< QgsVectorLayer *>( QgsMapLayerRegistry::instance()->mapLayer( id ) );
  if ( !vl )
    return;

  const QgsFields &fields = vl->pendingFields();
  for ( int idx = 0; idx < fields.count(); ++idx )
  {
    QString fieldName = fields[idx].name();
    valueRelationKeyColumn->addItem( fieldName );
    valueRelationValueColumn->addItem( fieldName );
  }

  valueRelationKeyColumn->setCurrentIndex( valueRelationKeyColumn->findText( mValueRelationData.mKey ) );
  valueRelationValueColumn->setCurrentIndex( valueRelationValueColumn->findText( mValueRelationData.mValue ) );
}
