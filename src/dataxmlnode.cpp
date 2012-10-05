// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (BaseXMLNode)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: DataXMLNode
   Description:
*/
#include "dataxmlnode.h"

DataXMLNode::DataXMLNode (BaseXMLNode *parent):
  BaseXMLNode(parent)
{
  setNodeType(BaseXMLNode::Data);
  _data.clear();
}

DataXMLNode::~DataXMLNode ()
{
}

void DataXMLNode::setData ( const QString &d )
{
  _data = d;
}
QString DataXMLNode::data () const
{
  return _data;
}

/* Virtuals */
quint32 DataXMLNode::childCount() const
{
  return 0; // No childrens
}
XMLNodePtrList DataXMLNode::childs() const
{
  return XMLNodePtrList();
}
void DataXMLNode::appendChild(BaseXMLNode *child)
{
  // Do nothing
}
QString DataXMLNode::name () const
{
  return QString();
}
QString DataXMLNode::qName () const
{
  return _data;
}
QString DataXMLNode::namespaceURI () const
{
  return QString();
}
