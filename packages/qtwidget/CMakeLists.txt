# -*- cmake -*-

INCLUDE(QtLuaMacros)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets PrintSupport)

INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${QTLUA_INCLUDE_DIR}"
  "${LUA_INCLUDE_DIR}"
)

SET(qtwidget_SRC
  qtluapainter.h qtluapainter.cpp
  qtluaprinter.h qtluaprinter.cpp
  qtlualistener.h qtlualistener.cpp
  qtwidget.h qtwidget.cpp
)

MACRO_INSTALL_QTLUA_FILES(qtwidget init.lua)

# Qt6's automoc handles the source generation
MACRO_ADD_QTLUA_MODULE(libqtwidget ${qtwidget_SRC})
TARGET_LINK_LIBRARIES(libqtwidget
  Qt6::Gui
  Qt6::Widgets
  Qt6::PrintSupport
)
