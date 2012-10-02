// -*- C++ -*-
/* ------------------------------------ */
/* C++ class source (QApplication)      */
/* Author: Alexei Panov <me@elemc.name> */
/* ------------------------------------ */
/* Name: XMLerApplication
   Description:
*/
#include "xmlerapplication.h"

XMLerApplication::XMLerApplication ( int & argc, char ** argv ) :
  QApplication(argc, argv)
{
  autoOpenFiles.clear();
  if ( argc > 1) {
    for( int n = 1; n < argc; n++ ) {
      char *param = *(argv+n);
      qDebug() << param;
      QFile paramFile(param);
      if ( paramFile.exists() )
        autoOpenFiles.append( QString(param) );
    }
  }
}

XMLerApplication::~XMLerApplication ()
{
}

bool XMLerApplication::hasFilesInParams() const
{
  return ( autoOpenFiles.size() > 0 );
}

void XMLerApplication::openFiles ()
{
  for ( int i = 0; i < autoOpenFiles.count(); i++ ) {
    MainWindow *mw = new MainWindow;
    bool ok = mw->loadDocument( autoOpenFiles.at(i) );
    if ( ok )
      mw->show();
    else {
      delete mw;
    }
  }
}
