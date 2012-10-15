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

  setAttribute ( Qt::WA_DeleteOnClose );

  initialActions();
  initialTree();
  initialStatusBar();
  initialFindDock();

  setCentralWidget( tree );
  modelTouched ();
}

MainWindow::~MainWindow ()
{
  tree->setModel( 0 );
  delete model;
  delete tree;

  /* clean progress dialog */
  delete labelStatus;
  labelStatus = 0;
  delete progressDialog;
  progressDialog = 0;

  /* clean find dock */
  delete findDock->widget();
  delete findDock;
  findDock = 0;

  delete ui;
}

/* Inits */
void MainWindow::initialActionsIcons()
{
  /* generic icons */
  ui->actionNew->setIcon  ( QIcon::fromTheme("document-new") );
  ui->actionOpen->setIcon ( QIcon::fromTheme("document-open") );
  ui->actionSave->setIcon ( QIcon::fromTheme("document-save") );
  ui->actionSaveAs->setIcon ( QIcon::fromTheme("document-save-as") );
  ui->actionClose->setIcon( QIcon::fromTheme("document-close") );
  ui->actionExit->setIcon ( QIcon::fromTheme("application-exit") );
}
void MainWindow::initialActionsShortcuts ()
{
  /* File menu */
  ui->actionNew->setShortcut            ( QKeySequence ( QKeySequence::New ) );
  ui->actionOpen->setShortcut           ( QKeySequence ( QKeySequence::Open ) );
  ui->actionSave->setShortcut           ( QKeySequence ( QKeySequence::Save ) );
  ui->actionSaveAs->setShortcut         ( QKeySequence ( QKeySequence::SaveAs ) );
  ui->actionClose->setShortcut          ( QKeySequence ( QKeySequence::Close ) );
  ui->actionExit->setShortcut           ( QKeySequence ( QKeySequence::Quit ) );

  /* Edit menu */
  ui->actionCopy->setShortcut           ( QKeySequence ( QKeySequence::Copy ) );
  ui->actionFind->setShortcut           ( QKeySequence ( QKeySequence::Find ) );
  ui->actionFindNext->setShortcut       ( QKeySequence ( QKeySequence::FindNext ) );
  ui->actionFindPrevious->setShortcut   ( QKeySequence ( QKeySequence::FindPrevious ) );
}
void MainWindow::initialActions()
{
  initialActionsIcons();
  initialActionsShortcuts();

  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openDocumentAction()));
  connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveDocumentAction()));
  connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAsDocumentAction()));
  connect(ui->actionProperties, SIGNAL(triggered()), this, SLOT(propertiesAction()));
  connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeDocumentAction()));
  connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

  connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(copyNodeAction()));
  connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(findAction()));
}
void MainWindow::initialTree()
{
  model = new XMLerModel( this );
  tree = new QTreeView( this );
  tree->setModel( model );
  tree->setRootIsDecorated( false );

  /* tree signals */
  connect ( tree, SIGNAL(collapsed(QModelIndex)), this, SLOT(indexCollapsed(QModelIndex)) );
  /* connect ( tree, SIGNAL(expanded(QModelIndex)), this, SLOT(indexExpanded(QModelIndex)) ); */

  connect( model, SIGNAL(touchModel()), this, SLOT(modelTouched()));
  
  /* Loader signals */
  connect( model->loader(), SIGNAL(beginProgress(QString,qint64)), this, SLOT(beginProgressModel(QString,qint64)) );
  connect( model->loader(), SIGNAL(progress(qint64)), this, SLOT(progressModel(qint64)) );
  connect( model->loader(), SIGNAL(endProgress()), this, SLOT(endProgressModel()) );

  /* save signals */
  connect( model->saver(), SIGNAL(beginProgress(QString,qint64)), this, SLOT(beginProgressModel(QString,qint64)) );
  connect( model->saver(), SIGNAL(progress(qint64)), this, SLOT(progressModel(qint64)) );
  connect( model->saver(), SIGNAL(endProgress()), this, SLOT(endProgressModel()) );

  /* error and warning model loader/saver signals */
  connect ( model->loader(), SIGNAL( error(QString) ), this, SLOT( onError(QString) ) );
  connect ( model->loader(), SIGNAL( warning(QString) ), this, SLOT( onWarning(QString) ) );
  connect ( model->saver(), SIGNAL( error(QString) ), this, SLOT( onError(QString) ) );
  connect ( model->saver(), SIGNAL( warning(QString) ), this, SLOT( onWarning(QString) ) );
}
void MainWindow::initialStatusBar ()
{
  progressDialog = new QProgressDialog ( this, Qt::Dialog );
  progressDialog->setWindowModality( Qt::WindowModal );
  progressDialog->setCancelButton ( 0 );
  progressDialog->setAutoClose ( true );

  labelStatus = new QLabel ( progressDialog );
  progressDialog->setLabel ( labelStatus );

  /*progressBarStatus = new QProgressBar( ui->statusbar );
  labelStatus = new QLabel( ui->statusbar );
  ui->statusbar->addWidget ( labelStatus );
  ui->statusbar->addWidget ( progressBarStatus );

  labelStatus->hide();
  progressBarStatus->hide();*/

  _progress_max = 0;
  _progress_pos = 0;
}
void MainWindow::initialFindDock ()
{
  findDock = new QDockWidget( tr( "Find"), this );
  XMLerFindWidget *w = new XMLerFindWidget( findDock );
  findDock->setWidget ( w );

  findDock->setAllowedAreas ( Qt::BottomDockWidgetArea );
  findDock->setFeatures ( QDockWidget::AllDockWidgetFeatures );

  //findDock->show();
  addDockWidget( Qt::BottomDockWidgetArea, findDock );
  findDock->hide();
}

