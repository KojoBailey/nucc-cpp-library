cmake_minimum_required(VERSION 3.19)

project(nucc-cpp-library VERSION 0.1.0 DESCRIPTION "C++ NUCC lib")

# Find source files
file(GLOB SOURCES include/nucc/*.cpp include/nucc/chunks/binary/asbr/*.cpp)

# Create shared library
add_library(nucc-cpp-library STATIC ${SOURCES})

# Include header files
target_include_directories(nucc-cpp-library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(nucc-cpp-library PROPERTIES CXX_STANDARD 20)

add_subdirectory(include/nucc/external/json)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json)
target_include_directories(${PROJECT_NAME} PUBLIC include/nucc/external/json/include)