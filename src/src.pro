TARGET = recognition

QT -= gui core

CONFIG += console
CONFIG += -std=c++1z
QMAKE_CXXFLAGS += -std=c++17
QMAKE_LFLAGS += -static

CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += *.cpp
HEADERS += *.h

LIBS += -lstdc++fs
