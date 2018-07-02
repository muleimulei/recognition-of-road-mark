#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T08:46:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lesson_Desgn
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


SOURCES += \
        main.cpp \
        widget.cpp \
    mylabel.cpp

HEADERS += \
        widget.h \
    mylabel.h \
    tool.h

FORMS += \
        widget.ui

INCLUDEPATH += E:/opencv/opencv_qt/include \
               E:/opencv/opencv_qt/include/opencv \
               E:/opencv/opencv_qt/include/opencv2 \

LIBS += E:\\opencv\\opencv_qt\\lib\\libopencv_*

DISTFILES += \
    ../build-Lesson_Desgn-Desktop_Qt_5_10_1_MinGW_32bit-Debug/pictures/62.jpg
