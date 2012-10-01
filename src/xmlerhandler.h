// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QXmlDefaultHandler)*/
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerHandler
   Description: This is reimplement handler for XMLer
*/
#ifndef XMLERHANDLER_H
#define XMLERHANDLER_H

#include <QtXml/QXmlDefaultHandler>
#include <QtCore/QMap>
#include "basexmlnode.h"
#include "documentxmlnode.h"
#include "elementxmlnode.h"
#include "dataxmlnode.h"

class XMLerHandler : public QXmlDefaultHandler
{
  //  Q_OBJECT
public:
  enum Exceptions { Error, FatalError, Warning };

  explicit XMLerHandler();
  ~XMLerHandler();

  bool startDocument();
  bool endDocument();

  bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
  bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
  bool characters ( const QString & ch );

  bool error ( const QXmlParseException & exception );
  bool warning ( const QXmlParseException & exception );
  bool fatalError ( const QXmlParseException & exception );

  DocumentXMLNode *document() const;
  
private:
  DocumentXMLNode *_document;
  BaseXMLNode *current_parent;
  QMap<BaseXMLNode *, DataXMLNode *> current_chars;

  void emitException( XMLerHandler::Exceptions e, QXmlParseException exception );
  
  //signals:
  //void xmlException( XMLerHandler::Exceptions e, int columnNumber, int lineNumber, QString message );
};

#endif
