cmake_minimum_required (VERSION 2.8.11)

set (LIB hghtmap-ge)
project (${LIB})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../hm)
include_directories (../xbase)

set (HMGE_MAJOR 1)
set (HMGE_MINOR 0)
set (HMGE_PATCH 0)

set (HMGE_VERSION ${HMGE_MAJOR}.${HMGE_MINOR}.${HMGE_PATCH})
set (HMGE_SOVERSION ${HMGE_MAJOR}.${HMGE_MINOR})

set (HMGE_HDR dialogs/contouringoptionsdialog.h
              dialogs/extrapolationoptionsdialog.h
              dialogs/generatingoptionsdialog.h
              dialogs/terrainoptionsdialog.h
              widgets/abstractextrapolationwidget.h
              widgets/contouringoptionswidget.h
              widgets/extrapolationoptionswidget.h
              widgets/landscapeoptionswidget.h
              widgets/peakoptionswidget.h
              ge_shared.h
              guielements)

set (HMGE_SRC dialogs/contouringoptionsdialog.cpp
              dialogs/extrapolationoptionsdialog.cpp
              dialogs/generatingoptionsdialog.cpp
              dialogs/terrainoptionsdialog.cpp
              widgets/abstractextrapolationwidget.cpp
              widgets/contouringoptionswidget.cpp
              widgets/extrapolationoptionswidget.cpp
              widgets/landscapeoptionswidget.cpp
              widgets/peakoptionswidget.cpp)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pedantic -s -std=c++11")
endif ()
add_definitions (-DHEIGHTMAPGE_LIB)

add_library (${LIB} SHARED ${HMGE_SRC} ${HMGE_HDR})
set_target_properties (${LIB} PROPERTIES
                              VERSION ${HMGE_VERSION}
                              SOVERSION ${HMGE_SOVERSION})
target_link_libraries (${LIB} Qt5::Widgets)
target_link_libraries (${LIB} hghtmap-core)
target_link_libraries (${LIB} hghtmap-x)
