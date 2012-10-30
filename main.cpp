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
#include <QtCore/QStringList>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>
#include "mainwindow.h"

QStringList XMLer_languages ()
{
    QStringList sl;
    sl.append( QString("ru_ru") );

    return sl;
}

int main ( int argc, char **argv )
{
  XMLerApplication app( argc, argv );
  
  /* set utf-8 codec for tr */
  QTextCodec *utfCodec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForTr(utfCodec);

  /* translator */
  QTranslator trans;
  QTranslator qt_trans;
  QLocale locale = QLocale::system();
  if ( XMLer_languages().contains ( locale.name().toLower() ) ) {
    QString trans_name = QString( "XMLer_%1" ).arg ( locale.name() );
    QString qt_name = QString( "qt_%1").arg ( locale.name() );
    trans.load( trans_name, QString( XMLER_TRANSLATIONS_DIR ) );
    qt_trans.load( qt_name, QLibraryInfo::location( QLibraryInfo::TranslationsPath ) );
  }
  app.installTranslator( &trans );
  app.installTranslator( &qt_trans );

  app.openFiles();

  int result = app.exec();
  return result;
}
