cmake_minimum_required(VERSION 3.1.0)

project(BSATool)

set(CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Concurrent CONFIG REQUIRED)

# BSATool
add_subdirectory(src)

# Tests
add_subdirectory(test)
enable_testing()
add_test(NAME BSAToolTest COMMAND BSAToolTest)