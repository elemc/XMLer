// -*- C++ -*-
/* -------------------------------------- */
/* C++ class header (XMLerFileOperThread) */
/* Author: Alexei Panov <me@elemc.name>   */
/* -------------------------------------- */
/* Name: XMLerSaveFileThread
   Description:
*/
#ifndef XMLERSAVEFILETHREAD_H
#define XMLERSAVEFILETHREAD_H

#include "config.h"

#include <QtCore/QBuffer>
#include "xmlerfileoperthread.h"
#include "basexmlnode.h"
#include "elementxmlnode.h"
#include "attrxmlnode.h"
#include "dataxmlnode.h"

class XMLerSaveFileThread : public XMLerFileOperThread
{
  Q_OBJECT
public:
  explicit XMLerSaveFileThread(QObject *parent = 0);
  ~XMLerSaveFileThread();

  void run ();

  void setDocument ( DocumentXMLNode *doc );
  DocumentXMLNode *document () const;
  QByteArray toBuffer ( BaseXMLNode *node );
  
private:
  DocumentXMLNode *_document;
  QString progressMessage () const;
  bool saveNode ( QXmlStreamWriter &writer,  BaseXMLNode *node, qint64 &pos );
 
};

#endif
