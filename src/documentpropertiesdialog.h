// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QDialog)           */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: DocumentPropertiesDialog
   Description:
*/
#ifndef DOCUMENTPROPERTIESDIALOG_H
#define DOCUMENTPROPERTIESDIALOG_H

#include "config.h"

#include <QtGui/QDialog>
#include <QtCore/QTextCodec>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>
#include "documentxmlnode.h"

namespace Ui {
  class DocumentPropertiesDialog;
}

class DocumentPropertiesDialog : public QDialog
{
  Q_OBJECT
public:
  explicit DocumentPropertiesDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
  virtual ~DocumentPropertiesDialog();

  void setDocument ( DocumentXMLNode *doc );
  DocumentXMLNode *document () const;
  
private:
  Ui::DocumentPropertiesDialog *ui;
  DocumentXMLNode *_document;
  QStringList codecs_for_cb;

  void fillComboBox();
  void initialActions();
                                  
private slots:
  void preAccept();
  void preReject();
  void autoFormattingCheck(int state);
  void indentRadioChanged (int id);
};

#endif
