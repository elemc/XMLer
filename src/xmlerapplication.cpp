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
  selectIconTheme ();
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
  int opened = 0;
  for ( int i = 0; i < autoOpenFiles.count(); i++ ) {
    MainWindow *mw = new MainWindow;
    mw->show();
    bool ok = mw->loadDocument( autoOpenFiles.at(i) );
    opened += 1;
  }

  if ( opened == 0 ) {
    MainWindow *mw = new MainWindow;
    mw->show();
    return;
  }
}
void XMLerApplication::selectIconTheme ()
{
  QString generic_icon_to_check = "code-context";
  QString fallback_icon_theme = "oxygen-internal";

  if ( !QIcon::hasThemeIcon( generic_icon_to_check ) )
    QIcon::setThemeName( fallback_icon_theme );
}
