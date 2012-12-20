// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QObject)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: BaseXMLNode
   Description:
*/
#ifndef BASEXMLNODE_H
#define BASEXMLNODE_H

#include <QObject>
#include <QList>
#include <QIcon>

class BaseXMLNode : public QObject
{
  Q_OBJECT
public:
  enum XMLNodeType { Unknown, Document, Element, Attribute, Data, Comment };

  explicit BaseXMLNode(BaseXMLNode *parent = 0);
  ~BaseXMLNode();

  void setParentNode(BaseXMLNode *p = 0);
  BaseXMLNode *parentNode() const;

  BaseXMLNode::XMLNodeType nodeType() const;                    // return node type
  int childIndex( BaseXMLNode *child ) const;                   // return child index
  int row();
  QString typeToStr() const;                                    // return type of node as string
  QIcon typeToIcon () const;                                    // return type of node as icon
  BaseXMLNode *childItemAt( int index ) const;                  // return child item at index
  qint64 size () const;                                         // return all child count

  /* Virtuals */
  virtual quint32 childCount() const;                           // return child count
  virtual QList<BaseXMLNode *> childs() const;                  // return all childs (elements and attributes)
  virtual void appendChild(BaseXMLNode *child);                 // add child to this object
  virtual QString name () const;                                // return display name of node
  virtual QString qName () const;                               // return qName
  virtual QString namespaceURI () const;                        // return namespace URI

private:
  BaseXMLNode *_parentNode;
  BaseXMLNode::XMLNodeType _nodeType;

protected:
  void setNodeType(XMLNodeType nt);

};
typedef QList<BaseXMLNode *> XMLNodePtrList;

#endif
