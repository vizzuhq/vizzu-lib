include(../common.txt)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS
	${root}/plugin/*.cpp
	${root}/src/base/*.cpp
	${root}/src/chart/*.cpp
	${root}/src/dataframe/*.cpp)

include(../version.txt)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
	${root}/plugin/*.h
	${root}/src/base/*.h
	${root}/src/chart/*.h
	${root}/src/dataframe/*.h)

add_library(vizzulib ${sources})

include(../includes.txt)
include(../todochk.txt)

if(EMSCRIPTEN)
	include(../emcc.txt)
endif()
