cmake_minimum_required (VERSION 2.8)

if (WIN32)
    add_definitions (-DOS_WIN)
endif ()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
add_subdirectory (hm)
add_subdirectory (xbase)
add_subdirectory (io)
add_subdirectory (ge)
add_subdirectory (gui)
add_subdirectory (xplg)
