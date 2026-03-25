file(GLOB SRC "./*.cpp")

add_library(libtexture STATIC ${SRC})
target_link_libraries(libtexture PRIVATE libbase)
set_target_properties(libtexture PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)