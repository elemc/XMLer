// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QXmlDefaultHandler)*/
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerHandler
   Description:
*/
#include "xmlerhandler.h"

XMLerHandler::XMLerHandler ():
  QXmlDefaultHandler()
{
  _document = 0;
  current_parent = 0;
  current_chars.clear();
}

XMLerHandler::~XMLerHandler ()
{
}

bool XMLerHandler::startDocument()
{
  _document = new DocumentXMLNode();
  current_parent = _document;
  return true;
}
bool XMLerHandler::endDocument()
{
  return true;
}

bool XMLerHandler::startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts )
{
  ElementXMLNode *node = new ElementXMLNode( current_parent );
  node->setNamespaceURI ( namespaceURI );
  node->setLocalName ( localName );
  node->setQName ( qName );
  node->appendAttributes ( atts );

  current_parent->appendChild(node);

  DataXMLNode *data = new DataXMLNode();

  current_parent = node;
  current_chars[current_parent] = data;

  return true;
}
bool XMLerHandler::endElement ( const QString & namespaceURI, const QString & localName, const QString & qName )
{
  DataXMLNode *current_data = current_chars[current_parent];
  if ( !current_data->data().isEmpty() ) {
    current_data->setParentNode(current_parent);
    current_parent->appendChild (current_data);
  }
  else {
    delete current_data;
    current_data = 0;
  }

  current_chars.remove(current_parent);
  current_parent = current_parent->parentNode();
  return true;
}
bool XMLerHandler::characters ( const QString & ch )
{
  if ( !current_chars[current_parent] )
    return false;

  QString alldata = current_chars[current_parent]->data();
  alldata += ch;
  current_chars[current_parent]->setData ( alldata );
}
bool XMLerHandler::error ( const QXmlParseException & exception )
{
  emit emitException ( XMLerHandler::Error, exception );
  return true;
}
bool XMLerHandler::fatalError ( const QXmlParseException & exception )
{
  emit emitException ( XMLerHandler::FatalError, exception );
  return false;
}
bool XMLerHandler::warning ( const QXmlParseException & exception )
{
  emit emitException ( XMLerHandler::Warning, exception );
  return true;
}

void XMLerHandler::emitException( XMLerHandler::Exceptions e, QXmlParseException exception )
{
  //emit xmlException ( e, exception.columnNumber(), exception.lineNumber(), exception.message() );
}
DocumentXMLNode *XMLerHandler::document () const
{
  return _document;
}
