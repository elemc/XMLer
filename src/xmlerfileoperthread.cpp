// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QThread)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerFileOperThread
   Description: Base class for threaded file operation classes
*/
#include "xmlerfileoperthread.h"

XMLerFileOperThread::XMLerFileOperThread (QObject *parent):
  QThread(parent)
{
}

XMLerFileOperThread::~XMLerFileOperThread ()
{
}

/* self */
void XMLerFileOperThread::setFileName ( const QString &fn )
{
  _fileName = fn;
}
QString XMLerFileOperThread::fileName () const
{
  return _fileName;
}

/* virtual */
void XMLerFileOperThread::run () 
{
  /* Nothing to do */
}

/* private self */
QString XMLerFileOperThread::progressMessage () const
{
  return QString();
}

/* Slots */
void XMLerFileOperThread::on_beginProgress ( qint64 totalSize )
{
  emit beginProgress ( progressMessage(), totalSize );
}
void XMLerFileOperThread::on_progress ( qint64 pos )
{
  emit progress ( pos );
}
void XMLerFileOperThread::on_endProgress ()
{
  emit endProgress ();
}
