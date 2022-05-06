include(../common.txt)

file(GLOB_RECURSE sources
	${root}/plugin/*.cpp
	${root}/src/base/*.cpp
	${root}/src/data/*.cpp
    ${root}/src/data2/*.cpp
	${root}/src/chart/*.cpp)

include(../version.txt)

file(GLOB_RECURSE headers
	${root}/plugin/*.h
	${root}/src/base/*.h
	${root}/src/data/*.h
	${root}/src/data/*.inc
	${root}/src/data2/*.h
	${root}/src/data2/*.inc
	${root}/src/chart/*.h
	${root}/src/chart/*.inc)

add_library(vizzulib ${sources})

include(../includes.txt)
include(../todochk.txt)

if(EMSCRIPTEN)
	include(../emcc.txt)
endif()
