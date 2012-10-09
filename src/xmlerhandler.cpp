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
  /* CommentXMLNode *comment = new CommentXMLNode(); */

  current_parent = node;
  current_chars[current_parent] = data;
  /* current_comment[current_parent] = comment; */

  /* prefix mapping */
  if ( current_prefix_mapping.size() != 0 ) {
    node->appendMapping ( current_prefix_mapping );
    current_prefix_mapping.clear();
  }

  return true;
}
bool XMLerHandler::endElement ( const QString & namespaceURI, const QString & localName, const QString & qName )
{
  DataXMLNode *current_data = current_chars[current_parent];
  if ( !current_data->data().trimmed().isEmpty() ) {
    current_data->setParentNode(current_parent);
    current_parent->appendChild (current_data);
  }
  else {
    delete current_data;
    current_data = 0;
  }

  current_chars.remove(current_parent);
  /* current_comment.remove(current_parent); */
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
  return true;
}
/*
bool XMLerHandler::comment ( const QString & ch )
{
  qDebug() << ch;
  if ( !current_comment[current_parent] ) 
    return false;

  QString allcomment = current_comment[current_parent]->data();
  allcomment += ch;
  current_comment[current_parent]->setData ( allcomment );
  return true;
}
*/
bool XMLerHandler::error ( const QXmlParseException & exception )
{
  XMLerException err_exc( XMLer::Error, exception );
  _exceptions.append ( err_exc );
  return true;
}
bool XMLerHandler::fatalError ( const QXmlParseException & exception )
{
  XMLerException fe_exc( XMLer::FatalError, exception );
  _exceptions.append ( fe_exc );
  return false;
}
bool XMLerHandler::warning ( const QXmlParseException & exception )
{
  XMLerException warn_exc( XMLer::Warning, exception );
  _exceptions.append ( warn_exc );
  return true;
}
bool XMLerHandler::startPrefixMapping ( const QString & prefix, const QString & uri )
{
  current_prefix_mapping.insert ( prefix, uri );
}
bool XMLerHandler::endPrefixMapping ( const QString & prefix )
{
}
bool XMLerHandler::processingInstruction ( const QString & target, const QString & data )
{
  //qDebug() << target << data;
  if ( target == QString("xml") )
    setDocumentPI ( data );

  else if ( _document )
    _document->appendPI ( target, data );
}

/* self */
DocumentXMLNode *XMLerHandler::document () const
{
  return _document;
}
bool XMLerHandler::hasExceptions () const
{
  return ( _exceptions.size () > 0 );
}
bool XMLerHandler::hasWarnings () const
{
  return hasTypedException ( XMLer::Warning );
}
bool XMLerHandler::hasErrors () const
{
  return hasTypedException ( XMLer::Error );
}
bool XMLerHandler::hasFatalErrors () const
{
  return hasTypedException ( XMLer::FatalError );
}
XMLerExceptionList XMLerHandler::exceptions () const
{
  return _exceptions;
}

/* private self */
bool XMLerHandler::hasTypedException ( XMLer::ExceptionType et ) const
{
  if ( _exceptions.size() == 0 )
    return false;

  bool result = false;

  QList<XMLerException>::const_iterator it;
  for ( it = _exceptions.begin(); it != _exceptions.end(); ++it ) {
    if ( (*it).exceptionType() == et ) {
      result = true;
      break;
    }
  }

  return result;
}
void XMLerHandler::setDocumentPI ( const QString &data )
{
  QStringList list = data.split(' ', QString::SkipEmptyParts);
  for ( int i = 0; i < list.size(); ++i ) {
    QString key = list.at(i).section("=", 0, 0);
    QString value = list.at(i).section("=", 1, 1);
    value = value.replace( QRegExp("['\"]"), "" );

    if ( key == "version" )
      _document->setVersion ( value );
    else if ( key == "encoding" )
      _document->setCodec ( value );
  }
}
