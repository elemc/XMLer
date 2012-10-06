// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QThread)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerLoadFileThread
   Description:
*/
#include "xmlerloadfilethread.h"

XMLerLoadFileThread::XMLerLoadFileThread (QObject *parent):
  QThread(parent)
{
  handler = new XMLerHandler;
}
XMLerLoadFileThread::~XMLerLoadFileThread ()
{
  delete handler;
}

/* self */
void XMLerLoadFileThread::setFileName ( const QString &fn )
{
  _fileName = fn;
}
QString XMLerLoadFileThread::fileName () const
{
  return _fileName;
}

/* virtual */
void XMLerLoadFileThread::run ()
{
  QFile xml( _fileName );
  if ( !xml.exists() ) {
    emit error ( tr("File %1 does not exists.").arg(_fileName) );
    return;
  }

  QMap<QString, QString> info = getInformationFromFile();

  QXmlSimpleReader reader;
  XMLerInputSource *source = new XMLerInputSource ( &xml );

  /* connect source to slots in model */
  connect ( source, SIGNAL(beginRead(qint64)), this, SLOT(on_beginProgress(qint64)) );
  connect ( source, SIGNAL(readProgress(qint64)), this, SLOT(on_progress(qint64)) );
  connect ( source, SIGNAL(endRead()), this, SLOT(on_endProgress()) );

  reader.setContentHandler ( handler );
  reader.setErrorHandler ( handler );

  bool parseResult = reader.parse ( source );
  if ( !parseResult ) {
    checkExceptionInHandler();
    return;
  }

  /* set addition data (information) in document */
  handler->document()->setFileName( _fileName );
  if ( !info.isEmpty() ) {
    if ( info.contains ( "encoding" ) )
      handler->document()->setCodec ( info.value ( "encoding" ) );
    if ( info.contains ( "version" ) )
      handler->document()->setVersion ( info.value ( "version" ) );
  }
  emit loadDone ( handler->document() );
  checkExceptionInHandler ();

  /* clean */
  disconnect ( source, SIGNAL(beginRead(qint64)) );
  disconnect ( source, SIGNAL(readProgress(qint64)) );
  disconnect ( source, SIGNAL(endRead()) );
  delete source;
}

/* private self */
QMap<QString, QString> XMLerLoadFileThread::getInformationFromFile ( )
{
  QMap<QString, QString> result;

  QFile xml_file( _fileName );
  if ( !xml_file.exists() ) {
    emit error ( tr("File %1 does not exist.").arg(_fileName) ); 
    return result;
  }

  if ( !xml_file.open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
    emit error ( tr("Can not open file %1.").arg(_fileName) ); 
    return result;
  }

  QString encoding;
  QString version;

  QXmlStreamReader reader ( &xml_file );
  while ( !reader.atEnd() ) {
    QXmlStreamReader::TokenType tt = reader.readNext();
    if ( tt == QXmlStreamReader::StartDocument ) {
      encoding = reader.documentEncoding().toString();
      version = reader.documentVersion().toString();
      break;
    }
  }
  
  if ( !encoding.isEmpty() )
    result.insert("encoding", encoding);
  if ( !version.isEmpty() )
    result.insert("version", version);

  xml_file.close();

  return result;
}
void XMLerLoadFileThread::checkExceptionInHandler ()
{
  if ( !handler->hasExceptions () )
    return;

  XMLerException::ExceptionType mt = XMLerException::Warning;
  if ( handler->hasFatalErrors () )
    mt = XMLerException::FatalError;
  else if ( handler->hasErrors () )
    mt = XMLerException::Error;

  emit parseException ( mt, handler->exceptions() );
}
QString XMLerLoadFileThread::progressMessage () const
{
  QString msg = tr("%1 file loaded...").arg( _fileName );;
  return msg;
}

/* Slots */
void XMLerLoadFileThread::on_beginProgress ( qint64 totalSize )
{
  emit beginProgress ( progressMessage(), totalSize );
}
void XMLerLoadFileThread::on_progress ( qint64 pos )
{
  emit progress ( pos );
}
void XMLerLoadFileThread::on_endProgress ()
{
  emit endProgress ();
}
