// -*- C++ -*-
/* ------------------------------------------- */
/* C++ class source (QObject, QXmlInputSource) */
/* Author: Alexei Panov <me@elemc.name>        */
/* ------------------------------------------- */
/* Name: XMLerInputSource
   Description:
*/
#include "xmlerinputsource.h"

XMLerInputSource::XMLerInputSource (QIODevice *device, QObject *parent):
  QObject(parent),
  QXmlInputSource(device)
{
  _data_size = device->size();
  _data_pos = 0;
  _step = _data_size / 100;

  if ( _step == 0 )
    _step = 1;
}

XMLerInputSource::~XMLerInputSource ()
{
}

QChar XMLerInputSource::next ()
{
  if ( _data_pos == 0 )
    emit beginRead ( _data_size );
  
  QChar result = QXmlInputSource::next();
  if ( result == QXmlInputSource::EndOfData )
    return result;
  else if ( result == QXmlInputSource::EndOfDocument ) {
    emit readProgress ( _data_pos );
    emit endRead ();    
    return result;
  }

  if ( result.toLatin1() == '\0' )
    _data_pos += 2;
  else
    _data_pos += 1;

  if ( _data_pos % _step == 0 )
    emit readProgress ( _data_pos );
  
  return result;
}

