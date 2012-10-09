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
  XMLerFileOperThread(parent)
{
  handler = new XMLerHandler;
}
XMLerLoadFileThread::~XMLerLoadFileThread ()
{
  delete handler;
}

/* virtual */
void XMLerLoadFileThread::run ()
{
  QFile xml( fileName() );
  if ( !xml.exists() ) {
    emit error ( tr("File %1 does not exists.").arg( fileName() ) );
    return;
  }

  /* QMap<QString, QString> info = getInformationFromFile(); */

  QXmlSimpleReader reader;
  XMLerInputSource *source = new XMLerInputSource ( &xml );

  /* connect source to slots in model */
  connect ( source, SIGNAL(beginRead(qint64)), this, SLOT(on_beginProgress(qint64)) );
  connect ( source, SIGNAL(readProgress(qint64)), this, SLOT(on_progress(qint64)) );
  connect ( source, SIGNAL(endRead()), this, SLOT(on_endProgress()) );

  reader.setContentHandler ( handler );
  reader.setErrorHandler ( handler );

  bool parseResult = reader.parse ( source, true );

  /* FIXME: this is partial read */
  if ( parseResult ) {
    bool partResult = parseResult;
    while ( partResult )
      partResult = reader.parseContinue();
  }
  if ( !parseResult ) {
    checkExceptionInHandler();
    on_endProgress();
    return;
  }

  /* set addition data (information) in document */
  if ( handler->document() )
    handler->document()->setFileName( fileName() );
  /* CLEANIT
  if ( !info.isEmpty() ) {
    if ( info.contains ( "encoding" ) )
      handler->document()->setCodec ( info.value ( "encoding" ) );
    if ( info.contains ( "version" ) )
      handler->document()->setVersion ( info.value ( "version" ) );
      }*/
  emit done ( handler->document() );
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

  QFile xml_file( fileName() );
  if ( !xml_file.exists() ) {
    emit error ( tr("File %1 does not exist.").arg( fileName() ) ); 
    return result;
  }

  if ( !xml_file.open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
    emit error ( tr("Can not open file %1.").arg( fileName() ) ); 
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

  XMLer::ExceptionType mt = XMLer::Warning;
  if ( handler->hasFatalErrors () )
    mt = XMLer::FatalError;
  else if ( handler->hasErrors () )
    mt = XMLer::Error;

  QStringList messages;
  XMLerExceptionList ex = handler->exceptions();
  XMLerExceptionList::iterator it;
  for ( it = ex.begin(); it != ex.end(); ++it ) {
    messages.append ( (*it).printMessage() );
  }

  if ( mt == XMLer::Warning )
    emit warning ( messages.join("\n") );
  else
    emit error ( messages.join("\n") );
}
QString XMLerLoadFileThread::progressMessage () const
{
  QString msg = tr("%1 file loaded...").arg( fileName() );;
  return msg;
}

