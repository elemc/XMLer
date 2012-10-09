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
#include <QtCore/QMap>

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
  QString name () const;
  QString qName () const;
  QString namespaceURI () const;

  void appendMapping ( const QString &prefix, const QString &uri );
  void appendMapping ( const QMap<QString,QString> &map );
  QMap<QString,QString> prefixMapping () const;
  bool hasPrefixMapping () const;

private:
  QList<BaseXMLNode *> _childs;
  QMap<QString,QString> _prefixMapping;

  QString _namespaceURI;
  QString _localName;
  QString _qName;

};

#endif
