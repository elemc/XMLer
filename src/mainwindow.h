// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QMainWindow)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: MainWindow
   Description:
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"

#include <QMainWindow>
#include <QTreeView>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QLabel>
#include <QClipboard>
#include <QCloseEvent>
#include <QDockWidget>
#include <QMimeData>
#include "xmlermodel.h"
#include "xmlerexception.h"
#include "documentpropertiesdialog.h"
#include "xmlerfindwidget.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT;
public:
  explicit MainWindow(QWidget *parent = 0, Qt::WindowFlags f = 0);
  ~MainWindow();
  bool loadDocument( QString fileName );
  bool saveDocument( QString fileName );
  bool isEmptyDocument () const;
  
private:
  Ui::MainWindow *ui;
  XMLerModel *model;
  QTreeView *tree;
  QLabel *labelStatus;
  QProgressDialog *progressDialog;
  QDockWidget *findDock;
  XMLerFindWidget *findWidget;
  
  qint64 _progress_max;
  qint64 _progress_pos;

  /* inits */
  void initialActions();
  void initialActionsIcons();
  void initialActionsShortcuts();
  void initialTree();
  void initialStatusBar();
  void initialFindDock();

  void openDocumentInNewWindow( const QString &fileName );
  void expandRoot ();
  void resizeTreeColumns ();

  bool checkCloseWindow ();
  void closeEvent ( QCloseEvent *event );

private slots:
  /* File menu slots */
  void openDocumentAction();
  bool saveDocumentAction();
  bool saveAsDocumentAction();
  void closeDocumentAction();
  void propertiesAction();

  /* Edit menu slots */
  void copyNodeAction();
  void findAction();

  void modelTouched();
  void indexCollapsed ( const QModelIndex &index );
  void indexExpanded ( const QModelIndex &index );

  /* progress, warn and error slots */
  void beginProgressModel ( QString message, qint64 totalSize );
  void progressModel ( qint64 pos );
  void endProgressModel ();

  void onError ( QString msg );
  void onWarning ( QString msg );
  
  /* find slots */
  void showFounded ( BaseXMLNode *node );
  void foundedNodes ();

  /* bookmark slots */
  void bookmarkToggled ();
  void bookmarkNext ();
  void bookmarkPrev ();

  void collapseAll();
  void expandAll();
};

#endif
