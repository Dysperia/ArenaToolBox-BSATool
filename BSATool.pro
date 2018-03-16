#-------------------------------------------------
#
# Project created by QtCreator 2014-07-31T13:22:43
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BSATool
TEMPLATE = app


SOURCES += \
    src/askuserwindow.cpp \
    src/bsafile.cpp \
    src/globalImage.cpp \
    src/image.cpp \
    src/imageviewer.cpp \
    src/previewpalette.cpp \
    src/various.cpp \
    src/main/main.cpp \
    src/components/MenuBar.cpp \
    src/components/ToolBar.cpp \
    src/components/MainWindow.cpp \
    src/bsa/BsaArchive.cpp \
    src/bsa/BsaFile.cpp \
    src/assets/Palette.cpp \
    src/configuration/ApplicationConfiguration.cpp \
    src/configuration/ArchiveConfiguration.cpp \
    src/utils/Compression.cpp \
    src/log/Logger.cpp \
    src/components/ConsoleDock.cpp \
    src/assets/FileType.cpp \
    src/error/Status.cpp \
    src/assets/img.cpp

HEADERS  += \ 
    src/askuserwindow.hpp \
    src/bsafile.hpp \
    src/globalImage.hpp \
    src/image.hpp \
    src/imageviewer.hpp \
    src/previewpalette.hpp \
    src/various.hpp \
    src/components/MenuBar.h \
    src/components/ToolBar.h \
    src/components/MainWindow.h \
    src/bsa/BsaArchive.h \
    src/bsa/BsaFile.h \
    src/assets/Palette.h \
    src/configuration/ApplicationConfiguration.h \
    src/configuration/ArchiveConfiguration.h \
    src/utils/Compression.h \
    src/log/Logger.h \
    src/designpatterns/Singleton.h \
    src/components/ConsoleDock.h \
    src/assets/FileType.h \
    src/error/Status.h \
    src/assets/img.h

FORMS    +=
