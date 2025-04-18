# QtLua CMake Macros

# Enable Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Macro to install QtLua files
MACRO(MACRO_INSTALL_QTLUA_FILES _package)
  INSTALL(FILES ${ARGN}
    DESTINATION "${QtLua_INSTALL_LUA_PATH_SUBDIR}/${_package}")
ENDMACRO(MACRO_INSTALL_QTLUA_FILES)

# Macro to add QtLua module
MACRO(MACRO_ADD_QTLUA_MODULE _name)
  ADD_LIBRARY(${_name} SHARED ${ARGN})
  TARGET_LINK_LIBRARIES(${_name}
    ${LUA_LIBRARIES}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineWidgets
    libqtlua
  )
  SET_TARGET_PROPERTIES(${_name} PROPERTIES
    PREFIX ""
    IMPORT_PREFIX ""
  )
  
  INSTALL(TARGETS ${_name}
    RUNTIME DESTINATION "${QtLua_INSTALL_LUA_CPATH_SUBDIR}"
    LIBRARY DESTINATION "${QtLua_INSTALL_LUA_CPATH_SUBDIR}"
  )
ENDMACRO(MACRO_ADD_QTLUA_MODULE)