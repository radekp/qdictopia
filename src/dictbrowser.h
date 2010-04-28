#ifndef DICT_BROWSER_H
#define DICT_BROWSER_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QLabel>

#include "lib/lib.h"

class DictInfoWidget : public QWidget
{
public:
	DictInfoWidget(QWidget* parent, Qt::WindowFlags f, struct DictInfo info);

private:
	QGridLayout* mLayout;
	struct DictInfo mInfo;
	QLabel* mLabelCount;
	QLabel* mLabelCountResult;
	QLabel* mLabelAuthor;
	QLabel* mLabelAuthorResult;
	QLabel* mLabelEmail;
	QLabel* mLabelEmailResult;
	QLabel* mLabelWebsite;
	QLabel* mLabelWebsiteResult;
	QLabel* mLabelDescription;
	QLabel* mLabelDescriptionResult;
	QLabel* mLabelDate;
	QLabel* mLabelDateResult;
};

class DictBrowser : public QWidget 
{
	Q_OBJECT

public:
	DictBrowser(QWidget* parent = 0, Qt::WindowFlags f = 0, Libs* lib = 0);

private slots:
	void slotItemActivated(QListWidgetItem* item);

private:
	QGridLayout* mLayout;
	QListWidget* mListWidget;

	Libs* mLibs;
};

#endif
