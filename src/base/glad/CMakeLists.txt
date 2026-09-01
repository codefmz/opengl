file(GLOB SRC "./*.c")
add_library(glad STATIC ${SRC})
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})