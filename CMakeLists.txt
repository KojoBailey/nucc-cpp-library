# metadata & version info
cmake_minimum_required(VERSION 3.19)
project(xfbin VERSION 0.1.0 DESCRIPTION "Library for interacting with CyberConnect2 XFBIN files.")
set(CMAKE_CXX_STANDARD 23) # -std=c++23

# .cpp source files
set(SOURCES
  src/xfbin.cpp
)
add_library(xfbin STATIC ${SOURCES})

# .hpp header files
target_include_directories(xfbin
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

# submodule: KojoBailey/binary-cpp-library
add_subdirectory(extern/binary)
target_link_libraries(${PROJECT_NAME} PUBLIC kojo::binary)

# namespaced alias for lib users
add_library(kojo::xfbin ALIAS xfbin)

# lib system installation
install(TARGETS xfbin
	EXPORT xfbinTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT xfbinTargets
	FILE xfbinTargets.cmake
	NAMESPACE kojo::
	DESTINATION lib/cmake/xfbin
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/xfbinConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
	"${CMAKE_CURRENT_LIST_DIR}/cmake/xfbinConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/xfbinConfig.cmake"
	INSTALL_DESTINATION lib/cmake/xfbin
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/xfbinConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/xfbinConfigVersion.cmake"
	DESTINATION lib/cmake/xfbin
)
message(STATUS "XFBIN++ build configured. To install: `cmake --install . --prefix <install_path>`")

# sandbox for manual testing
option(BUILD_SANDBOX "Build the sandbox executable." ON)
if (BUILD_SANDBOX)
	add_executable(xfbin_lib_test test/main.cpp)
	target_link_libraries(xfbin_lib_test PRIVATE xfbin)
endif()