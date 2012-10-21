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
  labelStatus = new QLabel( this );
  box = new QHBoxLayout;
  box->addWidget(lineEditFind);
  box->addWidget(pushButtonFind);
  box->addWidget(labelStatus);

  setLayout( box );

  current_found_position = -1;

  connect ( pushButtonFind, SIGNAL(pressed()), this, SLOT(findButtonPressed()) );
}

XMLerFindWidget::~XMLerFindWidget ()
{
  delete pushButtonFind;
  pushButtonFind = 0;
  delete lineEditFind;
  lineEditFind = 0;
  delete box;
  box = 0;
}

/* Slots */
void XMLerFindWidget::Founded ( const QList<BaseXMLNode *> &list )
{
  foundedList = list;
  FindNext();
}
void XMLerFindWidget::FindNext ()
{
  if ( current_found_position == foundedList.size()-1 )
    current_found_position = 0;
  else
    current_found_position += 1;

  emit Show ( foundedList.at(current_found_position) );
}
void XMLerFindWidget::FindPrevious ()
{
  if ( current_found_position == 0 )
    current_found_position = foundedList.size() - 1;
  else
    current_found_position -= 1;

  emit Show ( foundedList.at(current_found_position) );
}

/* private slots */
void XMLerFindWidget::findButtonPressed ()
{
  QString text = lineEditFind->text();
  if ( !text.isEmpty() )
    emit FindNodes ( text );
}
