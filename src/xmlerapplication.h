// -*- C++ -*-
/* ------------------------------------ */
/* C++ class header (QApplication)      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerApplication
   Description:
*/
#ifndef XMLERAPPLICATION_H
#define XMLERAPPLICATION_H

#include "config.h"

#include <QtGui/QApplication>
#include <QtCore/QFile>
#include "mainwindow.h"

class XMLerApplication : public QApplication
{
  Q_OBJECT
public:
  XMLerApplication( int & argc, char ** argv );
  ~XMLerApplication();

  bool hasFilesInParams() const;
  void openFiles ();

private:
  QStringList autoOpenFiles;

  QList<QWidget *> openedWindows;

  void selectIconTheme ();
};

#endif
