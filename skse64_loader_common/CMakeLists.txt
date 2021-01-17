cmake_minimum_required(VERSION 3.18)

# ---- Project ----

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/versioning.cmake)

project(
	skse64_loader_common
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

# ---- Dependencies ----

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

add_library(
	${PROJECT_NAME}
	STATIC
	${headers}
	${sources}
)

add_library(skse64::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

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
		xse::common
)

# ---- Create an installable target ----

include(GNUInstallDirs)

install(
	TARGETS
		${PROJECT_NAME}
	EXPORT
		${PROJECT_NAME}-targets
)

install(
	FILES
		${headers}
	DESTINATION
		${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

configure_file(
	cmake/config.cmake.in
	${PROJECT_NAME}-config.cmake
	@ONLY
)

install(
	FILES
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
	EXPORT
		${PROJECT_NAME}-targets
	NAMESPACE
		skse64::
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
