file(GLOB SRC "./*.cpp")
add_executable(window ${SRC})
target_link_libraries(window PUBLIC base GTest::gtest GTest::gtest_main glfw3)
target_compile_definitions(window PRIVATE RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
