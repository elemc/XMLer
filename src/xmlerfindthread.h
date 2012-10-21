// -*- C++ -*-
/* -------------------------------------- */
/* C++ class header (XMLerFileOperThread) */
/* Author: Alexei Panov <me@elemc.name>   */
/* -------------------------------------- */
/* Name: XMLerFindThread
   Description:
*/
#ifndef XMLERFINDTHREAD_H
#define XMLERFINDTHREAD_H

#include "xmlerfileoperthread.h"

class XMLerFindThread : public XMLerFileOperThread
{
  Q_OBJECT
public:
  explicit XMLerFindThread(QObject *parent = 0);
  ~XMLerFindThread();

  void run ();
  void setDocument ( DocumentXMLNode *doc );
  DocumentXMLNode *document () const;
  void setText ( const QString &findText );
  QString text () const;

  XMLNodePtrList foundedNodes () const;
  
private:
  DocumentXMLNode *_document;
  QString _findText;
  XMLNodePtrList founded;

  QString progressMessage () const;
  void findNode ( BaseXMLNode *node, qint64 &pos );
  bool findText ( const QString &text ) const;
};

#endif
