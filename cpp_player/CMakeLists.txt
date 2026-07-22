cmake_minimum_required(VERSION 3.20)
project(Showdown_Player LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE LIB_FILES "${SOURCE_DIR}/*.cpp")

add_executable(Showdown_Player src/main.cpp ${LIB_FILES})
target_include_directories(Showdown_Player PUBLIC "${SOURCE_DIR}")
target_compile_options(Showdown_Player PRIVATE -Wall -O3 -Wno-reorder-init-list)
