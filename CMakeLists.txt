cmake_minimum_required(VERSION 3.1.0)

project(BSATool)

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

# Populate a CMake variable with the sources
file(GLOB BSATool_SRCS
        "src/assets/*.cpp"
        "src/bsa/*.cpp"
        "src/components/*.cpp"
        "src/configuration/*.cpp"
        "src/error/*.cpp"
        "src/log/*.cpp"
        "src/utils/*.cpp"
        "src/main/*.cpp"
        "src/assets/*.h"
        "src/bsa/*.h"
        "src/components/*.h"
        "src/configuration/*.h"
        "src/designpatterns/*.h"
        "src/error/*.h"
        "src/log/*.h"
        "src/utils/*.h")
# Tell CMake to create the executable
add_executable(BSATool ${BSATool_SRCS})
# Use the modules from Qt 5
target_link_libraries(BSATool Qt5::Widgets Qt5::Concurrent)