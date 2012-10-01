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


quint32 DataXMLNode::childCount() const
{
  return 0; // No childrens
}
XMLNodePtrList DataXMLNode::childs() const
{
  XMLNodePtrList list;
  return list;
}
void DataXMLNode::appendChild(BaseXMLNode *child)
{
  // Do nothing
}
BaseXMLNode *DataXMLNode::childItemAt( quint32 index ) const
{
  return 0; // always zero, this class not contained childrens
}
QString DataXMLNode::name () const
{
  return _data;
}