/* self */
void MainWindow::openDocumentInNewWindow( const QString &fileName )
{
  MainWindow *mw = new MainWindow;
  mw->show();
  mw->loadDocument( fileName );
}
bool MainWindow::loadDocument( QString fileName )
{
  bool result = model->loadXMLFile( fileName );
  QModelIndex rootIndex = model->rootIndex();
  if ( rootIndex.isValid () )
    tree->expand ( rootIndex );
  return result;
}
bool MainWindow::isEmptyDocument () const
{
  return model->isEmptyModel();
}
bool MainWindow::saveDocument( QString fileName )
{
  bool result = model->saveXMLFile ( fileName );
  if ( !result )
    QMessageBox::critical( this, tr("XML writer error"), tr("Error per write XML document") );
  return result;
}

/* Slots */
void MainWindow::openDocumentAction()
{
  QString selectedFileName = QFileDialog::getOpenFileName ( this, tr("Select file for open"), QDir::currentPath(), tr("XML files (*.xml);;All files (*.*)") );
  if ( selectedFileName.isEmpty() )
    return;

  if ( !QFile::exists( selectedFileName ) ) {
    QMessageBox::critical( this, tr("Input/output error"), tr("File %1 doesn't exist!").arg( selectedFileName ));
    return;
  }

  if ( isEmptyDocument() )
    loadDocument ( selectedFileName );
  else
    openDocumentInNewWindow ( selectedFileName );
}
void MainWindow::closeDocumentAction()
{
  close();
}
void MainWindow::modelTouched()
{
  /* change window title */
  QString genericWindowTitle = tr(APPNAME);
  setWindowTitle ( QString("%1 - %2").arg(genericWindowTitle).arg(model->titlePart()) );

  /* set root index is expanded always */
  tree->expand ( model->rootIndex() );

  ui->actionSave->setDisabled ( !model->isModified() );
}
void MainWindow::saveAsDocumentAction()
{
  if ( isEmptyDocument() )
    return;

  QString selectedFileName = QFileDialog::getSaveFileName ( this, tr("Select file for save"), QDir::currentPath(), tr("XML files (*.xml);;All files (*.*)") );
  if ( selectedFileName.isEmpty() )
    return;

  saveDocument ( selectedFileName );
}
void MainWindow::beginProgressModel ( QString message, qint64 totalSize )
{
  labelStatus->setText ( message );
  progressDialog->setMaximum( 100 );
  progressDialog->setMinimum( 0 );
  progressDialog->setValue( 0 );
  progressDialog->show();

  _progress_max = totalSize;
  _progress_pos = 0;
}
void MainWindow::progressModel ( qint64 pos )
{
  _progress_pos = pos;
  int progress = _progress_pos * 100 / _progress_max;
  if ( progress != progressDialog->value() )
    progressDialog->setValue ( progress );
}
void MainWindow::endProgressModel ()
{
  _progress_max = 0;
  _progress_pos = 0;
  progressDialog->close();
}
void MainWindow::onError ( QString msg )
{
  QMessageBox::critical( this, tr("File operation error"), msg );
}
void MainWindow::onWarning ( QString msg )
{
  QMessageBox::warning ( this, tr("File operation warning"), msg );
}
void MainWindow::propertiesAction ()
{
  DocumentPropertiesDialog *dlg = new DocumentPropertiesDialog ( this );
  dlg->setDocument ( model->document() );
  int result = dlg->exec();
  if ( result == QDialog::Accepted )
    model->modif();
}
void MainWindow::indexCollapsed ( const QModelIndex &index )
{
  if ( index == model->rootIndex () )
    tree->expand ( index );
}
/* Not needed now 
void MainWindow::indexExpanded ( const QModelIndex &index )
{

}
*/
void MainWindow::saveDocumentAction ()
{
  if ( model->isNewModel() ) {
    saveAsDocumentAction();
    return;
  }
  saveDocument ( model->fileName() );
}
void MainWindow::copyNodeAction ()
{
  QModelIndex index = tree->currentIndex();
  QClipboard *clip = qApp->clipboard();

  QByteArray xml_data = model->indexToBuffer ( index );
  QString caption = model->data( index, Qt::DisplayRole ).toString();
  QMimeData *md = new QMimeData;

  md->setData ( QString("text/plain"), caption.toUtf8() );
  md->setData ( QString("text/xml"), xml_data );
  
  clip->setMimeData ( md );
}
void MainWindow::findAction ()
{
  findDock->show();
}
