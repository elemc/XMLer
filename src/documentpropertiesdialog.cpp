// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QDialog)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: DocumentPropertiesDialog
   Description:
*/
#include "documentpropertiesdialog.h"
#include "ui_documentpropertiesdialog.h"

DocumentPropertiesDialog::DocumentPropertiesDialog (QWidget *parent, Qt::WindowFlags f) :
  QDialog(parent, f),
  ui(new Ui::DocumentPropertiesDialog)
{
  ui->setupUi(this);
  setAttribute ( Qt::WA_DeleteOnClose );  

  initialActions();
  fillComboBox();

  piModel = new QStringListModel( this );
  ui->listViewPI->setModel ( piModel );

  connect ( ui->listViewPI->selectionModel(), SIGNAL( currentChanged (QModelIndex, QModelIndex) ), this, SLOT( indexChanged (QModelIndex, QModelIndex) ) );
  connect ( ui->pushButtonPIAdd, SIGNAL ( clicked () ), this, SLOT ( piAdd () ) );
  connect ( ui->pushButtonPIRemove, SIGNAL ( clicked () ), this, SLOT ( piRemove () ) );
  connect ( ui->lineEditPITarget, SIGNAL ( textEdited (QString) ), this, SLOT (piTargetChanged(QString)) );
  connect ( ui->lineEditPIData, SIGNAL ( textEdited (QString) ), this, SLOT (piDataChanged(QString)) );

  ui->groupBoxPIEdit->setVisible ( ui->pushButtonPIEditToggle->isChecked() );
}

DocumentPropertiesDialog::~DocumentPropertiesDialog ()
{
  ui->listViewPI->setModel ( 0 );
  delete piModel;
  piModel = 0;

  delete ui;
}

/* self */
void DocumentPropertiesDialog::setDocument ( DocumentXMLNode *doc )
{
  _document = doc;

  if ( !_document ) {
    // TODO: make dialog disabled
    ui->tabWidget->setEnabled ( false );
    ui->buttonBox->setStandardButtons ( QDialogButtonBox::Close );
    return;
  }

  /* fill dialog */
  /* Encoding */
  QString codec_name = QString::fromLatin1 ( _document->codec()->name() );
  int cb_idx = codecs_for_cb.indexOf ( codec_name );
  ui->comboBoxEncoding->setCurrentIndex ( cb_idx );

  /* Version */
  ui->lineEditVersion->setText ( _document->version() );

  /* Indent */
  ui->checkBoxAutoFormatting->setChecked( _document->autoFormatting() );
  int indent = _document->formattingIndent();
  if ( indent < 0 ) {
    ui->radioButtonTabs->setChecked ( true );
    ui->radioButtonSpaces->setChecked ( false );
    ui->spinBoxNumber->setValue ( -indent );
  }
  else {
    ui->radioButtonTabs->setChecked ( false );
    ui->radioButtonSpaces->setChecked ( true );
    ui->spinBoxNumber->setValue ( indent );
  }

  /* Processing Instructions */
  _pi = _document->processingInstructions();
  resetModel ();
}
DocumentXMLNode *DocumentPropertiesDialog::document () const
{
  return _document;
}

/* private self */
void DocumentPropertiesDialog::fillComboBox ()
{
  /* fill combobox */
  //QStringList codecs_for_cb;
  QList<QByteArray> codecs = QTextCodec::availableCodecs();
  QList<QByteArray>::iterator it;
  for ( it = codecs.begin(); it != codecs.end(); ++it )
    codecs_for_cb.append ( QString::fromLatin1 ( *it ) );

  codecs_for_cb.sort();
  ui->comboBoxEncoding->addItems ( codecs_for_cb );
}
void DocumentPropertiesDialog::initialActions ()
{
  connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(preAccept()));
  connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(preReject()));
  connect(ui->checkBoxAutoFormatting, SIGNAL(stateChanged(int)), this, SLOT(autoFormattingCheck(int)));
  connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(indentRadioChanged(int)));
}
void DocumentPropertiesDialog::resetModel ()
{
  //checkEmptyInMap();
  piModel->setStringList ( _pi.keys () );
  indexChanged ( ui->listViewPI->currentIndex(), QModelIndex() );
}
QString DocumentPropertiesDialog::piKeyByIndex ( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return QString();

  return piModel->stringList().at ( index.row() );
}
QString DocumentPropertiesDialog::checkSymbols ( QString text )
{
  QString result = text.replace(" ", "_");
  result = result.replace("?>", "__");
  return result;
}
void DocumentPropertiesDialog::removeEmptyPI ()
{
  QMap<QString,QString> copy = _pi;
  QMap<QString,QString>::iterator it;
  for ( it = copy.begin(); it != copy.end(); ++it ) {
    if ( it.key().trimmed().isEmpty() )
      _pi.remove ( it.key() );
  }
}

