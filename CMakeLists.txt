#ghp_4xRfMdCFujawzmxseweqR2wHsWZySb2nJkcj

cmake_minimum_required(VERSION 3.1.0)

project(BSATool VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)

#set(CMAKE_AUTORCC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Concurrent REQUIRED)

# BSATool
add_subdirectory(src)

# Tests
add_subdirectory(test)
enable_testing()
add_test(NAME BSAToolTest COMMAND BSAToolTest)