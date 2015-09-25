cmake_minimum_required (VERSION 2.8.11)

set (APP hm-gui)
project (${APP})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../hm)

set (HMGUI_HDR dialogs/contouringoptionsdialog.h
               dialogs/extrapolationoptionsdialog.h
               dialogs/generatingoptionsdialog.h
               dialogs/terrainoptionsdialog.h
               extrapolation/baselevelextrapolationfactory.h
               extrapolation/extrapolationfactory.h
               extrapolation/fixedradiusextrapolationfactory.h
               extrapolation/simpleextrapolationfactory.h
               extrapolation/slopeextrapolationfactory.h
               widgets/abstractextrapolationwidget.h
               widgets/baselevelextrapolationwidget.h
               widgets/contouringoptionswidget.h
               widgets/extrapolationoptionswidget.h
               widgets/fixedradiusextrapolationwidget.h
               widgets/landscapeoptionswidget.h
               widgets/peakoptionswidget.h
               widgets/simpleextrapolationwidget.h
               widgets/slopeextrapolationwidget.h
               colormap.h
               engraver.h
               heightmapapplication.h
               heightmapwindow.h
               mappingdata.h
               mappingthread.h
               mappingworker.h
               preferences.h)
set (HMGUI_SRC dialogs/contouringoptionsdialog.cpp
               dialogs/extrapolationoptionsdialog.cpp
               dialogs/generatingoptionsdialog.cpp
               dialogs/terrainoptionsdialog.cpp
               extrapolation/baselevelextrapolationfactory.cpp
               extrapolation/extrapolationfactory.cpp
               extrapolation/fixedradiusextrapolationfactory.cpp
               extrapolation/simpleextrapolationfactory.cpp
               extrapolation/slopeextrapolationfactory.cpp
               widgets/abstractextrapolationwidget.cpp
               widgets/baselevelextrapolationwidget.cpp
               widgets/contouringoptionswidget.cpp
               widgets/extrapolationoptionswidget.cpp
               widgets/fixedradiusextrapolationwidget.cpp
               widgets/landscapeoptionswidget.cpp
               widgets/peakoptionswidget.cpp
               widgets/simpleextrapolationwidget.cpp
               widgets/slopeextrapolationwidget.cpp
               engraver.cpp
               heightmapapplication.cpp
               heightmapwindow.cpp
               main.cpp
               mappingthread.cpp
               mappingworker.cpp
               preferences.cpp)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (UNIX OR MINGW)
    set (CMAKE_CXX_FLAGS "-Wall -O3 -pedantic -s -std=c++11 ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
endif ()

add_executable (${APP} WIN32 ${HMGUI_SRC} ${HMGUI_HDR})
target_link_libraries (${APP} Qt5::Widgets)
target_link_libraries (${APP} hghtmap-core)
