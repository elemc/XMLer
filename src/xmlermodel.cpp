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
}

XMLerModel::~XMLerModel ()
{
  if ( _document )
    delete _document;
  /* if ( _rootItem )
     delete _rootItem; */
}

bool XMLerModel::loadXMLFile(const QString &fileName)
{
  QFile xml(fileName);
  if ( !xml.exists() )
    return false;

  beginResetModel();

  //xmlExceptionList.clear();
  QXmlSimpleReader reader;
  QXmlInputSource *source = new QXmlInputSource( &xml );
  XMLerHandler *handler = new XMLerHandler;
  /*connect ( handler, SIGNAL (xmlException (XMLerHandler::Exceptions, const QXmlParseException)), 
    this, SLOT(on_Exception(XMLerHandler::Exceptions, const QXmlParseException &)));*/

  reader.setContentHandler ( handler );
  reader.setErrorHandler ( handler );

  bool parseResult = reader.parse ( source );
  if ( !parseResult ) {
    /* TODO: make exception here */
    delete source;
    delete handler;
    return false;
  }

  /*if ( xmlExceptionList.count() > 0 )
    emit xmlException ( xmlExceptionList ); */

  _document = handler->document();
  _rootItem = _document->documentNode();
  endResetModel();

  return true;
}

Qt::ItemFlags XMLerModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);
  return defaultFlags;
}
int XMLerModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;
}
QVariant XMLerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      switch (section) {
      case 0:
        return tr("Name");
        break;
      case 1:
        return tr("Type");
        break;
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

  if ( !parentNode ) //|| parentNode == _document )
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
    case 0:
      return item->name();
      break;
    case 1:
      return item->typeToStr();
      break;
    default:
      break;
    }
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
void XMLerModel::on_Exception( XMLerHandler::Exceptions e, int column, int line, QString msg )
{
}
