cmake_minimum_required(VERSION 3.5.0)
project(glfw3 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
#specify whether the chosen C++ standard must be strictly adhered to by the compiler
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/base
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src/base)
add_subdirectory(src/window)
add_subdirectory(src/shader)
add_subdirectory(src/texture)
add_subdirectory(src/matrix)

message("base_h = "${BASE_H})

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

enable_testing()

#target
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")
add_executable(${target} main.cpp glad.c)
target_link_libraries(${target} glfw3 opengl32 gtest lib${target})
target_include_directories(${target} PRIVATE ${JSONCPP_INCLUDES})
target_link_options(${target} PRIVATE "/WHOLEARCHIVE:lib${target}.lib")
add_custom_command(TARGET ${target}
    COMMAND echo "compile ${target} test..."
)