/* Slots */
void DocumentPropertiesDialog::preAccept ()
{
  /* Fill a document parameters */
  /* encoding */
  _document->setCodec ( codecs_for_cb.at( ui->comboBoxEncoding->currentIndex() ) );
  bool formatting = ui->checkBoxAutoFormatting->checkState() == Qt::Checked;
  int indent = ui->spinBoxNumber->value();
  if ( ui->buttonGroup->checkedId() == ui->buttonGroup->id( ui->radioButtonTabs ) )
    indent = -indent;
  _document->setFormatting ( formatting, indent );
  _document->setVersion ( ui->lineEditVersion->text() );
  _document->setPI ( _pi );

  accept();
}
void DocumentPropertiesDialog::preReject ()
{
  reject();
}
void DocumentPropertiesDialog::autoFormattingCheck ( int state )
{
  ui->groupBoxIndent->setEnabled ( state==Qt::Checked );
}
void DocumentPropertiesDialog::indentRadioChanged ( int id )
{
  if ( id == ui->buttonGroup->id( ui->radioButtonSpaces ) )
    ui->labelIndentNumber->setText( tr("Number of spaces") );
  else if ( id == ui->buttonGroup->id( ui->radioButtonTabs) )
    ui->labelIndentNumber->setText( tr("Number of tabs") );
}
void DocumentPropertiesDialog::indexChanged ( const QModelIndex &current, const QModelIndex &previous )
{
  Q_UNUSED(previous);

  bool valid = current.isValid();

  ui->pushButtonPIRemove->setEnabled ( valid );
  ui->lineEditPITarget->setEnabled ( valid );
  ui->lineEditPIData->setEnabled ( valid );
  if ( !valid ) {   
    ui->lineEditPIData->clear(); //setText( QString() );
    ui->lineEditPITarget->clear(); //setText( QString() );
    return;
  }

  QString key = piModel->stringList().at ( current.row() );
  QString data = _pi[key];
  ui->lineEditPITarget->setText ( key );
  ui->lineEditPIData->setText ( data );
}
void DocumentPropertiesDialog::piAdd ()
{
  QString newTarget;
  for ( int i=1; true; i++ ) {
    newTarget = tr("target_%1").arg ( i );
    if ( !_pi.keys().contains( newTarget  ) )
      break;
  }
  _pi.insert ( newTarget, tr("Data") );
  resetModel();
}
void DocumentPropertiesDialog::piRemove ()
{
  const QModelIndex &idx = ui->listViewPI->currentIndex();
  if ( !idx.isValid() )
    return;

  QString key = piKeyByIndex ( idx );
  _pi.remove ( key );
  resetModel ();
}
void DocumentPropertiesDialog::piTargetChanged ( const QString &text )
{
  const QModelIndex &idx = ui->listViewPI->currentIndex();
  QString oldKey = piKeyByIndex ( idx );
  QString value = _pi[oldKey];
  _pi.remove ( oldKey );

  QString correct_text = checkSymbols ( text );

  _pi.insert ( correct_text, value );
  piModel->setData ( idx, correct_text );
}
void DocumentPropertiesDialog::piDataChanged ( const QString &text )
{
  const QModelIndex &idx = ui->listViewPI->currentIndex();
  QString key = piKeyByIndex ( idx );
  _pi[key] = text;
}
