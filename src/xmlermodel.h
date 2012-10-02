// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QAbstractItemModel)*/
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerModel
   Description:
*/
#ifndef XMLERMODEL_H
#define XMLERMODEL_H

#include "config.h"

#include <QtCore/QAbstractItemModel>
#include <QtXml/QXmlSimpleReader>
#include <QtXml/QXmlParseException>
#include <QtXml/QXmlInputSource>
#include "basexmlnode.h"
#include "documentxmlnode.h"
#include "xmlerhandler.h"

class XMLerModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  explicit XMLerModel(QObject *parent = 0);
  ~XMLerModel();

  bool loadXMLFile( const QString &fileName );

  /* virtuals */
  Qt::ItemFlags flags(const QModelIndex &index) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column, const QModelIndex &idx = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &idx) const;
  QVariant data(const QModelIndex &index, int role) const;
  int rowCount(const QModelIndex &idx = QModelIndex()) const;

private:
  DocumentXMLNode *_document;
  BaseXMLNode *_rootItem;
//QList<QXmlParseException> xmlExceptionList;
  
public slots:
void on_Exception( XMLerHandler::Exceptions e, int column, int line, QString msg );
  
};

#endif
