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
	std::string parse_data(const gchar* data);
	std::string xdxf2text(const char* p);

private:
	QGridLayout* mLayout;
	QTextEdit* mTextEdit;
};

#endif
