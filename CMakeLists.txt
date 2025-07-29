cmake_minimum_required(VERSION 3.19)
project(nucc VERSION 0.1.0 DESCRIPTION "C++ NUCC lib")

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(nucc STATIC ${SOURCES})
target_include_directories(nucc
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(nucc PROPERTIES CXX_STANDARD 23)

add_subdirectory(extern/binary)
target_link_libraries(${PROJECT_NAME} PUBLIC kojo::binary)

add_subdirectory(extern/logger)
target_link_libraries(${PROJECT_NAME} PUBLIC kojo::logger)

add_library(kojo::nucc ALIAS nucc)

install(TARGETS nucc
    EXPORT nuccTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT nuccTargets
    FILE nuccTargets.cmake
    NAMESPACE kojo::
    DESTINATION lib/cmake/nucc
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/nuccConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/nuccConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/nuccConfig.cmake"
  INSTALL_DESTINATION lib/cmake/nucc
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/nuccConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/nuccConfigVersion.cmake"
    DESTINATION lib/cmake/nucc
)

message(STATUS "NUCC++ Library build configured. To install: `cmake --install . --prefix <install_path>`")