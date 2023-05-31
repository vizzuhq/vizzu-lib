set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")

include(../common.txt)

set(dTest ${root}/src/apps/dtest)

file(GLOB_RECURSE sources
	${dTest}/*.cpp)

file(GLOB_RECURSE headers
	${dTest}/*.h)

include(../includes.txt)
include(../link.txt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable (dtest ${sources})

target_link_libraries (dtest LINK_PUBLIC
	vizzulib)

add_dependencies(dtest vizzulib)
