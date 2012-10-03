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
}

DocumentXMLNode::~DocumentXMLNode ()
{
  if ( rootNode )
    delete rootNode;
}

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
BaseXMLNode *DocumentXMLNode::documentNode () const
{
  return rootNode;
}
