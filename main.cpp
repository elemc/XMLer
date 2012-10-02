// -*- C++ -*-
/* ------------------------------------ */
/* C++ source main file                 */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: main.cpp
   Description: This is main file for XMLer project
*/

#include "config.h"

#include "xmlerapplication.h"
#include <QtCore/QTextCodec>

#include "mainwindow.h"

int main ( int argc, char **argv )
{
  XMLerApplication app( argc, argv );
  
  /* set utf-8 codec for tr */
  QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForTr(utfCodec);
  
  MainWindow *mainWindow = 0;
  if ( !app.hasFilesInParams() ) {
    mainWindow = new MainWindow;
    mainWindow->show();
  }
  else
    app.openFiles();

  int result = app.exec();
  
  if ( mainWindow )
    delete mainWindow;
  return result;
}
