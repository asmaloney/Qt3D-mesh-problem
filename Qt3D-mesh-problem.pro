TEMPLATE = app

INCLUDEPATH += $$PWD/common
SOURCES += $$PWD/common/window.cpp
HEADERS += $$PWD/common/window.h

QT += widgets

CONFIG += qt
CONFIG += c++11

QT += 3dcore 3drender 3dinput

HEADERS += \
    cylindergeometry.h \
    cylindermesh.h

SOURCES += \
    main.cpp \
    cylindergeometry.cpp \
    cylindermesh.cpp
