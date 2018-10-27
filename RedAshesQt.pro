TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    coordinates.cpp \
    map.cpp \
    room.cpp

DISTFILES += \
    CMakeLists.txt

HEADERS += \
    coordinates.h \
    map.h \
    room.h
