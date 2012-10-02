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
      QFile paramFile(param);
      if ( paramFile.exists() )
        autoOpenFiles.append( QString(param) );
    }
  }
}

XMLerApplication::~XMLerApplication ()
{
  qDeleteAll(openedWindows.begin(), openedWindows.end());
}

bool XMLerApplication::hasFilesInParams() const
{
  return ( autoOpenFiles.size() > 0 );
}

void XMLerApplication::openFiles ()
{
  int opened = 0;
  for ( int i = 0; i < autoOpenFiles.count(); i++ ) {
    MainWindow *mw = new MainWindow;
    bool ok = mw->loadDocument( autoOpenFiles.at(i) );
    if ( ok ) {
      mw->show();
      openedWindows.append(mw);
      opened += 1;
    }
    else {
      delete mw;
    }
  }

  if ( opened == 0 ) {
    MainWindow *mw = new MainWindow;
    mw->show();
    return;
  }
}
