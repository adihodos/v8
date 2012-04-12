TEMPLATE = app
CONFIG -= qt
CONFIG += debug_and_release thread
DESTDIR = ../build_output/tests
TARGET = gfx_lib_tests

win32 {
    CONFIG += console
}

unix {
    QMAKE_CC = gcc-48
    QMAKE_CXX = g++-48
    QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
    INCLUDEPATH += "../"
    LIBS += -lgtest
}

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -O0 -g3
    LIBS += "-L../build_output/lib" -lgfx_lib
} else {
    LIBS += "-L../build_output/lib" -lgfx_lib
}

SOURCES = main.cc \
    matrix4_tests.cc
