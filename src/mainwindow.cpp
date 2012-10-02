// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QMainWindow)       */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: MainWindow
   Description:
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow (QWidget *parent, Qt::WindowFlags f) :
  QMainWindow(parent, f),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  initialActions();
  initialTree();

  setCentralWidget( tree );
}

MainWindow::~MainWindow ()
{
  tree->setModel( 0 );
  delete model;
  delete tree;
  delete ui;
}

void MainWindow::initialActions()
{
  connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(testOpenDocument()));
}
void MainWindow::initialTree()
{
  model = new XMLerModel( this );
  tree = new QTreeView( this );
  tree->setModel( model );
}
void MainWindow::testOpenDocument()
{
  //model->loadXMLFile("/Users/alex/workspace/bases/qwe.xml"); ///Users/alex/workspace/code/XMLer/test.xml");
}
bool MainWindow::loadDocument( QString fileName )
{
  return model->loadXMLFile( fileName );
}
