// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (DataXMLNode)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: CommentXMLNode
   Description:
*/
#ifndef COMMENTXMLNODE_H
#define COMMENTXMLNODE_H

#include "dataxmlnode.h"

class CommentXMLNode : public DataXMLNode
{
  Q_OBJECT
public:
  explicit CommentXMLNode(BaseXMLNode *parent = 0);
  ~CommentXMLNode();
  
};

#endif
