file(GLOB SRC "./*.cpp")

add_library(libshader STATIC ${SRC})
target_link_libraries(libshader PRIVATE libbase)
set_target_properties(libshader PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)