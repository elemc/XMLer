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
#include "basexmlnode.h"
#include "documentxmlnode.h"
#include "xmlerloadfilethread.h"
#include "xmlersavefilethread.h"

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
  XMLerLoadFileThread *loader ();
  XMLerSaveFileThread *saver ();

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

  /* threaded loader and writer */
  XMLerLoadFileThread *_loader;
  XMLerSaveFileThread *_saver;

signals:
  void touchModel ();

private slots:
  void on_loaderDone ( DocumentXMLNode *doc );
  void on_saverDone ( DocumentXMLNode *doc );
};

#endif
