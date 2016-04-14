QT       += core widgets

TARGET = gravastre
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11


SOURCES += src/main.cpp \
    src/astre.cpp \
    src/engine.cpp \
    src/system.cpp \
    src/orbit.cpp \
    src/view.cpp

HEADERS += \
    src/converters.h \
    src/astre.h \
    src/engine.h \
    src/system.h \
    src/orbit.h \
    src/view.h \
    src/utils.h \
    src/options.h
