cmake_minimum_required (VERSION 2.8.11)

set (LIB hghtmap-x)
project (${LIB})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Core REQUIRED)

include_directories (../hm)

set (HMX_MAJOR 1)
set (HMX_MINOR 0)
set (HMX_PATCH 0)

set (HMX_VERSION ${HMX_MAJOR}.${HMX_MINOR}.${HMX_PATCH})
set (HMX_SOVERSION ${HMX_MAJOR}.${HMX_MINOR})

set (HMX_HDR extrapolationdata.h
             extrapolationfactory.h
             preferences.h
             preferencescontroller.h
             xbase_shared.h
             xreader.h
             xwriter.h)

set (HMX_SRC extrapolationdata.cpp
             extrapolationfactory.cpp
             preferences.cpp
             preferencescontroller.cpp
             xreader.cpp
             xwriter.cpp)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pedantic -s -std=c++11")
endif ()
add_definitions (-DHEIGHTMAPX_LIB)

add_library (${LIB} SHARED ${HMX_SRC} ${HMX_HDR})
set_target_properties (${LIB} PROPERTIES
                              VERSION ${HMX_VERSION}
                              SOVERSION ${HMX_SOVERSION})
target_link_libraries (${LIB} Qt5::Core)
target_link_libraries (${LIB} hghtmap-core)
