cmake_minimum_required(VERSION 2.8.11)
project(Vizzu)

add_subdirectory(lib)
add_subdirectory(test)

if(EMSCRIPTEN)
    add_subdirectory(weblib)
else()
    find_package(Qt5Core QUIET)

    if(Qt5Core_FOUND)
        add_subdirectory(qtest)
    endif()
endif()

include(tests.txt)
