/***************************************************************************
    qgsattributeform.h
     --------------------------------------
    Date                 : 3.5.2014
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

#ifndef QGSATTRIBUTEFORM_H
#define QGSATTRIBUTEFORM_H

#include "qgsfeature.h"
#include "qgsvectorlayer.h"
#include "qgseditorwidgetwrapper.h"
#include "qgsattributeeditorcontext.h"

#include <QWidget>

class GUI_EXPORT QgsAttributeForm : public QWidget
{
    Q_OBJECT

  public:
    explicit QgsAttributeForm( QgsVectorLayer* vl, const QgsFeature feature = QgsFeature(), QgsAttributeEditorContext context = QgsAttributeEditorContext(), QWidget *parent = 0 );

  signals:
    void attributeChanged( QString attribute, const QVariant& value );

  public slots:
    void changeAttribute( const QString& field, const QVariant& value );
    void setFeature( const QgsFeature& feature );
    bool save();

  private slots:
    void onAttributeChanged( const QVariant& value );
    void synchronizeEnabledState();

  private:
    void init();

    QWidget* createWidgetFromDef( const QgsAttributeEditorElement* widgetDef, QWidget* parent, QgsVectorLayer* vl, QgsAttributeEditorContext& context, QString& labelText, bool& labelOnTop );

    /**
     * Creates widget wrappers for all suitable widgets found.
     * Called once maximally.
     */
    void createWrappers();
    void connectWrappers();

    QgsVectorLayer* mLayer;
    QgsFeature mFeature;
    QList<QgsEditorWidgetWrapper*> mWidgets;
    QgsAttributeEditorContext mContext;

    //! Set to true while saving to prevent recursive saves
    bool mIsSaving;
};

#endif // QGSATTRIBUTEFORM_H
