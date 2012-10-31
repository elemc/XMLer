# -*- CMake -*-
# ------------------------------------ #
# CMake file                           #
# Author: Alexei Panov <me@elemc.name> #
# ------------------------------------ #
# Project: XMLer
# Description:
#

cmake_minimum_required (VERSION 2.6)

if (NOT CMAKE_INSTALL_PREFIX)
  set (CMAKE_INSTALL_PREFIX /usr/local)
endif ()

if (NOT DEBUG)
  if ( CMAKE_BUILD_TYPE STREQUAL Debug )
    set (DEBUG 1)
  else ( CMAKE_BUILD_TYPE STREQUAL Debug )
    set (DEBUG 0)
  endif ( CMAKE_BUILD_TYPE STREQUAL Debug )
endif (NOT DEBUG)

if ( NOT DEFAULT_ENCODING )
  set ( DEFAULT_ENCODING "UTF-8" )
endif ( NOT DEFAULT_ENCODING )

set (PROJECT_NAME XMLer)
set (PROJECT_VERSION 0.0.1)
set (HEADERS 
  src/xmlerapplication.h
  src/mainwindow.h
  src/documentpropertiesdialog.h
  src/xmlerfindwidget.h
  src/xmlermodel.h
  src/xmlerinputsource.h
  src/xmlerfileoperthread.h
  src/xmlerloadfilethread.h
  src/xmlersavefilethread.h
  src/xmlerfindthread.h
  src/basexmlnode.h
  src/elementxmlnode.h
  src/documentxmlnode.h
  src/attrxmlnode.h
  src/dataxmlnode.h)
set (SOURCES 
  main.cpp 
  src/xmlerapplication.cpp
  src/mainwindow.cpp
  src/documentpropertiesdialog.cpp
  src/xmlerfindwidget.cpp
  src/xmlermodel.cpp
  src/xmlerhandler.cpp
  src/xmlerexception.cpp
  src/xmlerinputsource.cpp
  src/xmlerfileoperthread.cpp
  src/xmlerloadfilethread.cpp
  src/xmlersavefilethread.cpp
  src/xmlerfindthread.cpp
  src/basexmlnode.cpp
  src/elementxmlnode.cpp
  src/documentxmlnode.cpp
  src/attrxmlnode.cpp
  src/dataxmlnode.cpp)
set (FORMS 
  forms/mainwindow.ui
  forms/documentpropertiesdialog.ui)
set (RESOURCES xmler.qrc)
set (DOC_FILES README)
set (TRANS_FILES translations/XMLer_ru.ts)
set (DESKTOP_FILE ${CMAKE_BINARY_DIR}/XMLer.desktop)
set (ICON_SIZES
  16x16
  22x22
  32x32
  48x48
  64x64
  128x128
  256x256)
if ( NOT TRANSLATIONS_DIR )
    if ( CMAKE_BUILD_TYPE STREQUAL Debug )
        set ( TRANSLATIONS_DIR "." )
    else ( CMAKE_BUILD_TYPE STREQUAL Debug )
        set ( TRANSLATIONS_DIR ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/translations )
    endif ( CMAKE_BUILD_TYPE STREQUAL Debug )
endif ( NOT TRANSLATIONS_DIR )
message (STATUS "Translations directory set to: ${TRANSLATIONS_DIR}")

set (QT_USE_QTXML TRUE)
set (QT_USE_QTNETWORK FALSE)

project (${PROJECT_NAME})

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
configure_file (
  "${PROJECT_SOURCE_DIR}/tools/${PROJECT_NAME}.desktop.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.desktop"
  )

include_directories (${CMAKE_CURRENT_BINARY_DIR} src)
if (WIN32)
  find_package (Qt4 COMPONENTS QtMain QtCore QtGui QtXml REQUIRED)
else (WIN32)
  find_package (Qt4 COMPONENTS QtCore QtGui QtXml REQUIRED)
endif (WIN32)

include (${QT_USE_FILE})
qt4_add_resources (QRC_SOURCES ${RESOURCES})
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
qt4_wrap_ui (UI_SOURCES ${FORMS})
source_group ("Header Files" FILES ${HEADERS})
source_group ("Source Files" FILES ${SOURCES})
source_group ("Forms Files" FILES ${UI_SOURCES})
source_group ("Generated Files" FILES ${MOC_SOURCES})
source_group ("Resource Files" FILES ${QRC_SOURCES})

# Translations
QT4_ADD_TRANSLATION(QM ${TRANS_FILES})

# Compile RC-file, only for Windows
if (WIN32)
  set (GUI_TYPE WIN32)
  set (RC_FILE ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.rc)
  ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.o
	COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc 
	-o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.o )
endif (WIN32)

add_executable (${PROJECT_NAME} ${GUI_TYPE} ${HEADERS} ${SOURCES} ${UI_SOURCES} ${MOC_SOURCES} ${QRC_SOURCES} ${RC_FILE} ${QM})
target_link_libraries (${PROJECT_NAME} ${QT_LIBRARIES})

if (UNIX AND NOT APPLE)
    install (TARGETS ${PROJECT_NAME} DESTINATION bin)
    install(FILES ${QM} DESTINATION share/${PROJECT_NAME}/translations)
    install(FILES ${DESKTOP_FILE} DESTINATION share/applications)

    foreach (ICON_SIZE ${ICON_SIZES})
        install (
            FILES icons/${ICON_SIZE}/${PROJECT_NAME}.png 
            DESTINATION share/icons/hicolor/${ICON_SIZE}/apps)
    endforeach ()
endif (UNIX AND NOT APPLE)

