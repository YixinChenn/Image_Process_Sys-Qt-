#-------------------------------------------------
#
# Project created by QtCreator 2018-10-13T16:55:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Digital_Image_Processing_System
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        psmainwindow.cpp \
    bmpimg.cpp \
    headerinfodialog.cpp \
    position.cpp \
    colordisplay.cpp \
    scalingdialog.cpp \
    msedialog.cpp

HEADERS += \
        psmainwindow.h \
    bmpfile.h \
    bmpimg.h \
    headerinfodialog.h \
    position.h \
    colordisplay.h \
    scalingdialog.h \
    msedialog.h

FORMS += \
        psmainwindow.ui \
    headerinfodialog.ui \
    position.ui \
    colordisplay.ui \
    scalingdialog.ui \
    msedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
