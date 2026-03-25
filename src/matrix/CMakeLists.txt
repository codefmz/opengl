file(GLOB SRC "./*.cpp")

add_library(libmatrix STATIC ${SRC})
target_link_libraries(libmatrix PRIVATE libbase)
set_target_properties(libmatrix PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)