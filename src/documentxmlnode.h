// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (BaseXMLNode)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: DocumentXMLNode
   Description:
*/
#ifndef DOCUMENTXMLNODE_H
#define DOCUMENTXMLNODE_H

#include "basexmlnode.h"

class DocumentXMLNode : public BaseXMLNode
{
  Q_OBJECT
public:
  explicit DocumentXMLNode();
  ~DocumentXMLNode();

  void appendChild(BaseXMLNode *child);
  quint32 childCount() const;
  XMLNodePtrList childs() const;
  BaseXMLNode *childItemAt( quint32 index ) const;
  QString name() const;

  BaseXMLNode *documentNode() const;
  
private:
  BaseXMLNode *rootNode;
  
};

#endif
