// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (BaseXMLNode)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: ElementXMLNode
   Description:
*/
#include "elementxmlnode.h"

ElementXMLNode::ElementXMLNode (BaseXMLNode *parent):
  BaseXMLNode(parent)
{
  setNodeType(BaseXMLNode::Element);
  _childs.clear();
}

ElementXMLNode::~ElementXMLNode ()
{
  qDeleteAll(_childs.begin(), _childs.end());
  _childs.clear();
}

void ElementXMLNode::setNamespaceURI( const QString &uri )
{
  _namespaceURI = uri;
}
void ElementXMLNode::setLocalName( const QString &ln )
{
  _localName = ln;
}
void ElementXMLNode::setQName( const QString &qn )
{
  _qName = qn;
}
void ElementXMLNode::appendChild( BaseXMLNode *child )
{
  if ( child )
    _childs.append( child );
}
quint32 ElementXMLNode::childCount() const
{
  return _childs.count();
}
XMLNodePtrList ElementXMLNode::childs() const
{
  return _childs;
}

void ElementXMLNode::appendAttributes( const QXmlAttributes &attr )
{
  for ( quint32 i = 0; i < attr.count(); i++ ) {
    // TODO: make append attributes
    AttrXMLNode *pattr = new AttrXMLNode( this );
    pattr->setNamespaceURI( attr.uri(i) );
    pattr->setLocalName( attr.localName(i) );
    pattr->setQName( attr.qName(i) );

    DataXMLNode *data = new DataXMLNode( pattr );
    data->setData( attr.value (i) );

    pattr->appendChild(data);
    appendChild(pattr);
  }
}
QString ElementXMLNode::name () const
{
  return _localName;
}
QString ElementXMLNode::qName () const
{
  return _qName;
}
QString ElementXMLNode::namespaceURI () const
{
  return _namespaceURI;
}

void ElementXMLNode::appendMapping ( const QString &prefix, const QString &uri )
{
  _prefixMapping.insert ( prefix, uri );
}
void ElementXMLNode::appendMapping ( const QMap<QString,QString> &map )
{
  QMap<QString,QString>::const_iterator it;
  for ( it = map.begin(); it != map.end(); ++it )
    _prefixMapping.insert ( it.key(), it.value() );
}
QMap<QString, QString> ElementXMLNode::prefixMapping () const
{
  return _prefixMapping;
}
bool ElementXMLNode::hasPrefixMapping () const
{
  return ( _prefixMapping.size() != 0 );
}
