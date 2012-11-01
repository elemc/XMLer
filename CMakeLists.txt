# -*- CMake -*-
# ------------------------------------ #
# CMake file                           #
# Author: Alexei Panov <me@elemc.name> #
# ------------------------------------ #
# Project: XMLer
# Description:
#

cmake_minimum_required (VERSION 2.6)

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
set (PROJECT_VERSION 0.1)
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
    elseif ( NOT APPLE )
        set ( TRANSLATIONS_DIR ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/translations )
    elseif ( APPLE )
        set ( TRANSLATIONS_DIR Resource/translations )
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
configure_file (
  "${PROJECT_SOURCE_DIR}/tools/Info.plist.in"
  "${PROJECT_BINARY_DIR}/Info.plist"
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
elseif ( APPLE )
    set ( GUI_TYPE MACOSX_BUNDLE )
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

if ( APPLE )
    # MACOSX sets
    set (MACOSX_BUNDLE_ICON_FILE "${PROJECT_NAME}")
    set (MACOSX_BUNDLE_INFO_STRING "The simple XML viewer")
    set (MACOSX_BUNDLE_GUI_IDENTIFIER "name.elemc.${PROJECT_NAME}")
    set (MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}")
    set (MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
    set (MACOSX_BUNDLE_VERSION "${PROJECT_VERSION}")
    set (MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}")

    set (BUNDLE_DIR ${MACOSX_BUNDLE_BUNDLE_NAME}.app)
    set (CMAKE_INSTALL_PREFIX ${BUNDLE_DIR}/Contents)

    add_custom_command (TARGET ${PROJECT_NAME}
        COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/Resources/translations
        COMMAND install -m 644 ${CMAKE_SOURCE_DIR}/icons/${PROJECT_NAME}.icns ${CMAKE_INSTALL_PREFIX}/Resources/
        COMMAND install -m 644 ${QM} ${CMAKE_INSTALL_PREFIX}/Resources/translations/
        COMMAND macdeployqt ${BUNDLE_DIR})

    set (DMG_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
    add_custom_target(dmg
        COMMAND mkdir -p ${DMG_DIR}
        COMMAND cp -R ${BUNDLE_DIR} ${DMG_DIR}/
        COMMAND ln -sf /Applications ${DMG_DIR}
        COMMAND cp ${CMAKE_SOURCE_DIR}/icons/${PROJECT_NAME}.icns .VolumeIcon.icns
        COMMAND hdiutil create ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dmg -srcfolder ${DMG_DIR}
        DEPENDS ${PROJECT_NAME}
        COMMENT "Create DMG image"
    )
endif ( APPLE )

