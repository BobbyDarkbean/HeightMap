cmake_minimum_required (VERSION 2.8.11)

set (APP hm-gui)
project (${APP})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../hm)

set (HMGUI_HDR mappingworker.h)
set (HMGUI_SRC main.cpp
               mappingworker.cpp)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

set (CMAKE_CXX_FLAGS "-Wall -O3 -pedantic -s -std=c++11 ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_executable (${APP} WIN32 ${HMGUI_SRC} ${HMGUI_HDR})
target_link_libraries (${APP} Qt5::Widgets)
target_link_libraries (${APP} hghtmap-core)
