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
#include <QtXml/QXmlStreamReader>
#include "basexmlnode.h"
#include "documentxmlnode.h"
#include "xmlerhandler.h"
#include "xmlerexception.h"

class XMLerModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  explicit XMLerModel(QObject *parent = 0);
  ~XMLerModel();

  /* self */
  bool loadXMLFile ( const QString &fileName );
  bool saveXMLFile ( const QString &fileName );
  bool isEmptyModel () const;
  bool isModified () const;
  QString fileName () const;
  QString titlePart () const;
  QModelIndex rootIndex () const;

  /* virtuals */
  Qt::ItemFlags flags(const QModelIndex &index) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  QVariant data(const QModelIndex &index, int role) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
  DocumentXMLNode *_document;
  BaseXMLNode *_rootItem;
  bool _modified;
  QString _encoding;
  QString _version;

  void checkExceptionInHandler ( XMLerHandler *handler );
  QMap<QString, QString> getInformationFromFile ( const QString &fileName );

signals:
  void touchModel ();
  void parseException ( XMLerException::ExceptionType mainType, XMLerExceptionList exceptions );
};

#endif
