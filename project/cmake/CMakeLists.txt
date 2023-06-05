cmake_minimum_required (VERSION 2.8.11)
project (Vizzu)

add_subdirectory (lib)
add_subdirectory (weblib)
add_subdirectory (test)

if(NOT EMSCRIPTEN)

find_package(Qt5 COMPONENTS Widgets Gui Core QUIET)
if( Qt5_FOUND )
    add_subdirectory (qtest)
endif()

endif()

include(tests.txt)
