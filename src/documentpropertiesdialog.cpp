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
}

DocumentPropertiesDialog::~DocumentPropertiesDialog ()
{
  delete ui;
}

/* self */
void DocumentPropertiesDialog::setDocument ( DocumentXMLNode *doc )
{
  _document = doc;

  if ( !_document ) {
    // TODO: make dialog disabled
  }

  /* fill dialog */
  /* Encoding */
  QString codec_name = QString::fromLatin1 ( _document->codec()->name() );
  int cb_idx = codecs_for_cb.indexOf ( codec_name );
  ui->comboBoxEncoding->setCurrentIndex ( cb_idx );

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

  
