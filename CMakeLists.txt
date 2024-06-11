cmake_minimum_required(VERSION 3.16)
project(IRTransPlugin VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network Widgets)

qt_standard_project_setup()

qt_add_plugin(IRTransPlugin)
target_sources(IRTransPlugin PRIVATE
    IRTrans.ui
    IRTransPlugin.cpp IRTransPlugin.h
)
target_link_libraries(IRTransPlugin PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Widgets
)

install(TARGETS IRTransPlugin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
