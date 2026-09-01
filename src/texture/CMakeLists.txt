file(GLOB SRC "./*.cpp")
add_executable(texture ${SRC})
target_link_libraries(texture PUBLIC base GTest::gtest GTest::gtest_main glfw3)
target_compile_definitions(texture PRIVATE RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}")