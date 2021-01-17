# Populate a CMake variable with the sources
set(BSATool_SRCS
        "assets/FileType.cpp"
        "assets/FileType.h"
        "assets/Cfa.cpp"
        "assets/Cfa.h"
        "assets/Dfa.cpp"
        "assets/Dfa.h"
        "assets/Img.cpp"
        "assets/Img.h"
        "assets/Palette.cpp"
        "assets/Palette.h"
        "bsa/BsaArchive.cpp"
        "bsa/BsaArchive.h"
        "bsa/BsaFile.cpp"
        "bsa/BsaFile.h"
        "components/console/ConsoleDock.cpp"
        "components/console/ConsoleDock.h"
        "components/fileList/FileListViewer.cpp"
        "components/fileList/FileListViewer.h"
        "components/fileList/FileListViewerItem.cpp"
        "components/fileList/FileListViewerItem.h"
        "components/fileDisplay/AnimationDisplayer.cpp"
        "components/fileDisplay/AnimationDisplayer.h"
        "components/fileDisplay/FileDisplayer.cpp"
        "components/fileDisplay/FileDisplayer.h"
        "components/fileDisplay/ImageDisplayer.cpp"
        "components/fileDisplay/ImageDisplayer.h"
        "components/fileDisplay/ImagesCollectionDisplayer.cpp"
        "components/fileDisplay/ImagesCollectionDisplayer.h"
        "components/fileDisplay/TextDisplayer.cpp"
        "components/fileDisplay/TextDisplayer.h"
        "components/bsaExplorer/MainWindow.cpp"
        "components/bsaExplorer/MainWindow.h"
        "components/menu/MenuBar.cpp"
        "components/menu/MenuBar.h"
        "components/toolbar/ToolBar.cpp"
        "components/toolbar/ToolBar.h"
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
        "utils/Compression.h"
        "utils/HuffmanTree.cpp"
        "utils/HuffmanTree.h"
        "utils/BitsStreams.cpp"
        "utils/BitsStreams.h"
        "utils/SlidingWindow.h")
set(BSATool_MAIN "main/main.cpp")

# BSATool library
add_library(BSATool_library ${BSATool_SRCS})
target_link_libraries(BSATool_library Qt5::Widgets Qt5::Concurrent)

# BSATool executable
add_executable(BSATool ${BSATool_MAIN})
target_link_libraries(BSATool BSATool_library)