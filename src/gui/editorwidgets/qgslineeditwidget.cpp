/***************************************************************************
    qgslineeditwidget.cpp
     --------------------------------------
    Date                 : 5.1.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias dot kuhn at gmx dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgslineeditwidget.h"

#include "qgsfieldvalidator.h"
#include "qgsvectorlayer.h"

QgsLineEditWidget::QgsLineEditWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent )
    :  QgsEditorWidgetWrapper( vl, fieldIdx, editor, parent )
{
}

QVariant QgsLineEditWidget::value()
{
  QVariant text;

  if ( mLineEdit )
    text = mLineEdit->text();

  if ( mTextEdit )
    text = mTextEdit->toHtml();

  if ( mPlainTextEdit )
    text = mPlainTextEdit->toPlainText();

  if ( mComboBox )
    text = mComboBox->currentText();

  return text;
}

QWidget*QgsLineEditWidget::createWidget( QWidget* parent )
{
  return new QLineEdit( parent );
}

void QgsLineEditWidget::initWidget( QWidget* editor )
{
  mLineEdit = qobject_cast<QLineEdit*>( editor );
  mTextEdit = qobject_cast<QTextEdit*>( editor );
  mPlainTextEdit = qobject_cast<QPlainTextEdit *>( editor );
  mComboBox = qobject_cast<QComboBox *>( editor );

  if ( mLineEdit )
  {
    mLineEdit->setValidator( new QgsFieldValidator( mLineEdit, layer()->pendingFields()[fieldIdx()], layer()->dateFormat( fieldIdx() ) ) );
    connect( mLineEdit, SIGNAL( textChanged( QString ) ), this, SLOT( valueChanged( QString ) ) );
  }

  if ( mTextEdit )
  {
    mTextEdit->setAcceptRichText( true );
    connect( mTextEdit, SIGNAL( textChanged() ), this, SLOT( valueChanged() ) );
  }

  if ( mPlainTextEdit )
  {
    connect( mPlainTextEdit, SIGNAL( textChanged() ), this, SLOT( valueChanged() ) );
  }

  if ( mComboBox && mComboBox->isEditable() )
  {
    mComboBox->setValidator( new QgsFieldValidator( mLineEdit, layer()->pendingFields()[fieldIdx()], layer()->dateFormat( fieldIdx() ) ) );
    connect( mComboBox, SIGNAL( editTextChanged( QString ) ), this, SLOT( valueChanged( QString ) ) );
  }
}

void QgsLineEditWidget::setValue( const QVariant& value )
{
  if ( mLineEdit )
    mLineEdit->setText( value.toString() );

  if ( mTextEdit )
    mTextEdit->setHtml( value.toString() );

  if ( mPlainTextEdit )
    mPlainTextEdit->setPlainText( value.toString() );

  if ( mComboBox )
    mComboBox->setEditText( value.toString() );
}
