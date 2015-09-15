cmake_minimum_required (VERSION 2.8.11)

set (APP hm-gui)
project (${APP})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../hm)

set (HMGUI_HDR colormap.h
               engraver.h
               heightmapapplication.h
               heightmapwindow.h
               mappingdata.h
               mappingworker.h
               preferences.h)
set (HMGUI_SRC engraver.cpp
               heightmapapplication.cpp
               heightmapwindow.cpp
               main.cpp
               mappingworker.cpp
               preferences.cpp)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (${UNIX} OR ${MINGW})
    set (CMAKE_CXX_FLAGS "-Wall -O3 -pedantic -s -std=c++11 ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
endif ()

add_executable (${APP} WIN32 ${HMGUI_SRC} ${HMGUI_HDR})
target_link_libraries (${APP} Qt5::Widgets)
target_link_libraries (${APP} hghtmap-core)
