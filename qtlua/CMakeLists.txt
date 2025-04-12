# -*- cmake -*-

SET(QTLUA_DEFINITIONS)
SET(QTLUA_INCLUDE_DIR 
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}")

ADD_DEFINITIONS(${QTLUA_DEFINITIONS} ${LUA_DEFINITIONS})
INCLUDE_DIRECTORIES(${QTLUA_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

# Qt6 setup
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

CONFIGURE_FILE("qtluaconf.h.in" "${CMAKE_CURRENT_BINARY_DIR}/qtluaconf.h")

# --- compile library

SET(qtlua_SRC "qtluautils.h" "qtluautils.cpp" "qtluaengine.h" "qtluaengine.cpp")

# Use Qt6's automoc instead of Qt4's autogen
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

ADD_LIBRARY(libqtlua SHARED ${qtlua_SRC})
TARGET_LINK_LIBRARIES(libqtlua PRIVATE
  ${LUA_LIBRARIES}
  Qt6::Core
  Qt6::Widgets
)

# Add include directories for Lua
target_include_directories(libqtlua PRIVATE
  ${LUA_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
SET_TARGET_PROPERTIES(libqtlua PROPERTIES 
  LINKER_LANGUAGE CXX 
  OUTPUT_NAME "qtlua")

# --- install library and include files

INSTALL(TARGETS libqtlua
  RUNTIME DESTINATION ${QtLua_INSTALL_BIN_SUBDIR} 
  LIBRARY DESTINATION ${QtLua_INSTALL_LIB_SUBDIR} 
  ARCHIVE DESTINATION ${QtLua_INSTALL_LIB_SUBDIR})

INSTALL(FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/qtluaengine.h
  ${CMAKE_CURRENT_SOURCE_DIR}/qtluautils.h
  ${CMAKE_CURRENT_BINARY_DIR}/qtluaconf.h
  DESTINATION ${QtLua_INSTALL_INCLUDE_SUBDIR}/qtlua
)

# --- config for internal use

SET(QTLUA_LIBRARIES "libqtlua")
SET(QTLUA_DEFINITIONS)
CONFIGURE_FILE(QtLuaConfig.cmake.in 
  "${QtLua_BINARY_DIR}/cmake/QtLuaConfig.cmake")
SET(QtLua_DIR "${QtLua_BINARY_DIR}/cmake" CACHE PATH 
  "Directory containing QtLuaConfig.cmake")
MARK_AS_ADVANCED(QtLua_DIR)

# --- config for external use

SET(QTLUA_OUTPUT_NAME "$<TARGET_FILE_NAME:libqtlua>")
SET(QTLUA_LIBRARIES "${QtLua_INSTALL_LIB}/${QTLUA_OUTPUT_NAME}")
SET(QTLUA_INCLUDE_DIR "${QtLua_INSTALL_INCLUDE}/qtlua")
CONFIGURE_FILE("QtLuaConfig.cmake.in" 
  "${QtLua_BINARY_DIR}/cmake-external/QtLuaConfig.cmake")
INSTALL(FILES "${QtLua_BINARY_DIR}/cmake-external/QtLuaConfig.cmake" 
  DESTINATION "${QtLua_INSTALL_CMAKE_SUBDIR}")
