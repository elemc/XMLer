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

#include <QtGui/QMainWindow>
#include <QtGui/QTreeView>
#include "xmlermodel.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0, Qt::WindowFlags f = 0);
  ~MainWindow();
  
private:
  Ui::MainWindow *ui;
  XMLerModel *model;
  QTreeView *tree;

  void initialActions();
  void initialTree();
                       
private slots:
  void testOpenDocument();
  
};

#endif
