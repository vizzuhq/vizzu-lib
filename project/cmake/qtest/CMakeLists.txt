include(../common.txt)

set(qTest ${root}/src/apps/qtest)
set(qUtils ${root}/src/apps/qutils)

file(GLOB_RECURSE sources
	${qTest}/*.cpp
	${qUtils}/*.cpp)

file(GLOB_RECURSE headers
	${qTest}/*.h
	${qUtils}/*.h)

include(../includes.txt)
include(../link.txt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(forms ${qTest}/window.ui)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable (qtest ${sources} ${forms})

include(../guicommon.txt)

target_link_libraries (qtest LINK_PUBLIC
	vizzulib
	Qt5::Widgets
	Qt5::Gui
	Qt5::Core)

add_dependencies(qtest vizzulib)
