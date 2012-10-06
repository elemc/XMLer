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
  codec = 0;

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
bool DocumentXMLNode::save ( const QString &fileName )
{
  QFile xml ( fileName );

  if ( !xml.open ( QIODevice::WriteOnly ) )
    return false;

  QXmlStreamWriter writer( &xml );

  /* set information about document */
  writer.setAutoFormatting ( _document_auto_formatting );
  writer.setAutoFormattingIndent ( _document_formatting_indent );
  writer.setCodec ( codec );

  bool result = true;

  /* save a document */
  writer.writeStartDocument( _version );
  
  if ( rootNode )
    result &= saveNode ( writer, rootNode );

  writer.writeEndDocument();

  xml.close();

  return result;
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
  codec = codec_in;
}
void DocumentXMLNode::setVersion ( const QString & version )
{
  _version = version;
}

/* private self */
bool DocumentXMLNode::saveNode ( QXmlStreamWriter &writer, BaseXMLNode *node )
{
  bool result = true;

  if ( ElementXMLNode *element = qobject_cast<ElementXMLNode *>(node) ) {
    if ( element->namespaceURI().isEmpty () )
      writer.writeStartElement ( element->qName() );
    else
      writer.writeStartElement ( element->namespaceURI(), element->name() );

    XMLNodePtrList chlist = element->childs();
    XMLNodePtrList::iterator it;

    for ( it = chlist.begin(); it != chlist.end(); ++it ) {
      BaseXMLNode *child_node = (*it);
      result &= saveNode ( writer, child_node );
    }

    writer.writeEndElement();
  }
  else if ( AttrXMLNode *attr = qobject_cast<AttrXMLNode *>(node) ) {
    if ( attr->namespaceURI().isEmpty() )
      writer.writeAttribute ( attr->qName(), attr->value() );
    else
      writer.writeAttribute ( attr->namespaceURI(), attr->name(), attr->value() );
  }
  else if ( DataXMLNode *data = qobject_cast<DataXMLNode *>(node) ) {
    writer.writeCharacters ( data->data() );
  }

  return result;
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
