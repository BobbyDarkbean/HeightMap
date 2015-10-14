cmake_minimum_required (VERSION 2.8.11)

set (APP hm-gui)
project (${APP})

message ("Project: ${PROJECT_NAME}")

find_package (Qt5Widgets REQUIRED)

include_directories (../hm)

set (HMGUI_HDR auxiliary/colormap.h
               auxiliary/mappingdata.h
               commands/commands
               commands/contouringcommand.h
               commands/extrapolatecommand.h
               commands/generatecommand.h
               dialogs/contouringoptionsdialog.h
               dialogs/extrapolationoptionsdialog.h
               dialogs/generatingoptionsdialog.h
               dialogs/terrainoptionsdialog.h
               extrapolation/baselevelextrapolationfactory.h
               extrapolation/extrapolationdata.h
               extrapolation/extrapolationfactory.h
               extrapolation/fixedradiusextrapolationfactory.h
               extrapolation/simpleextrapolationfactory.h
               extrapolation/slopeextrapolationfactory.h
               extrapolation/xreader.h
               extrapolation/xwriter.h
               widgets/abstractextrapolationwidget.h
               widgets/baselevelextrapolationwidget.h
               widgets/contouringoptionswidget.h
               widgets/extrapolationoptionswidget.h
               widgets/fixedradiusextrapolationwidget.h
               widgets/landscapeoptionswidget.h
               widgets/peakoptionswidget.h
               widgets/simpleextrapolationwidget.h
               widgets/slopeextrapolationwidget.h
               engraver.h
               heightmapapplication.h
               heightmaplogic.h
               heightmapviewmode.h
               heightmapwindow.h
               mappingthread.h
               mappingworker.h
               preferences.h
               preferencescontroller.h
               trigger.h)

set (HMGUI_SRC commands/contouringcommand.cpp
               commands/extrapolatecommand.cpp
               commands/generatecommand.cpp
               dialogs/contouringoptionsdialog.cpp
               dialogs/extrapolationoptionsdialog.cpp
               dialogs/generatingoptionsdialog.cpp
               dialogs/terrainoptionsdialog.cpp
               extrapolation/baselevelextrapolationfactory.cpp
               extrapolation/extrapolationdata.cpp
               extrapolation/extrapolationfactory.cpp
               extrapolation/fixedradiusextrapolationfactory.cpp
               extrapolation/simpleextrapolationfactory.cpp
               extrapolation/slopeextrapolationfactory.cpp
               extrapolation/xreader.cpp
               extrapolation/xwriter.cpp
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
               heightmaplogic.cpp
               heightmapwindow.cpp
               main.cpp
               mappingthread.cpp
               mappingworker.cpp
               preferences.cpp
               preferencescontroller.cpp
               trigger.cpp)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -pedantic -s -std=c++11")
    if (UNIX)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
    endif ()
endif ()

add_executable (${APP} WIN32 ${HMGUI_SRC} ${HMGUI_HDR})
target_link_libraries (${APP} Qt5::Widgets)
target_link_libraries (${APP} hghtmap-core)
target_link_libraries (${APP} hghtmap-io)
