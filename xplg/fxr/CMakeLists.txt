cmake_minimum_required (VERSION 2.8.11)

set (LIB hmxplg-fxr)
project (${LIB})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../../hm)
include_directories (../../ge)
include_directories (../../xbase)

set (HMX_FXR_HDR fixedradiusextrapolationfactory.h
                 fixedradiusextrapolationwidget.h
                 fixedradiusextrapolator.h)

set (HMX_FXR_SRC fixedradiusextrapolationfactory.cpp
                 fixedradiusextrapolationwidget.cpp
                 fixedradiusextrapolator.cpp)

set (HMX_FXR_MET fxr.json)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pedantic -s -std=c++0x")
endif ()
add_definitions (-DQT_PLUGIN)

add_library (${LIB} SHARED ${HMX_FXR_SRC} ${HMX_FXR_HDR} ${HMX_FXR_MET})
target_link_libraries (${LIB} Qt5::Widgets)
target_link_libraries (${LIB} hghtmap-core)
target_link_libraries (${LIB} hghtmap-x)
target_link_libraries (${LIB} hghtmap-ge)
