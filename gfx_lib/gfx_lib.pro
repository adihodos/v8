TEMPLATE = lib
CONFIG += staticlib debug_and_release sse2
CONFIG -= qt
PRECOMPILED_HEADER = pch_hdr.h
lib_name = gfx_lib

HEADERS += \
    vector4.h \
    vector3.h \
    vector2.h \
    rectangle.h \
    quaternion.h \
    plane.h \
    pch_hdr.h \
    matrix4X4.h \
    matrix3X3.h \
    matrix2X2.h \
    line.h \
    gfx_misc.h \
    ellipse.h \
    details.h \
    color.h \
    circle.h \
    camera.h \
    gfx_math.h

SOURCES += \
    camera.cc

unix {
    QMAKE_CC = gcc-48
    QMAKE_CXX = g++-48
    DESTDIR = ../build_output/lib
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
}

win32 {
    DESTDIR = ..\build_output\lib
}

CONFIG(debug, debug|release) {
    TARGET = $$join($$lib_name,,,_debug)
} else {
    TARGET = $$lib_name
}
