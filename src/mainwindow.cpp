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

  setCentralWidget( tree );
}

MainWindow::~MainWindow ()
{
  tree->setModel( 0 );
  delete model;
  delete tree;

  delete labelStatus;
  labelStatus = 0;
  delete progressDialog;
  progressDialog = 0;
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
void MainWindow::initialActions()
{
  initialActionsIcons();

  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openDocumentAction()));
  connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAsDocumentAction()));
  connect(ui->actionProperties, SIGNAL(triggered()), this, SLOT(propertiesAction()));
  connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeDocumentAction()));
  connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
}
void MainWindow::initialTree()
{
  model = new XMLerModel( this );
  tree = new QTreeView( this );
  tree->setModel( model );
  tree->setRootIsDecorated( false );

  connect( model, SIGNAL(touchModel()), this, SLOT(modelTouched()));
  
  /* Loader signals */
  connect( model->loader(), SIGNAL(parseException(XMLerException::ExceptionType,XMLerExceptionList)), this, SLOT(parsingException(XMLerException::ExceptionType,XMLerExceptionList)) );
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
}
void MainWindow::parsingException( XMLerException::ExceptionType mainType, XMLerExceptionList exceptions )
{
  QString caption = tr("XML parse %1").arg(XMLerException::exceptionTypeStr(mainType));
  QStringList msgs;
  XMLerExceptionList::iterator it;
  for ( it = exceptions.begin(); it != exceptions.end(); ++it )
    msgs.append ( (*it).printMessage() );

  if ( mainType == XMLerException::Warning )
    QMessageBox::warning( this, caption, msgs.join("\n") );
  else
    QMessageBox::critical( this, caption, msgs.join("\n") );
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
