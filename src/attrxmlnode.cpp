// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QObject)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: AttrXMLNode
   Description:
*/
#include "attrxmlnode.h"

AttrXMLNode::AttrXMLNode (BaseXMLNode *parent):
  BaseXMLNode(parent)
{
  setNodeType(BaseXMLNode::Attribute);
  _data = 0;
}

AttrXMLNode::~AttrXMLNode ()
{
  if ( _data )
    delete _data;
}
void AttrXMLNode::setNamespaceURI( const QString &uri )
{
  _namespaceURI = uri;
}
void AttrXMLNode::setLocalName( const QString &ln )
{
  _localName = ln;
}
void AttrXMLNode::setQName( const QString &qn )
{
  _qName = qn;
}

quint32 AttrXMLNode::childCount() const
{
  if ( _data )
    return 1;

  return 0;
}
XMLNodePtrList AttrXMLNode::childs() const
{
  XMLNodePtrList list;
  if ( _data )
    list.append( _data );
  return list;
}
void AttrXMLNode::appendChild(BaseXMLNode *child)
{
  if ( DataXMLNode *d = qobject_cast<DataXMLNode *>(child) ) 
    _data = d;
}
QString AttrXMLNode::name () const
{
  return _localName;
}
QString AttrXMLNode::qName () const
{
  return _qName;
}
QString AttrXMLNode::namespaceURI () const
{
  return _namespaceURI;
}

/* self */
QString AttrXMLNode::value () const
{
  if ( _data )
    return _data->data();

  return QString();
}
