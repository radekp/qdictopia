#include "qdictopia.h"

QDictOpia::QDictOpia(QWidget* parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	setWindowTitle(tr("QDictopia"));
	
	mMainWindow = new MainWindow(this);
	setCentralWidget(mMainWindow);
}
