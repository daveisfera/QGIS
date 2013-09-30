/***************************************************************************
                             QgsLayerChooserwidget.cpp
                             -------------------------
    begin                : September 2013
    copyright            : (C) 2013 Denis Rouzaud
    email                : denis.rouzaud@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QGSLAYERCHOOSERCOMBO_H
#define QGSLAYERCHOOSERCOMBO_H

#include "qgslayerchooserwidget.h"


class GUI_EXPORT QgsLayerChooserCombo : public QgsLayerChooserWidget
{
    Q_OBJECT

  public:
    /** constructor */
    QgsLayerChooserCombo( QObject *parent = 0 );
    virtual ~QgsLayerChooserCombo()
    {}

  protected slots:
    void currentIndexChanged( int idx );

  private:
    QComboBox* mWidget;

    // QgsLayerChooserWidget interface
  public:
    virtual bool initWidget( QWidget* widget );
    virtual QgsMapLayer* getLayer() const;
    QString getLayerId() const;
    //using QgsLayerChooserWidget::setLayer;
    virtual void setLayer( QString layerid );
    virtual void setLayer( QgsMapLayer* layer );
    void clearWidget();
    void addLayer( QgsMapLayer* layer, DisplayStatus display );
};

#endif // QGSLAYERCHOOSERCOMBO_H
