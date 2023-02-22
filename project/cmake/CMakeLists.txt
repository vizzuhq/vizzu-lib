cmake_minimum_required (VERSION 2.8.11)
project (Vizzu)

add_subdirectory (lib)
add_subdirectory (weblib)
add_subdirectory (test)

if(NOT EMSCRIPTEN)
add_subdirectory (qtest)
add_subdirectory (dtest)
endif()

include(tests.txt)
