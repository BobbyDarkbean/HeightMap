cmake_minimum_required (VERSION 2.8.11)

set (LIB hghtmap-io)
project (${LIB})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Core REQUIRED)

include_directories (../hm)
include_directories (../xbase)

set (HMIO_MAJOR 1)
set (HMIO_MINOR 0)
set (HMIO_PATCH 0)

set (HMIO_VERSION ${HMIO_MAJOR}.${HMIO_MINOR}.${HMIO_PATCH})
set (HMIO_SOVERSION ${HMIO_MAJOR}.${HMIO_MINOR})

set (HMIO_HDR heightmapbinaryiohandler.h
              heightmapiohandler.h
              io_shared.h
              iohandlers
              landscapetextiohandler.h
              peakstextiohandler.h)

set (HMIO_SRC heightmapbinaryiohandler.cpp
              heightmapiohandler.cpp
              landscapetextiohandler.cpp
              peakstextiohandler.cpp)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pedantic -s -std=c++0x")
endif ()
add_definitions (-DHEIGHTMAPIO_LIB)

add_library (${LIB} SHARED ${HMIO_SRC} ${HMIO_HDR})
set_target_properties (${LIB} PROPERTIES
                              VERSION ${HMIO_VERSION}
                              SOVERSION ${HMIO_SOVERSION})
target_link_libraries (${LIB} Qt5::Core)
target_link_libraries (${LIB} hghtmap-core)
target_link_libraries (${LIB} hghtmap-x)
