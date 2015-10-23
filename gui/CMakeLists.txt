cmake_minimum_required (VERSION 2.8.11)

set (APP hm-gui)
project (${APP})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../hm)
include_directories (../io)
include_directories (../ge)
include_directories (../xbase)

set (HMGUI_HDR auxiliary/colormap.h
               auxiliary/mappingdata.h
               cmd/contouringcommand.h
               cmd/extrapolatecommand.h
               cmd/generatecommand.h
               commands
               engraver.h
               heightmapapplication.h
               heightmaplogic.h
               heightmapviewmode.h
               heightmapwindow.h
               mappingthread.h
               mappingworker.h
               trigger.h)

set (HMGUI_SRC cmd/contouringcommand.cpp
               cmd/extrapolatecommand.cpp
               cmd/generatecommand.cpp
               engraver.cpp
               heightmapapplication.cpp
               heightmaplogic.cpp
               heightmapwindow.cpp
               main.cpp
               mappingthread.cpp
               mappingworker.cpp
               trigger.cpp)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pedantic -s -std=c++0x")
    if (UNIX)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
    endif ()
endif ()

add_executable (${APP} WIN32 ${HMGUI_SRC} ${HMGUI_HDR})
target_link_libraries (${APP} Qt5::Widgets)
target_link_libraries (${APP} hghtmap-core)
target_link_libraries (${APP} hghtmap-x)
target_link_libraries (${APP} hghtmap-io)
target_link_libraries (${APP} hghtmap-ge)
