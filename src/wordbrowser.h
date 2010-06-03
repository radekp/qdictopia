#ifndef WORD_BROWSER_H
#define WORD_BROWSER_H

#include <string>

#include <QWidget>
#include <QTextEdit>
#include <QGridLayout>

#include "lib/lib.h"

class WordBrowser : public QWidget
{
public:
	WordBrowser(QWidget* parent = 0, Qt::WindowFlags f = 0);
	bool lookup(QString& word, Libs* lib);

private:
	QString parseData(const char*, int, bool, bool);
	void xdxf2html(QString&);

private:
	QGridLayout* mLayout;
	QTextEdit* mTextEdit;
};

#endif
