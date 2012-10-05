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
  QString name () const;
  QString qName () const;
  QString namespaceURI () const;

  BaseXMLNode *documentNode() const;

  void setFileName( const QString &name );
  QString fileName () const;
  
private:
  BaseXMLNode *rootNode;
  QString _fileName;
  
};

#endif
