#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QDir>
#include <QKeyEvent>
#include <QSoftMenuBar>
#include <QMenu>
#include <QAction>

#include "lib/lib.h"
#include "wordbrowser.h"
#include "dictbrowser.h"

#define MAX_FUZZY 30
#define DIC_PATH "/usr/share/stardict/dic/"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0, Qt::WindowFlags f = 0);
	~MainWindow();

protected:
	void keyPressEvent(QKeyEvent* event);
	void paintEvent(QPaintEvent* event);

private:
	bool loadDicts();

private slots:
	bool slotTextChanged(const QString& text);
	void slotItemActivated(QListWidgetItem* item);
	void slotClearText();
	void slotDictList();
	void slotWarning();

private:
	QGridLayout* mLayout;
	QLineEdit* mLineEdit;
	QListWidget* mListWidget;
	WordBrowser* mWordBrowser;
	DictBrowser* mDictBrowser;
	QMenu* mMenu;
	QAction* mActionClear;
	QAction* mActionDList;

	Libs* mLibs;
	QDir mDictDir;
};

#endif
