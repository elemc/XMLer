// -*- C++ -*-
/* -------------------------------------- */
/* C++ class source (XMLerFileOperThread) */
/* Author: Alexei Panov <me@elemc.name>   */
/* -------------------------------------- */
/* Name: XMLerSaveFileThread
   Description:
*/
#include "xmlersavefilethread.h"

XMLerSaveFileThread::XMLerSaveFileThread (QObject *parent):
  XMLerFileOperThread(parent)
{
  _document = 0;
}

XMLerSaveFileThread::~XMLerSaveFileThread ()
{
}

/* self */
void XMLerSaveFileThread::run () 
{
  if ( !_document ) {
    emit warning ( tr("Document is empty or doesn't set.") );
    return;
  }

  QFile xml ( fileName() );

  if ( !xml.open ( QIODevice::WriteOnly ) ) {
    emit error ( tr("Can not open file %1.").arg( fileName() ) );
    return;
  }

  QXmlStreamWriter writer( &xml );

  /* set information about document */
  writer.setAutoFormatting ( _document->autoFormatting() );
  writer.setAutoFormattingIndent ( _document->formattingIndent() );
  writer.setCodec ( _document->codec() );

  emit beginProgress ( progressMessage(), 0 );
  emit beginProgress ( progressMessage(), _document->size() );
  bool result = true;
  qint64 pos = 0;

  /* save a document */
  writer.writeStartDocument( _document->version() );

  if ( _document->hasPI() ) {
    const QMap<QString,QString> &pi = _document->processingInstructions();
    QMap<QString,QString>::const_iterator it;
    for ( it = pi.begin(); it != pi.end(); ++it ) {
      if ( it.key().trimmed().isEmpty() )
        continue;
      writer.writeProcessingInstruction ( it.key(), it.value() );
    }
  }
  
  if ( _document->documentNode() )
    result &= saveNode ( writer, _document->documentNode(), pos );

  writer.writeEndDocument();

  xml.close();
  emit endProgress ();

  /* set filename */
  if ( result && _document->fileName() != fileName() )
    _document->setFileName ( fileName() );

  if ( !result )
    emit error ( tr("Error while writing the document.").arg( fileName() ) );
  else
    emit done ( _document );
}
void XMLerSaveFileThread::setDocument ( DocumentXMLNode * doc )
{
  _document = doc;
}
DocumentXMLNode *XMLerSaveFileThread::document() const
{
  return _document;
}

/* private self */
QString XMLerSaveFileThread::progressMessage () const
{
  QString msg = tr("%1 file saved...").arg( fileName() );;
  return msg;
}
bool XMLerSaveFileThread::saveNode ( QXmlStreamWriter &writer, BaseXMLNode *node, qint64 &pos )
{
  bool result = true;
  pos += 1;

  if ( ElementXMLNode *element = qobject_cast<ElementXMLNode *>(node) ) {
    /* prefix mapping */
    if ( element->hasPrefixMapping() ) {
      QMap<QString,QString> prefixMapping = element->prefixMapping();
      QMap<QString,QString>::const_iterator it;

      for ( it = prefixMapping.begin(); it != prefixMapping.end(); ++it )
        writer.writeNamespace ( it.value(), it.key() );
    }


    if ( element->namespaceURI().isEmpty () )
      writer.writeStartElement ( element->qName() );
    else
      writer.writeStartElement ( element->namespaceURI(), element->name() );

    XMLNodePtrList chlist = element->childs();
    XMLNodePtrList::iterator it;

    for ( it = chlist.begin(); it != chlist.end(); ++it ) {
      BaseXMLNode *child_node = (*it);
      result &= saveNode ( writer, child_node, pos );
    }

    writer.writeEndElement();
  }
  else if ( AttrXMLNode *attr = qobject_cast<AttrXMLNode *>(node) ) {
    pos += 1;
    if ( attr->namespaceURI().isEmpty() )
      writer.writeAttribute ( attr->qName(), attr->value() );
    else
      writer.writeAttribute ( attr->namespaceURI(), attr->name(), attr->value() );
  }
  else if ( DataXMLNode *data = qobject_cast<DataXMLNode *>(node) ) {
    writer.writeCharacters ( data->data() );
  }

  emit progress ( pos );

  return result;
}
