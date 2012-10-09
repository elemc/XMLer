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

#include "config.h"

#include <QtXml/QXmlDefaultHandler>
#include <QtCore/QMap>
#include "basexmlnode.h"
#include "documentxmlnode.h"
#include "elementxmlnode.h"
#include "dataxmlnode.h"
/* #include "commentxmlnode.h" */
#include "xmlerexception.h"

class XMLerHandler : public QXmlDefaultHandler
{
public:
  explicit XMLerHandler();
  ~XMLerHandler();

  bool startDocument();
  bool endDocument();

  bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
  bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
  bool characters ( const QString & ch );
  /* bool comment ( const QString & ch ); */
  bool startPrefixMapping ( const QString & prefix, const QString & uri );
  bool endPrefixMapping ( const QString & prefix );

  bool processingInstruction ( const QString & target, const QString & data );

  bool error ( const QXmlParseException & exception );
  bool warning ( const QXmlParseException & exception );
  bool fatalError ( const QXmlParseException & exception );

  /* self */
  DocumentXMLNode *document() const;
  bool hasExceptions () const;
  bool hasWarnings () const;
  bool hasErrors () const;
  bool hasFatalErrors () const;

  XMLerExceptionList exceptions () const;
  
private:
  DocumentXMLNode *_document;
  BaseXMLNode *current_parent;
  QMap<BaseXMLNode *, DataXMLNode *> current_chars;
  QMap<QString, QString> current_prefix_mapping;
  /* QMap<BaseXMLNode *, CommentXMLNode *> current_comment; */
  XMLerExceptionList _exceptions;

  bool hasTypedException ( XMLer::ExceptionType et ) const;
  void setDocumentPI( const QString &data );
};

#endif
