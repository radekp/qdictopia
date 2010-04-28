#ifdef QTOPIA

#include <qtopiaapplication.h>
#include "qdictopia.h"

QTOPIA_ADD_APPLICATION(QTOPIA_TARGET, QDictOpia)
QTOPIA_MAIN

#else // QTOPIA

#include <QtGui/QApplication>
#include "qdictopia.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDictOpia w;
    w.show();
    return a.exec();
}

#endif // QTOPIA
