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

void BaseXMLNode::setParentNode(BaseXMLNode *p)
{
  _parentNode = p;
  /* if ( p ) {
    p->appendChild(this);
    } */
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
  //XMLNodePtrList list = childs();
  QLinkedList< BaseXMLNode *> l = childs();
  int idx = -1;

  QLinkedListIterator< BaseXMLNode *> it(l);
  while ( it.hasNext() ) {
    idx += 1;
    if ( it.next() == child )
      break;
  }

  return idx; //l.indexOf ( child );
}
int BaseXMLNode::row ()
{
  if ( _parentNode )
    return _parentNode->childIndex( this );
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
BaseXMLNode *BaseXMLNode::childItemAt( quint32 index ) const
{
  return 0; // return zero
}
QString BaseXMLNode::name() const
{
  return QString(); // return empty string
}
