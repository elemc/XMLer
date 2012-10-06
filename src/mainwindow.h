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
#include <QtGui/QProgressDialog>
#include <QtGui/QLabel>
#include "xmlermodel.h"
#include "xmlerexception.h"

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
  

  qint64 _progress_max;
  qint64 _progress_pos;

  /* inits */
  void initialActions();
  void initialActionsIcons();
  void initialTree();
  void initialStatusBar();

  void openDocumentInNewWindow( const QString &fileName );

private slots:
  void openDocumentAction();
  void saveAsDocumentAction();
  void closeDocumentAction();
  void modelTouched();

  void parsingException( XMLerException::ExceptionType mainType, XMLerExceptionList exceptions );

  void on_model_beginProgress ( QString message, qint64 totalSize );
  void on_model_progress ( qint64 pos );
  void on_model_endProgress ();
};

#endif
