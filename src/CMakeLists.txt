# Populate a CMake variable with the sources
set(BSATool_SRCS
        "assets/FileType.cpp"
        "assets/FileType.h"
        "assets/Img.cpp"
        "assets/Img.h"
        "assets/Palette.cpp"
        "assets/Palette.h"
        "bsa/BsaArchive.cpp"
        "bsa/BsaArchive.h"
        "bsa/BsaFile.cpp"
        "bsa/BsaFile.h"
        "components/ConsoleDock.cpp"
        "components/ConsoleDock.h"
        "components/FileDisplayer.cpp"
        "components/FileDisplayer.h"
        "components/FileListViewer.cpp"
        "components/FileListViewer.h"
        "components/FileListViewerItem.cpp"
        "components/FileListViewerItem.h"
        "components/ImageDisplayer.cpp"
        "components/ImageDisplayer.h"
        "components/MainWindow.cpp"
        "components/MainWindow.h"
        "components/MenuBar.cpp"
        "components/MenuBar.h"
        "components/ToolBar.cpp"
        "components/ToolBar.h"
        "configuration/ApplicationConfiguration.cpp"
        "configuration/ApplicationConfiguration.h"
        "configuration/ArchiveConfiguration.cpp"
        "configuration/ArchiveConfiguration.h"
        "designpatterns/Singleton.h"
        "error/Status.cpp"
        "error/Status.h"
        "log/Logger.cpp"
        "log/Logger.h"
        "main/main.cpp"
        "utils/Compression.cpp"
        "utils/Compression.h")
set(BSATool_MAIN "main/main.cpp")

# BSATool library
add_library(BSATool_library ${BSATool_SRCS})
target_link_libraries(BSATool_library Qt5::Widgets Qt5::Concurrent)

# BSATool executable
add_executable(BSATool ${BSATool_MAIN})
target_link_libraries(BSATool BSATool_library)