# -*- cmake -*-

INCLUDE(QtLuaMacros)
find_package(Qt6 REQUIRED COMPONENTS Core)
INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}"
  "${QTLUA_INCLUDE_DIR}"
  "${LUA_INCLUDE_DIR}"
)

SET(qtcore_SRC qtcore.h qtcore.cpp)

MACRO_INSTALL_QTLUA_FILES(qtcore init.lua)

MACRO_ADD_QTLUA_MODULE(libqtcore ${qtcore_SRC})

#ADD_TORCH_DOK(dok qtcore "QT Interface" "Core interface" 6.2)
