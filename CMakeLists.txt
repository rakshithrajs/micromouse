cmake_minimum_required(VERSION 3.10)
project(MicroMouse)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_subdirectory(simulation)

add_executable(micromouse_gui
    main.cpp
    gui/mainwindow.cpp
)

target_include_directories(micromouse_gui PRIVATE simulation)

target_link_libraries(micromouse_gui
    Qt5::Widgets
    simulation_lib
)
