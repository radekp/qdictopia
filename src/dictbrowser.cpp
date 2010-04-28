#include <QTextCodec>
#include "dictbrowser.h"

DictInfoWidget::DictInfoWidget(QWidget* parent, Qt::WindowFlags f, struct DictInfo info) : QWidget(parent, f), mInfo(info)
{
	QString count;
	QString uni_str;
	const char* utf8_cstr;
	QTextCodec* codec = QTextCodec::codecForName("UTF-8");

	mLayout = new QGridLayout(this);

	mLabelCount = new QLabel(tr("<qt><b>Word count:</qt></b>"), this);
	count.sprintf("%d", mInfo.wordcount);
	mLabelCountResult = new QLabel(count, this);

	mLabelAuthor = new QLabel(tr("<qt><b>Author:</qt></b>"), this);
	utf8_cstr = mInfo.author.data();
	uni_str = codec->toUnicode(utf8_cstr);
	mLabelAuthorResult = new QLabel(uni_str, this);

	mLabelEmail = new QLabel(tr("<qt><b>Email:</qt></b>"), this);
	mLabelEmailResult = new QLabel(mInfo.email.c_str(), this);

	mLabelWebsite = new QLabel(tr("<qt><b>Website:</qt></b>"), this);
	mLabelWebsiteResult = new QLabel(mInfo.website.c_str(), this);

	mLabelDescription = new QLabel(tr("<qt><b>Description:</qt></b>"), this);
	utf8_cstr = mInfo.description.data();
	uni_str = codec->toUnicode(utf8_cstr);
	mLabelDescriptionResult = new QLabel(uni_str, this);
	
	mLabelDate = new QLabel(tr("<qt><b>Date:</qt></b>"), this);
	mLabelDateResult = new QLabel(mInfo.date.c_str(), this);

	mLayout->addWidget(mLabelCount, 0, 0);
	mLayout->addWidget(mLabelCountResult, 0, 1);
	mLayout->addWidget(mLabelAuthor, 1, 0);
	mLayout->addWidget(mLabelAuthorResult, 1, 1);
	mLayout->addWidget(mLabelEmail, 2, 0);
	mLayout->addWidget(mLabelEmailResult, 2, 1);
	mLayout->addWidget(mLabelWebsite, 3, 0);
	mLayout->addWidget(mLabelWebsiteResult, 3, 1);
	mLayout->addWidget(mLabelDescription, 4, 0);
	mLayout->addWidget(mLabelDescriptionResult, 4, 1);
	mLayout->addWidget(mLabelDate, 5, 0);
	mLayout->addWidget(mLabelDateResult, 5, 1);
}

DictBrowser::DictBrowser(QWidget* parent, Qt::WindowFlags f, Libs* lib) : QWidget(parent, f), mLibs(lib)
{
	int i;

	mLayout = new QGridLayout(this);

	mListWidget = new QListWidget(this);
	for (i = 0; i < mLibs->ndicts(); i++) {
		const char* bkname = mLibs->dict_name(i).data();
		QTextCodec* codec = QTextCodec::codecForName("UTF-8");
		QString str = codec->toUnicode(bkname);
		mListWidget->addItem(str);
	}
	mListWidget->setCurrentRow(0);
	
	connect(mListWidget, SIGNAL(itemActivated(QListWidgetItem*)), 
			this, SLOT(slotItemActivated(QListWidgetItem*)));
	
	mLayout->addWidget(mListWidget, 0, 0);
}

void DictBrowser::slotItemActivated(QListWidgetItem* item)
{
	DictInfoWidget* info_widget;

	info_widget = new DictInfoWidget(this, Qt::Popup, 
			mLibs->dict_info(mListWidget->row(item)));
	info_widget->setAttribute(Qt::WA_DeleteOnClose);
	info_widget->showMaximized();
}
