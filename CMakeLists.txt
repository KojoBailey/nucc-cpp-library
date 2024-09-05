cmake_minimum_required(VERSION 3.16)
project(NUCC_Lib)

set(CMAKE_CXX_STANDARD 11)

target_include_directories(${PROJECT_NAME} PRIVATE "src" "external/binary/binary")