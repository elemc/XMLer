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
  _attributes.clear();
  _childs.clear();
}

ElementXMLNode::~ElementXMLNode ()
{
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
  if ( AttrXMLNode *n = qobject_cast<AttrXMLNode *>(child) )
    _attributes.append( n );
  else
    _childs.append( child );
}
quint32 ElementXMLNode::childCount() const
{
  return _childs.count() + _attributes.count();
}
XMLNodePtrList ElementXMLNode::childs() const
{
  XMLNodePtrList allList;
  allList += _attributes;
  allList += _childs;

  return allList;
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
BaseXMLNode *ElementXMLNode::childItemAt( quint32 index ) const
{
  /* First we return a attributes.
     And second we return a childrens */
  /* check index in attr && childs */
  quint32 count_attrs   = _attributes.count();
  quint32 count_childs  = _childs.count();
  quint32 count_all     = count_attrs + count_childs; 

  if ( count_all == 0 )
    return 0;

  if ( index > (count_all-1) )
    return 0;

  // check what index in attrs
  if ( index < count_attrs )
    return _attributes.at( index );
  else {
    quint32 c_index = count_all - index - 1; // this is real index
    return _childs.at(c_index);
  }

  return 0;
}
QString ElementXMLNode::name() const
{
  return _localName;
}
