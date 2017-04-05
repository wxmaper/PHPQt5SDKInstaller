#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T15:32:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PHPQt5SDKInstaller
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += NOCRYPT USE_FILE32API
INCLUDEPATH += quazip quazip/quazip zlib/include

DEFINES += QUAZIP_BUILD
DEFINES += QUAZIP_STATIC
    # workaround for qdatetime.h macro bug
    DEFINES += NOMINMAX

CONFIG += embed_manifest_exe
#QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"

LIBS += -LD:/qt-projects/PHPQt5SDKInstaller/zlib/lib -lz
LIBS += -LD:/qt-projects/quazip/build-quazip-Desktop_Qt_5_7_0_STATIC-Release/release/libquazip.a
#LIBS += -L"D:/qt-projects/PHPQt5PluginInstaller/zlib/lib/libzdll.a"
#LIBS += -L"D:/qt-projects/PHPQt5PluginInstaller/zlib/lib/libz.a"


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    quazip/quazip/JlCompress.cpp \
    quazip/quazip/qioapi.cpp \
    quazip/quazip/quaadler32.cpp \
    quazip/quazip/quacrc32.cpp \
    quazip/quazip/quagzipfile.cpp \
    quazip/quazip/quaziodevice.cpp \
    quazip/quazip/quazip.cpp \
    quazip/quazip/quazipdir.cpp \
    quazip/quazip/quazipfile.cpp \
    quazip/quazip/quazipfileinfo.cpp \
    quazip/quazip/quazipnewinfo.cpp \
    quazip/quazip/unzip.c \
    quazip/quazip/zip.c

HEADERS  += mainwindow.h \
    data.h \
    quazip/quazip/crypt.h \
    quazip/quazip/ioapi.h \
    quazip/quazip/JlCompress.h \
    quazip/quazip/quaadler32.h \
    quazip/quazip/quachecksum32.h \
    quazip/quazip/quacrc32.h \
    quazip/quazip/quagzipfile.h \
    quazip/quazip/quaziodevice.h \
    quazip/quazip/quazip.h \
    quazip/quazip/quazip_global.h \
    quazip/quazip/quazipdir.h \
    quazip/quazip/quazipfile.h \
    quazip/quazip/quazipfileinfo.h \
    quazip/quazip/quazipnewinfo.h \
    quazip/quazip/unzip.h \
    quazip/quazip/zip.h \
    zlib/include/zconf.h \
    zlib/include/zlib.h

FORMS    += mainwindow.ui

RESOURCES += \
    assets.qrc

RC_FILE += data.rc

DISTFILES += \
    data.rc

#include(quazip/quazip/quazip.pro)
