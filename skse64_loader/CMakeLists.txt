cmake_minimum_required(VERSION 3.18)

# ---- Project ----

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/versioning.cmake)

project(
	skse64_loader
	VERSION ${SKSE_VERSION_MAJOR}.${SKSE_VERSION_MINOR}.${SKSE_VERSION_PATCH}
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Build options ----

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

# ---- Dependencies ----

if (NOT TARGET skse64_common)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../skse64_common skse64_common)	# bundled
endif()

if (NOT TARGET skse64_loader_common)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../skse64_loader_common skse64_loader_common)	# bundled
endif()

find_package(common REQUIRED CONFIG)

# ---- Add source files ----

include(cmake/headerlist.cmake)
include(cmake/sourcelist.cmake)

source_group(
	${PROJECT_NAME}
	FILES
		${headers}
		${sources}
)

# ---- Create library ----

add_executable(
	${PROJECT_NAME}
	${headers}
	${sources}
	${skse64_common_SOURCE_DIR}/skse_version.rc
)

add_executable(skse64::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/configuration.cmake)

target_compile_features(
	${PROJECT_NAME}
	PUBLIC
		cxx_std_11
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
		skse64::skse64_common
		skse64::skse64_loader_common
		Shlwapi.lib
		Version.lib
		xse::common
)

# ---- Configure all targets ----

set_target_properties(
	${PROJECT_NAME}
	skse64_common
	skse64_loader_common
	PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/installation.cmake)
