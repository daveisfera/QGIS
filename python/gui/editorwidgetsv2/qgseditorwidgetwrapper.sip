/***************************************************************************
    qgseditorwidgetwrapper.sip
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

class QgsEditorWidgetWrapper : public QObject
{
%TypeHeaderCode
#include <editorwidgetsv2/qgseditorwidgetwrapper.h>
%End

  public:
    explicit QgsEditorWidgetWrapper( QObject *parent = 0 );
    virtual QWidget* widget( QWidget* parent ) = 0;
    virtual void setConfig( QMap<QString, QVariant> config );
    virtual const QVariant& value() = 0;
    QVariant config( QString key );

  signals:
    void valueChanged( const QVariant& value );
    
  public slots:
    virtual void setValue( const QVariant& value ) = 0;
};
