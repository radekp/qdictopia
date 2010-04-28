#include <QTextCodec>
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
		if (lib->LookupWord((gchar*)(word.toLatin1().data()), index, i))
		{
			QString result = QString((char*)lib->poGetWord(index, i));
			
			if (result == word)
			{	
				const char* utf8_cstr;
				QString uni_str;
				QTextCodec* codec = QTextCodec::codecForName("UTF-8");

				translation.append("<---");
				utf8_cstr = lib->dict_name(i).data();
				uni_str = codec->toUnicode(utf8_cstr);
				translation.append(uni_str);
				translation.append("--->");

				utf8_cstr = (parse_data(lib->poGetWordData(index, i))).data();
				uni_str = codec->toUnicode(utf8_cstr);
				translation.append(uni_str);
				translation.append(tr("\n\n"));
			}
		}
	}

	if (translation.isEmpty())
		translation = QString("Not found!");

	mTextEdit->setText(translation);

	return true;
}

std::string WordBrowser::xdxf2text(const char* p)
{
	std::string res;
	for (; *p; ++p) {
		if (*p!='<') {
			if (g_str_has_prefix(p, "&gt;")) {
				res+=">";
				p+=3;
			} else if (g_str_has_prefix(p, "&lt;")) {
				res+="<";
				p+=3;
			} else if (g_str_has_prefix(p, "&amp;")) {
				res+="&";
				p+=4;
			} else if (g_str_has_prefix(p, "&quot;")) {
				res+="\"";
				p+=5;
			} else
				res+=*p;
			continue;
		}

		const char *next=strchr(p, '>');
		if (!next)
			continue;

		std::string name(p+1, next-p-1);

		if (name=="abr")
			res+="";
		else if (name=="/abr")
			res+="";
		else if (name=="k") {
			const char *begin=next;
			if ((next=strstr(begin, "</k>"))!=NULL)
				next+=sizeof("</k>")-1-1;
			else
				next=begin;
		} else if (name=="b")
			res+="";
		else if (name=="/b")
			res+="";
		else if (name=="i")
			res+="";
		else if (name=="/i")
			res+="";
		else if (name=="tr")
			res+="[";
		else if (name=="/tr")
			res+="]";
		else if (name=="ex")
			res+="";
		else if (name=="/ex")
			res+="";
		else if (!name.empty() && name[0]=='c' && name!="co") {
			std::string::size_type pos=name.find("code");
			if (pos!=std::string::size_type(-1)) {
				pos+=sizeof("code=\"")-1;
				std::string::size_type end_pos=name.find("\"");
				std::string color(name, pos, end_pos-pos);
				res+="";
			} else {
				res+="";
			}
		} else if (name=="/c")
			res+="";

		p=next;
	}
	return res;
}

std::string WordBrowser::parse_data(const gchar* data)
{
	if (!data)
		return "";

	std::string res;
	guint32 data_size, sec_size=0;
	gchar *m_str;
	const gchar *p=data;
	data_size=*((guint32 *)p);
	p+=sizeof(guint32);
	while (guint32(p - data)<data_size) {
		switch (*p++) {
		case 'm':
		case 'l': //need more work...
		case 'g':
			sec_size = strlen(p);
			if (sec_size) {
				res+="\n";
				m_str = g_strndup(p, sec_size);
				res += m_str;
				g_free(m_str);
			}
			sec_size++;
			break;
		case 'x':
			sec_size = strlen(p);
			if (sec_size) {
				res+="\n";
				m_str = g_strndup(p, sec_size);
				res += xdxf2text(m_str);
				g_free(m_str);
			}
			sec_size++;
			break;
		case 't':
			sec_size = strlen(p);
			if(sec_size){
				res+="\n";
				m_str = g_strndup(p, sec_size);
				res += "["+std::string(m_str)+"]";
				g_free(m_str);
			}
			sec_size++;
			break;
		case 'y':
			sec_size = strlen(p);
			sec_size++;				
			break;
		case 'W':
		case 'P':
			sec_size=*((guint32 *)p);
			sec_size+=sizeof(guint32);
			break;
		}
		p += sec_size;
	}

  
	return res;
}
