cmake_minimum_required(VERSION 3.5.0)
project(glfw3 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
#specify whether the chosen C++ standard must be strictly adhered to by the compiler
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
add_compile_options(/utf-8)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

add_subdirectory(src)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(og main.cpp glad.c)
target_link_libraries(og glfw3 opengl32 gtest base window shader)