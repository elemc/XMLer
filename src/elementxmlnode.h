// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (BaseXMLNode)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: ElementXMLNode
   Description:
*/
#ifndef ELEMENTXMLNODE_H
#define ELEMENTXMLNODE_H

#include "basexmlnode.h"
#include "attrxmlnode.h"
#include "dataxmlnode.h"

#include <QtXml/QXmlAttributes>

class ElementXMLNode : public BaseXMLNode
{
  Q_OBJECT
public:
  explicit ElementXMLNode(BaseXMLNode *parent = 0);
  ~ElementXMLNode();
  
  void setNamespaceURI( const QString &uri );
  void setLocalName( const QString &ln );
  void setQName( const QString &qn );
  void appendAttributes( const QXmlAttributes &attr );

  void appendChild ( BaseXMLNode *child );
  quint32 childCount() const;
  XMLNodePtrList childs() const;
  QString name() const;

private:
  QList<BaseXMLNode *> _childs;

  QString _namespaceURI;
  QString _localName;
  QString _qName;

};

#endif
