#include "qdictopia.h"

QDictOpia::QDictOpia(QWidget* parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	setWindowTitle(tr("QDictopia"));
	
        QMenu *menu = menuBar()->addMenu("&File");
        mMainWindow = new MainWindow(this, 0, menu);
	setCentralWidget(mMainWindow);
}
