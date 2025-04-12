# -*- cmake -*-

INCLUDE(QtLuaMacros)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets)

# WebEngine support
ADD_DEFINITIONS("-DHAVE_QTWEBENGINE=1")

INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${QTLUA_INCLUDE_DIR}"
  "${LUA_INCLUDE_DIR}"
)

SET(qtgui_SRC qtgui.h qtgui.cpp qtluagui.h qtluagui.cpp)

MACRO_INSTALL_QTLUA_FILES(qtgui init.lua)

# Qt6's automoc handles the source generation
MACRO_ADD_QTLUA_MODULE(libqtgui ${qtgui_SRC})

#ADD_TORCH_DOK(dok qtgui "QT Interface" "GUI" 6.)
