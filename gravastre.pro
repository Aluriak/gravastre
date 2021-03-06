QT       += core widgets

TARGET = gravastre
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11


SOURCES += src/main.cpp \
    src/jsonConfig.cpp \
    src/drawableVelocity.cpp \
    src/simulationFactory.cpp \
    src/interactant.cpp \
    src/astre.cpp \
    src/player.cpp \
    src/engine.cpp \
    src/system.cpp \
    src/orbit.cpp \
    src/view.cpp

HEADERS += \
    src/simulationFactory.h \
    src/drawableVelocity.h \
    src/converters.h \
    src/interactant.h \
    src/astre.h \
    src/player.h \
    src/engine.h \
    src/system.h \
    src/orbit.h \
    src/view.h \
    src/jsonConfig.h \
    src/utils.h \
    src/options.h
