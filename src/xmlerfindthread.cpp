// -*- C++ -*-
/* -------------------------------------- */
/* C++ class source (XMLerFileOperThread) */
/* Author: Alexei Panov <me@elemc.name>   */
/* -------------------------------------- */
/* Name: XMLerFindThread
   Description:
*/
#include "xmlerfindthread.h"

XMLerFindThread::XMLerFindThread (QObject *parent):
  XMLerFileOperThread(parent)
{
}

XMLerFindThread::~XMLerFindThread ()
{
}

void XMLerFindThread::run ()
{
  if ( !_document ) {
    emit error( tr("Document is empty" ) );
    return;
  }
  if ( _findText.isEmpty() ) {
    emit warning ( tr("Search phrase is empty") );
    return;
  }
  founded.clear();
      
  emit beginProgress ( progressMessage(), 0 );
  qint64 docSize = _document->size();
  qint64 pos = 0;
  emit beginProgress ( progressMessage(), docSize );

  /* search process */
  findNode ( _document, pos );
  /* end search process */
  emit endProgress ();
  emit done ();
}
void XMLerFindThread::setDocument ( DocumentXMLNode * doc )
{
  _document = doc;
}
DocumentXMLNode *XMLerFindThread::document() const
{
  return _document;
}
void XMLerFindThread::setText ( const QString &findText )
{
  _findText = findText;
}
QString XMLerFindThread::text () const
{
  return _findText;
}
XMLNodePtrList XMLerFindThread::foundedNodes () const
{
  return founded;
}

/* Privates */
QString XMLerFindThread::progressMessage () const
{
  return tr("Search \"%1\" in document...").arg(_findText);
}
bool XMLerFindThread::findText ( const QString &text ) const
{
  return text.contains ( _findText );
}
void XMLerFindThread::findNode ( BaseXMLNode *node, qint64 &pos )
{
  pos += 1;

  if ( ElementXMLNode *element = qobject_cast<ElementXMLNode *>(node) ) {
    if ( findText ( element->namespaceURI() ) ||
         findText ( element->name() ) ||
         findText ( element->qName() ) )
      founded.append ( element );

    XMLNodePtrList chlist = element->childs();
    XMLNodePtrList::iterator it;

    for ( it = chlist.begin(); it != chlist.end(); ++it ) {
      BaseXMLNode *child_node = (*it);
      findNode ( child_node, pos );
    }
  }
  else if ( AttrXMLNode *attr = qobject_cast<AttrXMLNode *>(node) ) {
    if ( findText ( attr->namespaceURI() ) ||
         findText ( attr->name() ) ||
         findText ( attr->qName() ) )
      founded.append ( attr );

    XMLNodePtrList chlist = attr->childs();
    XMLNodePtrList::iterator it;

    for ( it = chlist.begin(); it != chlist.end(); ++it ) {
      BaseXMLNode *child_node = (*it);
      findNode ( child_node, pos );
    }
  }
  else if ( DataXMLNode *data = qobject_cast<DataXMLNode *>(node) ) {
    if ( findText ( data->data() ) )
      founded.append ( data );
  }
  else if ( DocumentXMLNode *doc = qobject_cast<DocumentXMLNode *>(node) ) {
    if ( doc->documentNode() )
      findNode ( doc->documentNode(), pos );
  }

  emit progress ( pos );
}
