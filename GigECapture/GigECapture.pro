QT += core gui
QT += widgets
QT += multimedia
QT += multimediawidgets

CONFIG += c++11
CONFIG += console
CONFIG += qt

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/lib64
LIBS += -L"$$_PRO_FILE_PWD_/lib64/" -lopencv_core2413 -lopencv_highgui2413 -lopencv_imgproc2413
LIBS += -L"$$_PRO_FILE_PWD_/lib64/" -lFlyCapture2


HEADERS += \
    mainwindow.h \
    controlpanel.h \
    data.h \
    videostreamwindow.h \
    helper.h

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    controlpanel.cpp \
    videostreamwindow.cpp \
    helper.cpp


# Default rules for deployment.
TARGET = GigECapture
TEMPLATE = app
