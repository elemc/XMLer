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

  _loader = new XMLerLoadFileThread ( this );
  connect ( _loader, SIGNAL(done(DocumentXMLNode*)), this, SLOT(on_loaderDone(DocumentXMLNode*)) );

  _saver = new XMLerSaveFileThread ( this );
  connect ( _saver, SIGNAL(done(DocumentXMLNode*)), this, SLOT(on_saverDone(DocumentXMLNode*)) );
}
XMLerModel::~XMLerModel ()
{
  if ( _document )
    delete _document;

  /* clean loader */
  delete _loader;
  _loader = 0;
}

/* self */
bool XMLerModel::loadXMLFile( const QString &fileName )
{
  _loader->setFileName ( fileName );
  _loader->start();
}
bool XMLerModel::saveXMLFile( const QString &fileName )
{
  bool result = true;

  _saver->setFileName ( fileName );
  _saver->setDocument ( _document );
  _saver->start();

  return result;
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
XMLerLoadFileThread *XMLerModel::loader ()
{
  return _loader;
}
XMLerSaveFileThread *XMLerModel::saver ()
{
  return _saver;
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
void XMLerModel::on_loaderDone ( DocumentXMLNode *doc )
{
  if ( !doc )
    return;

  beginResetModel();
  _document = doc;
  endResetModel();
  emit touchModel();
}
void XMLerModel::on_saverDone ( DocumentXMLNode *doc )
{
  emit touchModel();
}
