find_package(Qt5 COMPONENTS Test REQUIRED)

# Populate a CMake variable with the sources
set(BSAToolTest_SRCS
        bsa/BsaFileTest.cpp
        bsa/BsaFileTest.h
        utils/CompressionTest.cpp
        utils/CompressionTest.h
        main/main.cpp)
# Tell CMake to create the executable
add_executable(BSAToolTest ${BSAToolTest_SRCS})
# Use the modules from Qt 5
target_link_libraries(BSAToolTest BSATool Qt5::Test)