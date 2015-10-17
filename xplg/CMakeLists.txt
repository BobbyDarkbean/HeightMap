cmake_minimum_required (VERSION 2.8)

message ("Extrapolation plugins...")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/xdata)

add_subdirectory (bsl)
add_subdirectory (fxr)
add_subdirectory (slp)
add_subdirectory (sml)
