# -*- cmake -*-

INCLUDE(QtLuaMacros)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Xml Svg SvgWidgets)

INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${QTLUA_INCLUDE_DIR}"
  "${LUA_INCLUDE_DIR}"
)

SET(qtsvg_SRC
  qtsvg.h qtsvg.cpp
  qtluasvggenerator.h qtluasvggenerator.cpp
)
   
# Qt6's automoc handles the source generation
MACRO_ADD_QTLUA_MODULE(libqtsvg ${qtsvg_SRC})
TARGET_LINK_LIBRARIES(libqtsvg
  Qt6::Xml
  Qt6::Svg
  Qt6::SvgWidgets
)

MACRO_INSTALL_QTLUA_FILES(qtsvg init.lua)
