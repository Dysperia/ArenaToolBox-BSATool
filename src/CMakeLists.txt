set(BSATool_SRCS
        components/console/ConsoleDock.cpp
        components/console/ConsoleDock.h
        components/fileList/FileListViewer.cpp
        components/fileList/FileListViewer.h
        components/fileList/FileListViewerItem.cpp
        components/fileList/FileListViewerItem.h
        components/fileDisplay/AnimationDisplayer.cpp
        components/fileDisplay/AnimationDisplayer.h
        components/fileDisplay/FileDisplayer.cpp
        components/fileDisplay/FileDisplayer.h
        components/fileDisplay/ImageDisplayer.cpp
        components/fileDisplay/ImageDisplayer.h
        components/fileDisplay/ImagesCollectionDisplayer.cpp
        components/fileDisplay/ImagesCollectionDisplayer.h
        components/fileDisplay/TextDisplayer.cpp
        components/fileDisplay/TextDisplayer.h
        components/bsaExplorer/MainWindow.cpp
        components/bsaExplorer/MainWindow.h
        components/menu/MenuBar.cpp
        components/menu/MenuBar.h
        components/toolbar/ToolBar.cpp
        components/toolbar/ToolBar.h
        log/Logger.cpp
        log/Logger.h
        )

set(BSAExplorer_MAIN main/main.cpp)

# ARENATOOLBOX_LIB is an env variable set to the base path of the ArenaToolBox Library (direct subdirs are include/ and lib/)

add_executable(BSAExplorer ${BSAExplorer_MAIN} ${BSATool_SRCS})
target_include_directories(BSAExplorer PUBLIC $ENV{ARENATOOLBOX_LIB}/include)
find_library(ArenaToolBoxLibrary NAMES ArenaToolBox PATHS ENV ARENATOOLBOX_LIB PATH_SUFFIXES /lib REQUIRED)
target_link_libraries(BSAExplorer ${ArenaToolBoxLibrary} Qt5::Widgets Qt5::Concurrent)