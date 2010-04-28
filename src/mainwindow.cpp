#include <QMessageBox>
#include <QTimer>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f), mLineEdit(NULL)
{
	if (loadDicts() == false) {
		QTimer::singleShot(0, this, SLOT(slotWarning()));
	} else {
		// Build UI
		mLayout = new QGridLayout(this);

		mLineEdit = new QLineEdit(this);
		mLineEdit->setFocusPolicy(Qt::NoFocus);
		connect(mLineEdit, SIGNAL(textChanged(const QString&)), 
				this, SLOT(slotTextChanged(const QString&)));

		mMenu = QSoftMenuBar::menuFor(mLineEdit);
		mActionClear = mMenu->addAction(tr("Clear text"), this, SLOT(slotClearText()));
		mActionClear->setVisible(false);
		mActionDList = mMenu->addAction(tr("Dictionaries..."), this, SLOT(slotDictList()));

		mListWidget = new QListWidget(this);
		mListWidget->setFocusPolicy(Qt::NoFocus);
		connect(mListWidget, SIGNAL(itemActivated(QListWidgetItem*)), 
				this, SLOT(slotItemActivated(QListWidgetItem*)));

		mLayout->addWidget(mLineEdit, 0, 0);
		mLayout->addWidget(mListWidget, 1, 0);	
	}	
}

MainWindow::~MainWindow()
{
	if (mLibs)
		delete mLibs;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	switch(event->key())
	{
		case Qt::Key_Up:
			if (mListWidget->count() > 0)
			{
				if (mListWidget->currentRow() <= 0)
					mListWidget->setCurrentRow(mListWidget->count() - 1);
				else
					mListWidget->setCurrentRow(mListWidget->currentRow() - 1);
			}
			break;

		case Qt::Key_Down:
			
			if (mListWidget->count() > 0)
			{
				if (mListWidget->currentRow() < (mListWidget->count() - 1))
					mListWidget->setCurrentRow(mListWidget->currentRow() + 1);
				else
					mListWidget->setCurrentRow(0);
			}
			break;

		case Qt::Key_Select:
			QString word;

			if (mListWidget->currentRow() >= 0)
				word = mListWidget->item(mListWidget->currentRow())->text();
			else
				word = mLineEdit->text();

			mWordBrowser = new WordBrowser(this, Qt::Popup);
			mWordBrowser->setAttribute(Qt::WA_DeleteOnClose);
			mWordBrowser->showMaximized();
			mWordBrowser->lookup(word, mLibs);

			break;
	}
	
	QWidget::keyPressEvent(event);
}

bool MainWindow::slotTextChanged(const QString& text)
{
	glong* index = (glong*)malloc(sizeof(glong) * mLibs->ndicts());
	const gchar* word;
	bool bfound = false;

	if (text.isEmpty() && mActionClear->isVisible())
		mActionClear->setVisible(false);
	else if (!text.isEmpty() && !mActionClear->isVisible())
		mActionClear->setVisible(true);

	for (int i = 0; i < mLibs->ndicts(); i++)
		if (mLibs->LookupWord((const gchar*)text.toLatin1().data(), index[i], i))
			bfound = true;

	for (int i = 0; i < mLibs->ndicts(); i++)
		if (mLibs->LookupSimilarWord((const gchar*)text.toLatin1().data(), index[i], i))
			bfound = true;
	
	if (bfound)
	{
		mListWidget->clear();

		word = mLibs->poGetCurrentWord(index);
		mListWidget->addItem(tr((const char*)word));
	
		for (int j = 0; j < MAX_FUZZY; j++)
		{
			if ((word = mLibs->poGetNextWord(NULL, index)))
				mListWidget->addItem(tr((const char*)word));
			else
				break;
		}		
	}

	free(index);
	return true;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
	if (mLineEdit)
		mLineEdit->setEditFocus(true);

	QWidget::paintEvent(event);
}

void MainWindow::slotItemActivated(QListWidgetItem* item)
{
	QString word = item->text();

	mWordBrowser = new WordBrowser(this, Qt::Popup);
	mWordBrowser->setAttribute(Qt::WA_DeleteOnClose);
	mWordBrowser->showMaximized();
	mWordBrowser->lookup(word, mLibs);
}

void MainWindow::slotClearText()
{
	mLineEdit->clear();
}

void MainWindow::slotDictList()
{
	mDictBrowser= new DictBrowser(this, Qt::Popup, mLibs);
	mDictBrowser->setAttribute(Qt::WA_DeleteOnClose);
	mDictBrowser->showMaximized();
}

void MainWindow::slotWarning()
{
	QMessageBox::warning(this, tr("Dictionary"), tr("There are no dictionary loaded!"));
}

bool MainWindow::loadDicts()
{
	mLibs = new Libs();

	// Retrieve all dict infors
	mDictDir = QDir(DIC_PATH, "*.ifo");
	for (unsigned int i = 0; i < mDictDir.count(); i++)
		mLibs->load_dict(mDictDir.absoluteFilePath(mDictDir[i]).toLatin1().data());

	if (mLibs->ndicts() == 0)
		return false;
	else
		return true;
}
