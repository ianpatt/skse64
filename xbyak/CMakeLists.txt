cmake_minimum_required(VERSION 3.18)

# ---- Project ----

project(
	xbyak
	VERSION 4.87
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Create library ----

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Xbyak::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}_Targets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${PROJECT_NAME}ConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	"${PROJECT_SOURCE_DIR}/cmake/config.cmake.in"
	"${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
	INSTALL_DESTINATION
	${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(
	EXPORT ${PROJECT_NAME}_Targets
	FILE ${PROJECT_NAME}Targets.cmake
	NAMESPACE Xbyak::
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(
	FILES
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(
	FILES
		xbyak.h
		xbyak_bin2hex.h
		xbyak_mnemonic.h
		xbyak_util.h
	DESTINATION
		${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)
