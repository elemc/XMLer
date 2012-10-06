// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QThread)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerLoadFileThread
   Description:
*/
#ifndef XMLERLOADFILETHREAD_H
#define XMLERLOADFILETHREAD_H

#include <QtCore/QThread>
#include <QtXml/QXmlSimpleReader>
#include <QtXml/QXmlParseException>
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlStreamReader>
#include "xmlerhandler.h"
#include "xmlerexception.h"
#include "xmlerinputsource.h"
#include "documentxmlnode.h"

class XMLerLoadFileThread : public QThread
{
  Q_OBJECT
public:
  explicit XMLerLoadFileThread(QObject *parent = 0);
  ~XMLerLoadFileThread();

  void run ();
  void setFileName ( const QString &fn );
  QString fileName () const;
  
private:
  QString _fileName;
  QMap<QString, QString> _info;
  XMLerHandler *handler;

  QMap<QString, QString> getInformationFromFile ();
  void checkExceptionInHandler ();
  QString progressMessage () const;

signals:
  void beginProgress ( QString process, qint64 totalSize );
  void progress ( qint64 pos );
  void endProgress ();
  void loadDone ( DocumentXMLNode *document );
  void error ( QString message );
  void parseException ( XMLerException::ExceptionType mainType, XMLerExceptionList exceptions );

public slots:
  void on_beginProgress ( qint64 totalSize );
  void on_progress ( qint64 pos );
  void on_endProgress ( );
  
};

#endif
