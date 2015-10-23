cmake_minimum_required (VERSION 2.8.11)

set (LIB hmxplg-slp)
project (${LIB})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../../hm)
include_directories (../../ge)
include_directories (../../xbase)

set (HMX_SLP_HDR slopeextrapolationfactory.h
                 slopeextrapolationwidget.h
                 slopeextrapolator.h)

set (HMX_SLP_SRC slopeextrapolationfactory.cpp
                 slopeextrapolationwidget.cpp
                 slopeextrapolator.cpp)

set (HMX_SLP_MET slp.json)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pedantic -s -std=c++0x")
endif ()
add_definitions (-DQT_PLUGIN)

add_library (${LIB} SHARED ${HMX_SLP_SRC} ${HMX_SLP_HDR} ${HMX_SLP_MET})
target_link_libraries (${LIB} Qt5::Widgets)
target_link_libraries (${LIB} hghtmap-core)
target_link_libraries (${LIB} hghtmap-x)
target_link_libraries (${LIB} hghtmap-ge)
