// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (BaseXMLNode)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: AttrXMLNode
   Description:
*/
#ifndef ATTRXMLNODE_H
#define ATTRXMLNODE_H

#include "basexmlnode.h"
#include "dataxmlnode.h"

class AttrXMLNode : public BaseXMLNode
{
  Q_OBJECT
public:
  explicit AttrXMLNode(BaseXMLNode *parent = 0);
  ~AttrXMLNode();

  void setNamespaceURI( const QString &uri );
  void setLocalName( const QString &ln );
  void setQName( const QString &qn );

  quint32 childCount() const;                           // return child count
  XMLNodePtrList childs() const;                        // return all childs (elements and attributes)
  void appendChild(BaseXMLNode *child);                 // add child to this object
  QString name() const;

private:
  QString _namespaceURI;
  QString _localName;
  QString _qName;

  DataXMLNode *_data;
  
};

#endif
