// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (BaseXMLNode)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: DataXMLNode
   Description:
*/
#ifndef DATAXMLNODE_H
#define DATAXMLNODE_H

#include "basexmlnode.h"

class DataXMLNode : public BaseXMLNode
{
  Q_OBJECT
public:
  explicit DataXMLNode(BaseXMLNode *parent = 0);
  ~DataXMLNode();

  void setData ( const QString &d );
  QString data() const;

  quint32 childCount() const;                           // return child count
  XMLNodePtrList childs() const;                        // return all childs (elements and attributes)
  void appendChild(BaseXMLNode *child);                 // add child to this object
  QString name () const;                               
  QString qName () const;
  QString namespaceURI () const;
  
private:
  QString _data;
};

#endif
