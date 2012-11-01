XMLer
=====

The simple XML viewer.
Application for view XML files in tree-look. You may search in XML. Set bookmarks. Save XML in another encoding and/or with another formatter.

Installation
------------

### Requires
- C/C++ compiler;
- cmake >= 2.6 ( http://www.cmake.org );
- Qt >= 4.5 ( http://qt-project.org );
- git - if you have download source from git (http://git-scm.com/).

### Download ( git )
For git version:
$ git clone git://github.com/elemc/XMLer.git

For zip version:
https://github.com/elemc/XMLer/zipball/master

For tar.gz version:
https://github.com/elemc/XMLer/tarball/master

### Configure
cd source root directory
$ mkdir build 
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..

### UNIX build (not OSX)
$ gmake                     # this command is compile sources
$ su -c 'gmake install'     # and this command install binaries
or
$ sudo gmake install

### Mac OS X
$ make                      # this command compile the sources and make app bundle
$ make dmg                  # this command make DMG image

### Windows (MS C++ Compiler)
$ nmake
$ nmake install             # this command create directory build/Release with executable, libraries and translations

### Windows (MinGW)
$ make
$ make install              # this command create directory build/Release with executable, libraries and translations

### cmake local variables
- DEBUG                     - enable/disable #include <QDebug> (automatic enable/disable by CMAKE_BUILD_TYPE)
- DEFUALT_ENCODING          - set default encoding for application
- TRANSLATIONS_DIR          - set directory for local application translation files (only UNIX, not OSX)

Bugs
----
Please send me bugs and feature request here https://github.com/elemc/XMLer/issues

TODO
----
I hope will enough time to make editing XML in the application.

Thanks
------
- to perchibald for main icon;
- to the authors icon theme "oxygen" (http://www.oxygen-icons.org).
