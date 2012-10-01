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

set (PROJECT_NAME XMLer)
set (PROJECT_VERSION 0.0.1)
set (HEADERS 
  src/mainwindow.h
  src/basexmlnode.h
  src/elementxmlnode.h
  src/documentxmlnode.h
  src/attrxmlnode.h
  src/dataxmlnode.h
  src/xmlerhandler.h
  src/xmlermodel.h)
set (SOURCES 
  main.cpp 
  src/mainwindow.cpp
  src/basexmlnode.cpp
  src/elementxmlnode.cpp
  src/documentxmlnode.cpp
  src/attrxmlnode.cpp
  src/dataxmlnode.cpp
  src/xmlerhandler.cpp
  src/xmlermodel.cpp)
set (FORMS 
  forms/mainwindow.ui)
set (RESOURCES xmler.qrc)
set (DOC_FILES README)

set (QT_USE_QTXML TRUE)
set (QT_USE_QTNETWORK FALSE)

project (${PROJECT_NAME})

configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
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

# Compile RC-file, only for Windows
if (WIN32)
  set (GUI_TYPE WIN32)
  set (RC_FILE ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.rc)
  ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.o
	COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc 
	-o ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.o )
endif (WIN32)

add_executable (${PROJECT_NAME} ${GUI_TYPE} ${HEADERS} ${SOURCES} ${UI_SOURCES} ${MOC_SOURCES} ${QRC_SOURCES} ${RC_FILE})
target_link_libraries (${PROJECT_NAME} ${QT_LIBRARIES})

install (TARGETS ${PROJECT_NAME} DESTINATION bin)
