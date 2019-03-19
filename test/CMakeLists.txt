find_package(Qt5Test CONFIG REQUIRED)

# Populate a CMake variable with the sources
file(GLOB_RECURSE BSAToolTest_SRCS
        "*.cpp"
        "*.h")
# Tell CMake to create the executable
add_executable(BSAToolTest ${BSAToolTest_SRCS})
# Use the modules from Qt 5
target_link_libraries(BSAToolTest BSATool_library Qt5::Test)