# -*- cmake -*-

INCLUDE(QtLuaMacros)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Xml UiTools)

INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${QTLUA_INCLUDE_DIR}"
  "${LUA_INCLUDE_DIR}"
)

SET(qtuiloader_SRC uiloader.h uiloader.cpp)

MACRO_INSTALL_QTLUA_FILES(qtuiloader init.lua test.lua test.ui)

MACRO_ADD_QTLUA_MODULE(libqtuiloader ${qtuiloader_SRC})
TARGET_LINK_LIBRARIES(libqtuiloader
  Qt6::Xml
  Qt6::UiTools
)
