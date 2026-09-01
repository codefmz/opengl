file(GLOB SRC "./*.cpp")
add_executable(shader ${SRC})
target_link_libraries(shader PUBLIC base GTest::gtest GTest::gtest_main glfw3)
target_compile_definitions(shader PRIVATE RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}")