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

#include <QAbstractItemModel>
#include <QBrush>
#include "basexmlnode.h"
#include "documentxmlnode.h"
#include "xmlerloadfilethread.h"
#include "xmlersavefilethread.h"
#include "xmlerfindthread.h"

class XMLerModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  explicit XMLerModel(QObject *parent = 0);
  ~XMLerModel();

  /* self */
  bool loadXMLFile ( const QString &fileName );
  bool saveXMLFile ( const QString &fileName );
  bool isNewModel () const;
  bool isEmptyModel () const;
  bool isModified () const;
  void modif();
  QString fileName () const;
  QString titlePart () const;
  QModelIndex rootIndex () const;

  XMLerLoadFileThread *loader ();
  XMLerSaveFileThread *saver ();
  XMLerFindThread *finder ();
  QByteArray indexToBuffer ( const QModelIndex &index );
  DocumentXMLNode *document () const;
  QModelIndex indexByNode ( BaseXMLNode *node ) const;
  void bookmarkToggle ( const QModelIndex &index );

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

  /* threaded loader, writer and finder */
  XMLerLoadFileThread *_loader;
  XMLerSaveFileThread *_saver;
  XMLerFindThread *_finder;

  XMLNodePtrList foundedNodes;
  XMLNodePtrList bookmarkNodes;
  qint64 bookmark_current_position;

  QIcon stateNodeIcon(BaseXMLNode *node) const;
  void safeUpdateBookmarkIndex ();

signals:
  void touchModel ();
  void gotoBookmark ( BaseXMLNode *node );

private slots:
  void on_loaderDone ( DocumentXMLNode *doc );
  void on_saverDone ();
  void on_finderDone ();

public slots:
  void findNodes ( const QString &findText );
  void bookmarkNext ();
  void bookmarkPrev ();
};

#endif
