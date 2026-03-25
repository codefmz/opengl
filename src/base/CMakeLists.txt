file(GLOB SRC "./*.cpp")

set(BASE_H ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "base header")

add_library(libbase STATIC ${SRC})
set_target_properties(libbase PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)