#include <QTextCodec>
#include <QStack>
#include "wordbrowser.h"

WordBrowser::WordBrowser(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
	mLayout = new QGridLayout(this);

	mTextEdit = new QTextEdit(this);
	mTextEdit->setFocusPolicy(Qt::NoFocus);
	mTextEdit->setReadOnly(true);
	mLayout->addWidget(mTextEdit, 0, 0);
}

bool WordBrowser::lookup(QString& word, Libs* lib)
{
	glong index;
	QString translation;

	for (int i = 0; i < lib->ndicts(); i++)
	{
		if (lib->LookupWord((gchar*)(word.toUtf8().data()), index, i))
		{
		    QString result;
		    translation.append("<span style='color:#99FF33;font-weight:bold'>");
		    result = QString::fromUtf8(lib->dict_name(i).data());
		    translation.append(result);
		    translation.append("</span><br>");
		    translation.append("<span style='color:#FFFF66;'>");
		    result = QString::fromUtf8((char*)lib->poGetWord(index, i));
		    translation.append(result);
		    translation.append("</span><br>");
		    result = parseData(lib->poGetWordData(index, i), i, true, false);
		    translation.append(result);
		    translation.append(tr("<hr>"));
		}
	}

	if (translation.isEmpty())
		translation = QString("Not found!");

	mTextEdit->setHtml(translation);

	return true;
}

void WordBrowser::xdxf2html(QString &str)
{
    str.replace("<abr>", "<font class=\"abbreviature\">");
    str.replace("<tr>", "<font class=\"transcription\">[");
    str.replace("</tr>", "]</font>");
    str.replace("<ex>", "<font class=\"example\">");
    str.replace(QRegExp("<k>.*<\\/k>"), "");
    str.replace(QRegExp("(<\\/abr>)|(<\\ex>)"), "</font");
}

// taken from qstardict
QString WordBrowser::parseData(const char *data, int dictIndex, bool htmlSpaces, bool reformatLists)
{
    QString result;
    quint32 dataSize = *reinterpret_cast<const quint32*>(data);
    const char *dataEnd = data + dataSize;
    const char *ptr = data + sizeof(quint32);
    while (ptr < dataEnd)
    {
        switch (*ptr++)
        {
            case 'm':
            case 'l':
            case 'g':
            {
                QString str = QString::fromUtf8(ptr);
                ptr += str.toUtf8().length() + 1;
                result += str;
                break;
            }
            case 'x':
            {
                QString str = QString::fromUtf8(ptr);
                ptr += str.toUtf8().length() + 1;
                xdxf2html(str);
                result += str;
                break;
            }
            case 't':
            {
                QString str = QString::fromUtf8(ptr);
                ptr += str.toUtf8().length() + 1;
                result += "<font class=\"example\">";
                result += str;
                result += "</font>";
                break;
            }
            case 'y':
            {
                ptr += strlen(ptr) + 1;
                break;
            }
            case 'W':
            case 'P':
            {
                ptr += *reinterpret_cast<const quint32*>(ptr) + sizeof(quint32);
                break;
            }
            default:
                ; // nothing
        }
    }


    if (reformatLists)
    {
        int pos = 0;
        QStack<QChar> openedLists;
        while (pos < result.length())
        {
            if (result[pos].isDigit())
            {
                int n = 0;
                while (result[pos + n].isDigit())
                    ++n;
                pos += n;
                if (result[pos] == '&' && result.mid(pos + 1, 3) == "gt;")
                    result.replace(pos, 4, ">");
                QChar marker = result[pos];
                QString replacement;
                if (marker == '>' || marker == '.' || marker == ')')
                {
                    if (n == 1 && result[pos - 1] == '1') // open new list
                    {
                        if (openedLists.contains(marker))
                        {
                            replacement = "</li></ol>";
                            while (openedLists.size() && openedLists.top() != marker)
                            {
                                replacement += "</li></ol>";
                                openedLists.pop();
                            }
                        }
                        openedLists.push(marker);
                        replacement += "<ol>";
                    }
                    else
                    {
                        while (openedLists.size() && openedLists.top() != marker)
                        {
                            replacement += "</li></ol>";
                            openedLists.pop();
                        }
                        replacement += "</li>";
                    }
                    replacement += "<li>";
                    pos -= n;
                    n += pos;
                    while (result[pos - 1].isSpace())
                        --pos;
                    while (result[n + 1].isSpace())
                        ++n;
                    result.replace(pos, n - pos + 1, replacement);
                    pos += replacement.length();
                }
                else
                    ++pos;
            }
            else
                ++pos;
        }
        while (openedLists.size())
        {
            result += "</li></ol>";
            openedLists.pop();
        }
    }
    if (htmlSpaces)
    {
        int n = 0;
        while (result[n].isSpace())
            ++n;
        result.remove(0, n);
        n = 0;
        while (result[result.length() - 1 - n].isSpace())
            ++n;
        result.remove(result.length() - n, n);

        for (int pos = 0; pos < result.length();)
        {
            switch (result[pos].toAscii())
            {
                case '[':
                    result.insert(pos, "<font class=\"transcription\">");
                    pos += 28 + 1; // sizeof "<font class=\"transcription\">" + 1
                    break;
                case ']':
                    result.insert(pos + 1, "</font>");
                    pos += 7 + 1; // sizeof "</font>" + 1
                    break;
                case '\t':
                    result.insert(pos, "&nbsp;&nbsp;&nbsp;&nbsp;");
                    pos += 24 + 1; // sizeof "&nbsp;&nbsp;&nbsp;&nbsp;" + 1
                    break;
                case '\n':
                {
                    int count = 1;
                    n = 1;
                    while (result[pos + n].isSpace())
                    {
                        if (result[pos + n] == '\n')
                            ++count;
                        ++n;
                    }
                    if (count > 1)
                        result.replace(pos, n, "</p><p>");
                    else
                        result.replace(pos, n, "<br>");
                    break;
                }
                default:
                    ++pos;
            }
        }
    }
    return result;
}


