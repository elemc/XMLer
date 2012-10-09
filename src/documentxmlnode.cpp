// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (BaseXMLNode)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: DocumentXMLNode
   Description:
*/
#include "documentxmlnode.h"

DocumentXMLNode::DocumentXMLNode ():
  BaseXMLNode(0)
{
  setNodeType(BaseXMLNode::Document);
  rootNode = 0;
  setCodec ( DEFAULT_ENCODING );
  setVersion ( "1.0" );

  _document_auto_formatting = true;
  _document_formatting_indent = 4;
}

DocumentXMLNode::~DocumentXMLNode ()
{
  if ( rootNode )
    delete rootNode;
}
/* self */
BaseXMLNode *DocumentXMLNode::documentNode () const
{
  return rootNode;
}
void DocumentXMLNode::setFileName ( const QString &name )
{
  _fileName = name;
}
QString DocumentXMLNode::fileName () const
{
  return _fileName;
}
void DocumentXMLNode::setCodec ( const QString &codecName )
{
  QByteArray cname = codecName.toLatin1();
  if ( !QTextCodec::availableCodecs().contains( cname ) )
    return;

  setCodec ( QTextCodec::codecForName ( cname ) );
}
void DocumentXMLNode::setCodec ( QTextCodec * codec_in )
{
  if ( codec_in == 0 ) {
    _codec = DEFAULT_ENCODING;
    return;
  }
  _codec = codec_in;
}
void DocumentXMLNode::setVersion ( const QString & version )
{
  _version = version;
}
bool DocumentXMLNode::autoFormatting () const
{
  return _document_auto_formatting;
}
int DocumentXMLNode::formattingIndent () const
{
  return _document_formatting_indent;
}
QTextCodec *DocumentXMLNode::codec () const
{
  return _codec;
}
QString DocumentXMLNode::version () const
{
  return _version;
}
void DocumentXMLNode::setFormatting ( bool formatting, int indent )
{
  _document_auto_formatting = formatting;
  _document_formatting_indent = indent;
}
void DocumentXMLNode::appendPI ( const QString &target, const QString &data )
{
  _processingInstructions.insert ( target, data );
}
QMap<QString,QString> DocumentXMLNode::processingInstructions () const 
{
  return _processingInstructions;
}
bool DocumentXMLNode::hasPI () const
{
  return ( _processingInstructions.size() != 0 );
}
void DocumentXMLNode::setPI ( const QMap<QString, QString> &pi )
{
  _processingInstructions.clear();
  _processingInstructions = pi;
}

/* Virtuals */
void DocumentXMLNode::appendChild(BaseXMLNode *child)
{
  rootNode = child;
}
quint32 DocumentXMLNode::childCount() const
{
  if ( rootNode )
    return 1;
  return 0;
}
XMLNodePtrList DocumentXMLNode::childs() const
{
  XMLNodePtrList list;
  if ( rootNode )
    list.append(rootNode);
  return list;
}
QString DocumentXMLNode::name () const
{
  return tr("XML Document");
}
QString DocumentXMLNode::qName () const
{
  return QString();
}
QString DocumentXMLNode::namespaceURI () const
{
  return QString();
}
