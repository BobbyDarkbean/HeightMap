cmake_minimum_required (VERSION 2.8)

set (LIB hghtmap-core)
project (${LIB})

message ("Project: ${PROJECT_NAME}")

set (HMCORE_MAJOR 1)
set (HMCORE_MINOR 2)
set (HMCORE_PATCH 0)

set (HMCORE_VERSION ${HMCORE_MAJOR}.${HMCORE_MINOR}.${HMCORE_PATCH})
set (HMCORE_SOVERSION ${HMCORE_MAJOR}.${HMCORE_MINOR})

set (HMCORE_HDR event.h
                hm_shared.h
                peakinfo.h
                line2dsegment.h
                landscape.h
                )
set (HMCORE_SRC landscape.cpp)

set (CMAKE_CXX_FLAGS "-Wall -O3 -pedantic -s")
add_definitions (-DHEIGHTMAP_LIB)

add_library (${LIB} SHARED ${HMCORE_SRC})
set_target_properties (${LIB} PROPERTIES
                              VERSION ${HMCORE_VERSION}
                              SOVERSION ${HMCORE_SOVERSION})
