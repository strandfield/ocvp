
add_subdirectory(drawcontour)
add_subdirectory(drawframe)
add_subdirectory(solvepnp)

set(BUILD_QT_GUI FALSE CACHE BOOL "Build the Qt GUI")

if(BUILD_QT_GUI)

  add_subdirectory(qtgui)

endif()


