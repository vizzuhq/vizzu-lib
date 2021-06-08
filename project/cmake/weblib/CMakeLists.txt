include(../common.txt)
include(../link.txt)

set(appsDir ${root}/src/apps)

file(GLOB_RECURSE sources ${appsDir}/weblib/*.cpp)
file(GLOB_RECURSE headers ${appsDir}/weblib/*.h)

if(EMSCRIPTEN)
	add_executable (cvizzu ${sources})
	target_link_libraries (cvizzu LINK_PUBLIC vizzulib)
else()
	add_library(cvizzu ${sources})
endif()

add_dependencies(cvizzu vizzulib)

include(../includes.txt)

if(EMSCRIPTEN)
	include(../emcc.txt)
	include(emcc.txt)

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"cvizzu.*"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/example/lib")

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"${CMAKE_CURRENT_SOURCE_DIR}/${appsDir}/weblib/js-api/*"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/example/lib")

endif()
