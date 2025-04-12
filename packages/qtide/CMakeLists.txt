# -*- cmake -*-

IF (WITH_QTLUA_IDE)
  INCLUDE(QtLuaMacros)

  # Find Qt6 components
  find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    WebEngineWidgets
    Network
    PrintSupport
  )

  # WebEngine support
  ADD_DEFINITIONS("-DHAVE_QTWEBENGINE=1")

  # Include directories
  INCLUDE_DIRECTORIES(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${QTLUA_INCLUDE_DIR}"
    "${LUA_INCLUDE_DIR}"
  )

  # Find QLua
  FIND_PACKAGE(QLua REQUIRED)
  INCLUDE_DIRECTORIES(${QLUA_INCLUDE_DIR})
  ADD_DEFINITIONS(${QLUA_DEFINITIONS})
  
  SET(qtide_SRC
    qtide.h qtide.cpp qtide.qrc
    qluatextedit.h qluatextedit.cpp
    qluagotodialog.ui qluafinddialog.ui qluareplacedialog.ui
    qluaide.h qluaide.cpp
    qluamainwindow.h qluamainwindow.cpp
    qluabrowser.h qluabrowser.cpp
    qluaeditor.h qluaeditor.cpp
    qluasdimain.h qluasdimain.cpp
    qluamdimain.h qluamdimain.cpp
    qluamode.h qluamode.cpp
    qluamode_txt.cpp
    qluamode_hlp.cpp
    qluamode_lua.cpp
    qluamode_c.cpp
  )
  
  # Qt6's automoc handles the source generation
  MACRO_ADD_QTLUA_MODULE(libqtide ${qtide_SRC})
  TARGET_LINK_LIBRARIES(libqtide
    ${QLUA_LIBRARIES}
    Qt6::Network
    Qt6::WebEngineWidgets
    Qt6::PrintSupport
  )
  
  MACRO_INSTALL_QTLUA_FILES(qtide init.lua prefs.lua prefs.ui)

ENDIF(WITH_QTLUA_IDE)
