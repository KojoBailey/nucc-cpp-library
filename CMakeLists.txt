cmake_minimum_required(VERSION 3.19)

project(nucc-cpp-library VERSION 0.1.0 DESCRIPTION "C++ NUCC lib")

# Find source files
file(GLOB SOURCES include/nucc/*.cpp)

# Create shared library
add_library(nucc-cpp-library STATIC ${SOURCES})

# Include header files
target_include_directories(nucc-cpp-library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(nucc-cpp-library PROPERTIES CXX_STANDARD 17)