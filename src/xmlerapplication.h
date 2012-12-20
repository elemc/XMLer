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

#include <QApplication>
#include <QFile>
#include "mainwindow.h"

class XMLerApplication : public QApplication
{
  Q_OBJECT
public:
  XMLerApplication( int & argc, char ** argv );
  ~XMLerApplication();

  bool hasFilesInParams() const;
  void openFiles ();

  bool notify ( QObject * receiver, QEvent * e );

private:
  QStringList autoOpenFiles;
  void selectIconTheme ();

  QList<MainWindow*> windowList;
  void appendChildWindow ( MainWindow *mw );
  void removeChildWindow ( MainWindow *mw );
  void changedChildWindowTitle ( MainWindow *mw );

};

#endif
