// -*- C++ -*-
/* ------------------------------------ */
/* C++ source main file                 */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: main.cpp
   Description: This is main file for XMLer project
*/
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>

#include "mainwindow.h"

int main ( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  /* set utf-8 codec for tr */
  QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForTr(utfCodec);
  
  MainWindow mainWindow;
  mainWindow.show();
  int result = app.exec();
  
  //delete utfCodec;
  return result;
}
