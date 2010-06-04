#include <QMessageBox>
#include <QTimer>
#ifdef QTOPIA
#include <QtopiaApplication>
#endif

#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags f, QMenu *menu) : QWidget(parent, f), mLineEdit(NULL)
{
#ifdef QTOPIA
        Q_UNUSED(menu);
#endif

	if (loadDicts() == false) {
		QTimer::singleShot(0, this, SLOT(slotWarning()));
	} else {
		// Build UI
		mLayout = new QGridLayout(this);

		mLineEdit = new QLineEdit(this);
                //mLineEdit->setFocusPolicy(Qt::NoFocus);
		connect(mLineEdit, SIGNAL(textChanged(const QString&)), 
				this, SLOT(slotTextChanged(const QString&)));

#ifdef QTOPIA
                mMenu = QSoftMenuBar::menuFor(mLineEdit);
#else
                mMenu = menu;
#endif
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

    int MaxFuzzy=MAX_FUZZY;
    const gchar* word;

    if (text.isEmpty() && mActionClear->isVisible())
       mActionClear->setVisible(false);
    else if (!text.isEmpty() && !mActionClear->isVisible())
		mActionClear->setVisible(true);

    gchar *fuzzy_res[MaxFuzzy];
    if (! mLibs->LookupWithFuzzy((const gchar*)text.toUtf8().data(), fuzzy_res, MaxFuzzy))
        return true;
    mListWidget->clear();
    for (gchar **p = fuzzy_res, **end = fuzzy_res + MaxFuzzy; p != end && *p; ++p)
    {
        mListWidget->addItem(QString::fromUtf8((const char*)*p));
        g_free(*p);
    }
    return true;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
#ifdef QTOPIA
	if (mLineEdit)
		mLineEdit->setEditFocus(true);
#endif

	QWidget::paintEvent(event);
}

void MainWindow::slotItemActivated(QListWidgetItem* item)
{
	QString word = item->text();

        mWordBrowser = new WordBrowser(this, Qt::Popup);
        mWordBrowser->setAttribute(Qt::WA_DeleteOnClose);
        mWordBrowser->lookup(word, mLibs);
        mWordBrowser->showMaximized();
        mWordBrowser->exec();
#ifdef QTOPIA
        QtopiaApplication::showInputMethod();
#endif
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
		mLibs->load_dict(mDictDir.absoluteFilePath(mDictDir[i]).toUtf8().data());

	if (mLibs->ndicts() == 0)
		return false;
	else
		return true;
}
