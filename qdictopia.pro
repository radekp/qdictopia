qtopia_project(qtopia app)
TARGET=qdictopia
CONFIG+=qtopia_main no_quicklaunch
INCLUDEPATH += /usr/include/glib-2.0 \
			   /usr/lib/glib-2.0/include
LIBS += -lglib-2.0

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

help.source=help
help.files=qdictopia*
help.hint=help
INSTALLS+=help

desktop.files=qdictopia.desktop
desktop.path=/apps/Applications
desktop.hint=desktop
INSTALLS+=desktop

pics.files=pics/*
pics.path=/pics/qdictopia
pics.hint=pics
INSTALLS+=pics

pkg.desc=A Dictionary Program from QStarDict
pkg.version=Alpha-x.x.x
pkg.maintainer=Luxi Liu(luxi.liu@gmail.com)
pkg.license=GPL
pkg.domain=trusted
