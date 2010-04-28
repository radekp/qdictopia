TEMPLATE=app
TARGET=qdictopia

CONFIG+=qtopia
DEFINES+=QTOPIA

# I18n info
STRING_LANGUAGE=en_US
LANGUAGES=en_US

INCLUDEPATH+=/usr/include/glib-2.0 \
             /usr/lib/glib-2.0/include

LIBS+=-lglib-2.0

# Package info
pkg [
    name=qdictopia
    desc="A Dictionary Program for StarDict"
    license=GPLv2
    version=1.0
    maintainer="Radek Polak <psonek2@seznam.cz>"
]

# Input files
HEADERS=\
    src/mainwindow.h \
    src/wordbrowser.h \
    src/qdictopia.h \
    src/dictbrowser.h \
    src/lib/dictziplib.hpp \
    src/lib/distance.h \
    src/lib/file.hpp \
    src/lib/lib.h \
    src/lib/mapfile.hpp

SOURCES=\
    src/main.cpp \
    src/mainwindow.cpp \
    src/wordbrowser.cpp \
    src/qdictopia.cpp \
    src/dictbrowser.cpp \
    src/lib/dictziplib.cpp \
    src/lib/distance.cpp \
    src/lib/lib.cpp

# Install rules
target [
    hint=sxe
    domain=untrusted
]

desktop [
    hint=desktop
    files=qdictopia.desktop
    path=/apps/Applications
]

pics [
    hint=pics
    files=pics/*
    path=/pics/qdictopia
]

help [
    hint=help
    source=help
    files=*.html
]

HEADERS += src/mainwindow.h \
           src/wordbrowser.h \
           src/qdictopia.h \
           src/dictbrowser.h \
           src/lib/dictziplib.hpp \
           src/lib/distance.h \
           src/lib/file.hpp \
           src/lib/lib.h \
           src/lib/mapfile.hpp
SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/wordbrowser.cpp \
           src/qdictopia.cpp \
           src/dictbrowser.cpp \
           src/lib/dictziplib.cpp \
           src/lib/distance.cpp \
           src/lib/lib.cpp