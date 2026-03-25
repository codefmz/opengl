file(GLOB SRC "./*.cpp")

add_library(libwindow STATIC ${SRC})
target_link_libraries(libwindow PRIVATE libbase)
set_target_properties(libwindow PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)