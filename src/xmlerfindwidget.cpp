// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QWidget)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerFindWidget
   Description:
*/
#include "xmlerfindwidget.h"

XMLerFindWidget::XMLerFindWidget (QWidget *parent, Qt::WindowFlags f) :
  QWidget(parent, f)
{
  lineEditFind = new QLineEdit( this );
  pushButtonFind = new QPushButton( tr("Find"), this );
  pushButtonFind->setAutoDefault( true );
  pushButtonFind->setDefault( true );

  pushButtonClear = new QPushButton( tr("&Clear"), this );
  //pushButtonClear->setShortcut( QKeySequence( Qt::Key_Escape ) );

  labelStatus = new QLabel( this );

  hbox = new QHBoxLayout;
  hbox->addWidget(lineEditFind);
  hbox->addWidget(pushButtonFind);
  hbox->addWidget(pushButtonClear);
  vbox = new QVBoxLayout;
  vbox->addLayout( hbox );
  vbox->addWidget(labelStatus);

  setLayout( vbox );

  current_found_position = -1;

  connect ( pushButtonFind, SIGNAL(pressed()), this, SLOT(findButtonPressed()) );
  connect ( pushButtonClear, SIGNAL(pressed()), this, SLOT(clearButtonPressed()) );
  connect ( lineEditFind, SIGNAL(returnPressed()), this, SLOT(findButtonPressed()) );
}

XMLerFindWidget::~XMLerFindWidget ()
{
  delete pushButtonFind;
  pushButtonFind = 0;

  delete pushButtonClear;
  pushButtonClear = 0;

  delete lineEditFind;
  lineEditFind = 0;

  delete hbox;
  hbox = 0;

  delete labelStatus;
  labelStatus = 0;

  delete vbox;
  vbox = 0;
}

/* self private */
void XMLerFindWidget::updateLabelStatus ()
{
    if ( oldFindText.isEmpty() ) {
        labelStatus->setText( QString () );
        return;
    }

    QString status;
    if ( foundedList.isEmpty() )
        status = tr("No found");
    else
        status = tr("Founded %1 items, selected %2 of %1").arg(foundedList.size()).arg(current_found_position+1);
    labelStatus->setText ( status );
}

/* Slots */
void XMLerFindWidget::Founded ( const QList<BaseXMLNode *> &list )
{
  foundedList = list;
  updateLabelStatus();
  if ( !foundedList.isEmpty() )
    FindNext();
  else
    current_found_position = -1;
}
void XMLerFindWidget::FindNext ()
{
  if ( foundedList.isEmpty() )
    return;

  if ( current_found_position == foundedList.size()-1 )
    current_found_position = 0;
  else
    current_found_position += 1;

  emit Show ( foundedList.at(current_found_position) );
  updateLabelStatus();
}
void XMLerFindWidget::FindPrevious ()
{
  if ( foundedList.isEmpty() )
    return;

  if ( current_found_position == 0 )
    current_found_position = foundedList.size() - 1;
  else
    current_found_position -= 1;

  emit Show ( foundedList.at(current_found_position) );
  updateLabelStatus();
}
void XMLerFindWidget::LineEditFocus ()
{
  lineEditFind->setFocus();
}

/* private slots */
void XMLerFindWidget::findButtonPressed ()
{
  QString text = lineEditFind->text();
  if ( !text.isEmpty() && text != oldFindText ) {
    emit FindNodes ( text );
    oldFindText = text;
  }
  else if ( text == oldFindText && !text.isEmpty() ) {
    FindNext();
  }
  else if ( text.isEmpty() ) {
    oldFindText = text;
    emit FindNodes( QString() );
    updateLabelStatus();
  }
}
void XMLerFindWidget::clearButtonPressed ()
{
    lineEditFind->setText ( QString() );
    foundedList.clear();
    findButtonPressed();
}
