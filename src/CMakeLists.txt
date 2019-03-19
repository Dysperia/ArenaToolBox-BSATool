# Populate a CMake variable with the sources
file(GLOB_RECURSE BSATool_SRCS
        "assets/*.cpp"
        "bsa/*.cpp"
        "components/*.cpp"
        "configuration/*.cpp"
        "error/*.cpp"
        "log/*.cpp"
        "utils/*.cpp"
        "assets/*.h"
        "bsa/*.h"
        "components/*.h"
        "configuration/*.h"
        "designpatterns/*.h"
        "error/*.h"
        "log/*.h"
        "utils/*.h")
file(GLOB_RECURSE BSATool_MAIN "main/*.cpp")

# BSATool library
add_library(BSATool_library ${BSATool_SRCS})
target_link_libraries(BSATool_library Qt5::Widgets Qt5::Concurrent)

# BSATool executable
add_executable(BSATool ${BSATool_MAIN})
target_link_libraries(BSATool BSATool_library)