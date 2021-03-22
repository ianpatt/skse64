cmake_minimum_required(VERSION 3.18)

# ---- Project ----

project(
	umbrella
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Add sub projects ----

if (NOT TARGET skse64)
	add_subdirectory(skse64)
endif()

if (NOT TARGET skse64_common)
	add_subdirectory(skse64_common)
endif()

if (NOT TARGET skse64_loader)
	add_subdirectory(skse64_loader)
endif()

if (NOT TARGET skse64_loader_common)
	add_subdirectory(skse64_loader_common)
endif()

if (NOT TARGET skse64_steam_loader)
	add_subdirectory(skse64_steam_loader)
endif()

if (NOT TARGET skse64_msstore_loader)
	add_subdirectory(skse64_msstore_loader)
endif()

if (NOT TARGET skse64_msstore_stub)
	add_subdirectory(skse64_msstore_stub)
endif()
