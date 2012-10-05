// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QObject)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: CommentXMLNode
   Description:
*/
#include "commentxmlnode.h"

CommentXMLNode::CommentXMLNode (BaseXMLNode *parent):
  DataXMLNode(parent)
{
  setNodeType ( BaseXMLNode::Comment );
}

CommentXMLNode::~CommentXMLNode ()
{
}
