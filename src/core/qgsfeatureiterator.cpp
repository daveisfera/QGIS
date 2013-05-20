/***************************************************************************
    qgsfeatureiterator.cpp
    ---------------------
    begin                : Juli 2012
    copyright            : (C) 2012 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qgsfeatureiterator.h"
#include "qgslogger.h"

QgsAbstractFeatureIterator::QgsAbstractFeatureIterator( const QgsFeatureRequest& request )
    : mRequest( request )
    , mClosed( false )
    , refs( 0 )
{
}

QgsAbstractFeatureIterator::~QgsAbstractFeatureIterator()
{
}

void QgsAbstractFeatureIterator::initializePostFilter( const QgsFields& fields )
{
  if ( mRequest.filterType() == QgsFeatureRequest::FilterExpression )
  {
    mRequest.filterExpression()->prepare( fields );

    if ( mRequest.filterExpression()->hasParserError() )
    {
      QgsDebugMsg( QString( "Parser error: '%1'" ).arg( mRequest.filterExpression()->parserErrorString() ) );
    }
  }
}

bool QgsAbstractFeatureIterator::postFilter( const QgsFeature& f )
{

}

void QgsAbstractFeatureIterator::ref()
{
  refs++;
}

void QgsAbstractFeatureIterator::deref()
{
  refs--;
  if ( !refs )
    delete this;
}

///////

QgsFeatureIterator& QgsFeatureIterator::operator=( const QgsFeatureIterator & other )
{
  if ( this != &other )
  {
    if ( mIter )
      mIter->deref();
    mIter = other.mIter;
    if ( mIter )
      mIter->ref();
  }
  return *this;
}
