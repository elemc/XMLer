// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QAbstractItemModel)*/
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerModel
   Description:
*/
#include "xmlermodel.h"

XMLerModel::XMLerModel (QObject *parent):
  QAbstractItemModel(parent)
{
  _document = 0;
  _rootItem = 0;
  _modified = false;
}
XMLerModel::~XMLerModel ()
{
  if ( _document )
    delete _document;
  /* if ( _rootItem )
     delete _rootItem; */
}

/* self */
bool XMLerModel::loadXMLFile( const QString &fileName )
{
  QFile xml( fileName );
  if ( !xml.exists() )
    return false;

  QMap<QString, QString> info = getInformationFromFile ( fileName );

  beginResetModel();

  QXmlSimpleReader reader;
  QXmlInputSource source( &xml );// = new QXmlInputSource( &xml );
  XMLerHandler *handler = new XMLerHandler;

  reader.setContentHandler ( handler );
  reader.setErrorHandler ( handler );

  bool parseResult = reader.parse ( &source );
  if ( !parseResult ) {
    checkExceptionInHandler( handler );

    delete handler;
    endResetModel();
    return false;
  }

  _document = handler->document();
  _rootItem = _document->documentNode();

  endResetModel();

  /* set addition data (information) in document */
  _document->setFileName( fileName );
  if ( !info.isEmpty() ) {
    if ( info.contains ( "encoding" ) )
      _document->setCodec ( info.value ( "encoding" ) );
    if ( info.contains ( "version" ) )
      _document->setVersion ( info.value ( "version" ) );
  }

  checkExceptionInHandler ( handler );

  emit touchModel();

  delete handler;
  return true;
}
bool XMLerModel::saveXMLFile ( const QString &fileName )
{
  if ( !_document )
    return false;

  return _document->save ( fileName );
}
bool XMLerModel::isEmptyModel () const
{
  return ( _document == 0 );
}
QString XMLerModel::fileName () const
{
  if ( isEmptyModel () )
    return tr("untitled");

  return _document->fileName();
}
bool XMLerModel::isModified () const
{
  return _modified;
}
QString XMLerModel::titlePart () const
{
  QString fn = fileName();
  if ( isModified() )
    fn += "*";
  return fn;
}
QModelIndex XMLerModel::rootIndex () const
{
  return index ( 0, 0 );
}

/* private self */
void XMLerModel::checkExceptionInHandler ( XMLerHandler *handler )
{
  if ( !handler->hasExceptions () )
    return;

  XMLerException::ExceptionType mt = XMLerException::Warning;
  if ( handler->hasFatalErrors () )
    mt = XMLerException::FatalError;
  else if ( handler->hasErrors () )
    mt = XMLerException::Error;

  emit parseException ( mt, handler->exceptions() );
}
QMap<QString, QString> XMLerModel::getInformationFromFile ( const QString &fileName )
{
  QMap<QString, QString> result;

  QFile xml_file( fileName );
  if ( !xml_file.exists() )
    return result;

  if ( !xml_file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
    return result;

  QString encoding;
  QString version;

  QXmlStreamReader reader ( &xml_file );
  while ( !reader.atEnd() ) {
    QXmlStreamReader::TokenType tt = reader.readNext();
    if ( tt == QXmlStreamReader::StartDocument ) {
      encoding = reader.documentEncoding().toString();
      version = reader.documentVersion().toString();
      break;
    }
  }
  
  if ( !encoding.isEmpty() )
    result.insert("encoding", encoding);
  if ( !version.isEmpty() )
    result.insert("version", version);

  xml_file.close();

  return result;
}

/* Virtuals */
Qt::ItemFlags XMLerModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
  return defaultFlags;
}
int XMLerModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 3;
}
QVariant XMLerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      switch (section) {
      case 2:
        return tr("Local name");
        break;
      case 0:
        return tr("Qualified name");
        break;
      case 1:
        return tr("Namespace URI");
      default:
        break;
      }
    }
  }

  return QAbstractItemModel::headerData( section, orientation, role );
}
QModelIndex XMLerModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  BaseXMLNode *parentItem;
  if (!parent.isValid())
    parentItem = _document; //rootItem;
  else
    parentItem = static_cast<BaseXMLNode *>(parent.internalPointer());

  BaseXMLNode *childItem = parentItem->childItemAt(row);
  if ( childItem )
    return createIndex(row, column, childItem);

  return QModelIndex();  
}
QModelIndex XMLerModel::parent(const QModelIndex &child) const
{
  if ( !child.isValid() )
    return QModelIndex();

  BaseXMLNode *childNode = static_cast<BaseXMLNode *>(child.internalPointer());
  BaseXMLNode *parentNode = childNode->parentNode();

  if ( !parentNode || parentNode == _document )
    return QModelIndex();

  return createIndex(parentNode->row(), 0, parentNode);
}
QVariant XMLerModel::data(const QModelIndex &index, int role) const
{
  if ( !index.isValid() )
    return QVariant();

  BaseXMLNode *item = static_cast<BaseXMLNode *>(index.internalPointer());
  if ( !item )
    return QVariant();

  if ( role == Qt::DisplayRole ) {
    switch( index.column() ) {
    case 2:
      return item->name();
      break;
    case 0:
      return item->qName();
      break;
    case 1:
      return item->namespaceURI();
      break;
    default:
      break;
    }
  }
  else if ( role == Qt::DecorationRole && index.column() == 0 ) {
    return item->typeToIcon();
  }

  return QVariant();
}
int XMLerModel::rowCount(const QModelIndex &parent) const
{
  if ( parent.column() > 0 )
    return 0;

  BaseXMLNode *item;
  if ( !parent.isValid() )
    item = _document; //rootItem;
  else
    item = static_cast<BaseXMLNode*>(parent.internalPointer());

  if ( item )
    return item->childCount();

  return 0;
}

/* Slots */

