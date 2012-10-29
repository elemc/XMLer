// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QWidget)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerFindWidget
   Description:
*/
#ifndef XMLERFINDWIDGET_H
#define XMLERFINDWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QList>
#include "basexmlnode.h"

class XMLerFindWidget : public QWidget
{
  Q_OBJECT
public:
  XMLerFindWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
  ~XMLerFindWidget();
  
private:
  QHBoxLayout *hbox;
  QVBoxLayout *vbox;
  QLineEdit *lineEditFind;
  QPushButton *pushButtonFind;
  QPushButton *pushButtonClear;
  QLabel *labelStatus;
  int current_found_position;
  XMLNodePtrList foundedList;
  QString oldFindText;

  void updateLabelStatus ();

public slots:
  void Founded ( const XMLNodePtrList &list );
  void FindNext ();
  void FindPrevious ();
  void LineEditFocus ();

signals:
  void FindNodes ( const QString &findText );
  void Show ( BaseXMLNode *node );
  void ClearFind ();

private slots:
  void findButtonPressed ();
  void clearButtonPressed ();
};

#endif
