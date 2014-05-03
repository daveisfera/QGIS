/***************************************************************************
    qgscalendarwidget.cpp
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

#include "qgscalendarwidget.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDialog>
#include "qgsfieldvalidator.h"

QgsCalendarWidget::QgsCalendarWidget( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent )
    :  QgsEditorWidgetWrapper( vl, fieldIdx, editor, parent )
{
}

void QgsCalendarWidget::selectDate()
{
  QDialog *dlg = new QDialog();
  dlg->setWindowTitle( tr( "Select a date" ) );
  QVBoxLayout *layout = new QVBoxLayout( dlg );

  const QgsFieldValidator *v = dynamic_cast<const QgsFieldValidator *>( mLineEdit->validator() );
  QString dateFormat = v ? v->dateFormat() : "yyyy-MM-dd";

  QCalendarWidget *cw = new QCalendarWidget( dlg );
  QString prevValue = mLineEdit->text();
  cw->setSelectedDate( QDate::fromString( prevValue, dateFormat ) );
  layout->addWidget( cw );

  QDialogButtonBox *buttonBox = new QDialogButtonBox( dlg );
  buttonBox->addButton( QDialogButtonBox::Ok );
  buttonBox->addButton( QDialogButtonBox::Cancel );
  layout->addWidget( buttonBox );

  connect( buttonBox, SIGNAL( accepted() ), dlg, SLOT( accept() ) );
  connect( buttonBox, SIGNAL( rejected() ), dlg, SLOT( reject() ) );

  if ( dlg->exec() == QDialog::Accepted )
  {
    QString newValue = cw->selectedDate().toString( dateFormat );
    mLineEdit->setText( newValue );
  }
}

QVariant QgsCalendarWidget::value()
{
  QVariant v;

  if ( mCalendarWidget )
    v = mCalendarWidget->selectedDate();

  return v;
}

QWidget* QgsCalendarWidget::createWidget(QWidget* parent)
{
  return new QCalendarWidget( parent );
}

void QgsCalendarWidget::initWidget( QWidget* editor )
{
  mCalendarWidget = qobject_cast<QCalendarWidget*>( editor );
  mLineEdit = qobject_cast<QLineEdit*>( editor );

  mLineEdit = qobject_cast<QLineEdit*>( editor );
  if ( !mLineEdit )
  {
    mLineEdit = editor->findChild<QLineEdit*>();
  }

  mPushButton = editor->findChild<QPushButton*>();

  if ( mPushButton )
  {
    mPushButton->setToolTip( tr( "Select date in calendar" ) );
    connect( mPushButton, SIGNAL( clicked() ), this, SLOT( selectDate() ) );
  }

  if ( mLineEdit )
    mLineEdit->setValidator( new QgsFieldValidator(mLineEdit, field(), config( "DateFormat" ).toString() ) );
}

void QgsCalendarWidget::setValue(const QVariant& value)
{
  if ( mCalendarWidget )
    mCalendarWidget->setSelectedDate( value.toDate() );
}
