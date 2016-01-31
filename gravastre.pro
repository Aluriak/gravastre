QT       += core widgets

TARGET = gravastre
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11


SOURCES += src/main.cpp \
    src/astre.cpp \
    src/engine.cpp \
    src/view.cpp

HEADERS += \
    src/converters.h \
    src/astre.h \
    src/engine.h \
    src/view.h
