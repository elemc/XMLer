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

#include <QtGui/QMainWindow>
#include <QtGui/QTreeView>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include "xmlermodel.h"

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
  bool isEmptyDocument () const;
  
private:
  Ui::MainWindow *ui;
  XMLerModel *model;
  QTreeView *tree;

  void initialActions();
  void initialActionsIcons();
  void initialTree();
  void openDocumentInNewWindow( const QString &fileName );

private slots:
  void openDocumentAction();
  void closeDocumentAction();
  void modelTouched();
  
};

#endif
