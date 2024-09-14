cmake_minimum_required(VERSION 3.19)

project(nucc-cpp-library VERSION 0.1.0 DESCRIPTION "C++ NUCC lib")

# Find source files
file(GLOB SOURCES include/nucc/*.cpp)

# Include header files
include_directories(include/nucc)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})