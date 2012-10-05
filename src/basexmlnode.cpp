// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QObject)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: BaseXMLNode
   Description:
*/
#include "basexmlnode.h"

BaseXMLNode::BaseXMLNode (BaseXMLNode *parent):
  QObject(parent)
{
  setParentNode(parent);
  setNodeType(BaseXMLNode::Unknown);
}

BaseXMLNode::~BaseXMLNode ()
{
  XMLNodePtrList cl = childs();
  qDeleteAll(cl.begin(), cl.end());
}

/* self */
void BaseXMLNode::setParentNode(BaseXMLNode *p)
{
  _parentNode = p;
}
BaseXMLNode *BaseXMLNode::parentNode() const
{
  return _parentNode;
}
void BaseXMLNode::setNodeType(XMLNodeType nt)
{
  _nodeType = nt;
}
BaseXMLNode::XMLNodeType BaseXMLNode::nodeType() const
{
  return _nodeType;
}
int BaseXMLNode::childIndex( BaseXMLNode *child ) const
{
  if ( !child )
    return -1;
  return childs().indexOf ( child );
}
int BaseXMLNode::row ()
{
  if ( parentNode() )
    return parentNode()->childIndex( this );
  return -1;
}
QString BaseXMLNode::typeToStr() const
{
  switch ( nodeType() ) {
  case BaseXMLNode::Document:
    return tr("Document");
    break;
  case BaseXMLNode::Element:
    return tr("Element");
    break;
  case BaseXMLNode::Attribute:
    return tr("Attribute");
    break;
  case BaseXMLNode::Data:
    return tr("Data");
    break;
  default:
    return tr("Unknown type");
    break;
  }
}
QIcon BaseXMLNode::typeToIcon () const
{
  switch ( nodeType() ) {
  case BaseXMLNode::Element:
    return QIcon::fromTheme("code-context");
    break;
  case BaseXMLNode::Attribute:
    return QIcon::fromTheme("code-function");
    break;
  case BaseXMLNode::Data:
    return QIcon::fromTheme("code-variable");
    break;
  default:
    return QIcon();
    break;
  }
}
BaseXMLNode *BaseXMLNode::childItemAt( int index ) const
{
  XMLNodePtrList cl = childs();
  if ( index >= 0 && index < cl.size() )
    return cl.at( index );
  return 0;
}

/* Virtuals */
quint32 BaseXMLNode::childCount() const
{
  return 0;
}
XMLNodePtrList BaseXMLNode::childs() const
{
  return XMLNodePtrList(); // Return empty list
}
void BaseXMLNode::appendChild(BaseXMLNode *child)
{
  // do nothing
}
QString BaseXMLNode::name () const
{
  return QString(); // return empty string
}
QString BaseXMLNode::qName () const
{
  return QString();
}
QString BaseXMLNode::namespaceURI () const
{
  return QString();
}
