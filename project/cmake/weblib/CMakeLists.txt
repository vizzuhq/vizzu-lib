include(../common.txt)
include(../link.txt)

set(appsDir ${root}/src/apps)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${appsDir}/weblib/*.cpp)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${appsDir}/weblib/*.h)

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
		COMMAND "$ENV{EMSDK}/upstream/bin/wasm-opt" "-Oz" 
		"$<TARGET_FILE_DIR:cvizzu>/cvizzu.wasm"
		"-o" "$<TARGET_FILE_DIR:cvizzu>/cvizzu.wasm")

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"cvizzu.*"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/dist")

	file(GLOB_RECURSE jsapi CONFIGURE_DEPENDS ${appsDir}/weblib/js-api/*)
	
	add_custom_target(copy_weblib
		COMMAND "${CMAKE_COMMAND}" -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/${appsDir}/weblib/js-api"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/dist"
		DEPENDS	${jsapi}
	)

	add_dependencies(cvizzu copy_weblib)

	add_custom_command(TARGET cvizzu POST_BUILD
			COMMAND gzip -9 < "$<TARGET_FILE_DIR:cvizzu>/cvizzu.wasm" > "$<TARGET_FILE_DIR:cvizzu>/cvizzu.wasm.gz"
			COMMAND stat -c "%s : %n" "$<TARGET_FILE_DIR:cvizzu>/cvizzu.*"
			COMMAND stat -c "%n %s" "$<TARGET_FILE_DIR:cvizzu>/cvizzu.*" > $<TARGET_FILE_DIR:cvizzu>/size.txt)


endif()
