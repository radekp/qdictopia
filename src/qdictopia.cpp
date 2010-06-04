#include "qdictopia.h"

QDictOpia::QDictOpia(QWidget* parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	setWindowTitle(tr("QDictopia"));
	
#ifdef QTOPIA
        QMenu *menu = QSoftMenuBar::menuFor(this);
#else
        QMenu *menu = menuBar()->addMenu("&File");
#endif
        mMainWindow = new MainWindow(this, 0, menu);
	setCentralWidget(mMainWindow);
}
