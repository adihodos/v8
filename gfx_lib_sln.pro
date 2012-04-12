TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = gfx_lib gfx_lib_tests

unix {
    QMAKE_CC = gcc-48
    QMAKE_CXX = g++-48
}
