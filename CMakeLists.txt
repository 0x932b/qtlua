# -*- cmake -*-

CMAKE_MINIMUM_REQUIRED(VERSION 3.16 FATAL_ERROR)
CMAKE_POLICY(VERSION 3.16)

project(QtLua)

IF(APPLE)
  SET(CMAKE_MACOSX_RPATH TRUE) # @rpath in libs
ENDIF()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

INCLUDE(QtLuaPaths)

# --- General

SET(WITH_QTLUA_IDE ON
  CACHE BOOL "Compile qlua ide")

# --- Qt6

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets OpenGLWidgets Xml Svg)

# --- Lua

# Set Lua version to 5.1
SET(WITH_LUA51 ON CACHE BOOL "Use Lua 5.1")
SET(LUA_VERSION "LUA51" CACHE STRING "Lua version")

# Include Lua Files
FIND_PACKAGE(Lua REQUIRED)
INCLUDE_DIRECTORIES(${LUA_INCDIR})
LINK_DIRECTORIES(${LUA_LIBDIR})

# --- compile libqtlua

ADD_SUBDIRECTORY(qtlua)
FIND_PACKAGE(QtLua REQUIRED)
ADD_SUBDIRECTORY(qlua)
ADD_SUBDIRECTORY(packages)

# --- install qt libraries

IF (WIN32 AND NOT CYGWIN)
  SET(QT_INSTALL_LIBRARIES "OFF"
    CACHE BOOL "Copy the required Qt DLLs into the Torch install dirs")
ENDIF (WIN32 AND NOT CYGWIN)

IF (QT_INSTALL_LIBRARIES)
  # -- this takes care of the install
  INCLUDE("InstallRequiredQtLibraries")
ENDIF (QT_INSTALL_LIBRARIES)
