/***************************************************************************
    qgsproviderexpressiontranslator.h
     --------------------------------------
    Date                 : 18.5.2013
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

#ifndef QGSEXPRESSIONPROVIDERTRANSLATOR_H
#define QGSEXPRESSIONPROVIDERTRANSLATOR_H

#include "qgsexpression.h"

class CORE_EXPORT QgsExpressionProviderTranslator
{
  public:
    virtual ~QgsExpressionProviderTranslator();

    virtual bool translateNodeUnaryOperator( QString& result, const QgsExpression::NodeUnaryOperator* op ) = 0;
    virtual bool translateNodeColumnRef( QString& result, const QgsExpression::NodeColumnRef* op ) = 0;
};

#endif // QGSPROVIDEREXPRESSIONTRANSLATOR_H
