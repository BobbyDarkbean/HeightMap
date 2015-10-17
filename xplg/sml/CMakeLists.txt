cmake_minimum_required (VERSION 2.8.11)

set (LIB hmxplg-sml)
project (${LIB})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../../hm)
include_directories (../../ge)
include_directories (../../xbase)

set (HMX_SML_HDR simpleextrapolationfactory.h
                 simpleextrapolationwidget.h
                 simpleextrapolator.h)

set (HMX_SML_SRC simpleextrapolationfactory.cpp
                 simpleextrapolationwidget.cpp
                 simpleextrapolator.cpp)

set (HMX_SML_MET sml.json)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pedantic -s -std=c++11")
endif ()
add_definitions (-DQT_PLUGIN)

add_library (${LIB} SHARED ${HMX_SML_SRC} ${HMX_SML_HDR} ${HMX_SML_MET})
target_link_libraries (${LIB} Qt5::Widgets)
target_link_libraries (${LIB} hghtmap-core)
target_link_libraries (${LIB} hghtmap-x)
target_link_libraries (${LIB} hghtmap-ge)
