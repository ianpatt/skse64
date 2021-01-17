option(SKSE_COPY_OUTPUT "Copies output files to the skyrim64 directory" OFF)

if (SKSE_COPY_OUTPUT)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "$ENV{Skyrim64Path}"
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> "$ENV{Skyrim64Path}"
	)
endif()
