include(../common.txt)

set(testRoot ${root}/test/unit)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${testRoot}/*.cpp)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${testRoot}/*.h)

include(../includes.txt)
include(../link.txt)

if(EMSCRIPTEN)
	include(../emcc.txt)
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -s DISABLE_EXCEPTION_CATCHING=0")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
 -s NODERAWFS=1 \
 -s EXIT_RUNTIME=1 \
 -s ASSERTIONS=1 \
 -s EXPORTED_FUNCTIONS=['_main']")
endif()

add_executable (vizzutest ${sources})
target_link_libraries (vizzutest LINK_PUBLIC vizzulib)
add_dependencies(vizzutest vizzulib)
