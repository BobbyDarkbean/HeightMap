cmake_minimum_required (VERSION 2.8)

set (LIB hghtmap-core)
project (${LIB})

message ("Project: ${PROJECT_NAME}")

set (HMCORE_MAJOR 1)
set (HMCORE_MINOR 2)
set (HMCORE_PATCH 0)

set (HMCORE_VERSION ${HMCORE_MAJOR}.${HMCORE_MINOR}.${HMCORE_PATCH})
set (HMCORE_SOVERSION ${HMCORE_MAJOR}.${HMCORE_MINOR})

set (HMCORE_HDR internal/contouring.h
                event.h
                eventservice
                extrapolator.h
                hm_shared.h
                landscape.h
                line2dsegment.h
                mapper.h
                peakgenerationoptions.h
                peakinfo.h
                terrain.h)
set (HMCORE_SRC extrapolator.cpp
                landscape.cpp
                mapper.cpp
                terrain.cpp)

if (UNIX OR MINGW)
    set (CMAKE_CXX_FLAGS "-Wall -O3 -pedantic -s -std=c++11")
endif ()
add_definitions (-DHEIGHTMAP_LIB)

add_library (${LIB} SHARED ${HMCORE_SRC} ${HMCORE_HDR})
set_target_properties (${LIB} PROPERTIES
                              VERSION ${HMCORE_VERSION}
                              SOVERSION ${HMCORE_SOVERSION})
