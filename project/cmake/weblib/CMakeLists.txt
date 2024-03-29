if(EMSCRIPTEN)
	include(../common.txt)
	include(../link.txt)

	set(appsDir ${root}/src/apps)

	file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${appsDir}/weblib/*.cpp)
	file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${appsDir}/weblib/*.h)

	add_executable(cvizzu ${sources})
	target_link_libraries(cvizzu LINK_PUBLIC vizzulib)

	add_dependencies(cvizzu vizzulib)

	include(../includes.txt)

	include(gen.txt)

	include(../emcc.txt)
	include(emcc.txt)

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "$ENV{EMSDK}/upstream/bin/wasm-opt" "-Oz"
		"$<TARGET_FILE_DIR:cvizzu>/cvizzu.wasm"
		"-o" "$<TARGET_FILE_DIR:cvizzu>/cvizzu.wasm")

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E make_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/dist"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy
		"cvizzu.*"
		"${CMAKE_CURRENT_SOURCE_DIR}/${root}/dist")

	add_custom_command(TARGET cvizzu POST_BUILD
		COMMAND gzip -9 < "$<TARGET_FILE_DIR:cvizzu>/cvizzu.wasm" > "$<TARGET_FILE_DIR:cvizzu>/cvizzu.wasm.gz"
		COMMAND stat -c "%s : %n" "$<TARGET_FILE_DIR:cvizzu>/cvizzu.*"
		COMMAND stat -c "%n %s" "$<TARGET_FILE_DIR:cvizzu>/cvizzu.*" > $<TARGET_FILE_DIR:cvizzu>/size.txt)
endif()