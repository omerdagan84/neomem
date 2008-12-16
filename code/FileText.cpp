

#include "precompiled.h"

#include "FileText.h"

#include "BObject.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CFileText::CFileText()
{
	SetFormat(ffCsv); // default
}

CFileText::~CFileText()
{
}



void CFileText::SetFormat(eFileFormat nFormat)
{
	m_nFormat = nFormat;

	m_strDelim = ",";
	m_strNewline = "\r\n";

	switch (m_nFormat)
	{
		case ffTab:
			m_strDelim = "\t";
			break;
		case ffUnknown:
		case ffCsv:
		case ffText:
		case ffRtf:
		case ffXml:
		case ffHtml:
		case ffOpml:
			break;
	}

}



// write the given string, surrounding with quotes (if necessary)
// doubling up quotes, and converting \r\n to \0a (which is how excel handles 
// multiline strings)
void CFileText::WriteValue(LPCTSTR psz)
{
	CString s = CString(psz);
	s.Replace("\"","\"\""); // double up quotes
	s.Replace("\r\n","\n"); // \r\n to \0a
	// only need to surround with quotes if string has a space or cr in it...
	// or comma...
	BOOL bNeedQuotes = ((s.Find(" ")>=0) || (s.Find("\n")>=0) || (s.Find(",")>=0));
	if (bNeedQuotes) this->WriteString("\"");
	this->WriteString(s);
	if (bNeedQuotes) this->WriteString("\"");
}


void CFileText::WriteDelim()
{
	this->WriteString(m_strDelim);
}

void CFileText::WriteNewline()
{
	this->WriteString(m_strNewline);
}

// use this in place of writestring, if you want a trailing newline. 
void CFileText::WriteLine(LPCTSTR psz)
{
	this->WriteString(psz);
	this->WriteNewline();
}




void CFileText::WriteHeader(BDataLink& datProps)
{
	switch (m_nFormat)
	{
		case ffTab:
		case ffCsv:
			// walk through properties, export as header
			{
				int nProps = datProps.GetLinkCount();

				for (int i = 0; i < nProps; i++)
				{
					BObject* pobjProp = datProps.GetLinkAt(i);
					LPCTSTR psz = pobjProp->GetName(FALSE);
					this->WriteValue(psz);
					// handle _value props
					if (pobjProp->PropertyDefHasMachineVersion())
					{
//						CString s = CString(psz) + _T("_value");
						CString s = pobjProp->GetPropertyDefMachineVersionName();
						this->WriteDelim();
						this->WriteValue(s);
					}
					if (i < nProps-1)
					{
						this->WriteDelim();
					}
				}
				this->WriteNewline();
			}
			break;
		case ffOpml:
			{
				CString str = "<opml version=""1.0"">\r\n"
					"  <head>\r\n"
/*					"    <title>playlist.xml</title>\r\n"
					"    <dateCreated>Thu, 27 Jul 2000 03:24:18 GMT</dateCreated>\r\n"
					"    <dateModified>Fri, 15 Sep 2000 09:01:23 GMT</dateModified>\r\n"
					"    <ownerName>Dave Winer</ownerName>\r\n"
					"    <ownerEmail>dave@userland.com</ownerEmail>\r\n"
					"    <expansionState>1,3,17</expansionState>\r\n"
					"    <vertScrollState>1</vertScrollState>\r\n"
					"    <windowTop>164</windowTop>\r\n"
					"    <windowLeft>50</windowLeft>\r\n"
					"    <windowBottom>672</windowBottom>\r\n"
					"    <windowRight>455</windowRight>\r\n"
*/					"  </head>\r\n"
					"  <body>\r\n";
				this->WriteString(str);
			}
			break;
		case ffRtf:
			{
				CString str = 
					"{\\rtf1\r\n"
					"{\\fonttbl {\\f0 Times New Roman;}}\r\n"
					"\r\n"
					"{\\stylesheet\r\n"
					"{\\ql \\widctlpar\\adjustright \\f0\\fs20 Normal;}\r\n"
					"{\\s1\\ql \\keepn\\outlinelevel0 \\b\\f0\\fs32\\sbasedon0 heading 1;}\r\n"
					"{\\s2\\ql \\keepn\\outlinelevel1 \\b\\f0\\fs28\\sbasedon0 heading 2;}\r\n"
					"{\\s3\\ql \\keepn\\outlinelevel2 \\b\\f0\\fs26\\sbasedon0 heading 3;}\r\n"
					"{\\s4\\ql \\keepn\\outlinelevel3 \\b\\f0\\fs24\\sbasedon0 heading 4;}\r\n"
					"{\\s5\\ql \\keepn\\outlinelevel4 \\b\\f0\\fs22\\sbasedon0 heading 5;}\r\n"
					"{\\s6\\ql \\keepn\\outlinelevel5 \\b\\f0\\fs20\\sbasedon0 heading 6;}\r\n"
					"{\\s7\\ql \\keepn\\outlinelevel6 \\b\\f0\\fs18\\sbasedon0 heading 7;}\r\n"
					"{\\s8\\ql \\keepn\\outlinelevel7 \\b\\f0\\fs16\\sbasedon0 heading 8;}\r\n"
					"}\r\n"
					"\r\n"
					"\\viewkind3\r\n";
				this->WriteString(str);
			}
			break;

		case ffXml:
			{
				CString str = "<?xml version=\"1.0\" ?>\r\n" //encoding=\"UTF-8\" ?>\r\n"
//					"<!DOCTYPE NeoMem:Root>\r\n"
//					"<NeoMem:Root>\r\n";
					"<root>\r\n";
				this->WriteString(str);
			}
			break;
		case ffText:
			break;
		case ffHtml:
			break;
	}

}


void CFileText::WriteFooter()
{
	switch (m_nFormat)
	{
		case ffTab:
		case ffCsv:
		case ffText:
			break;
		case ffOpml:
			{
				CString str = "  </body>\r\n"
					"</opml>\r\n";
				this->WriteString(str);
			}
			break;
		case ffRtf:
			{
				this->WriteString("}\r\n");
			}
			break;
		case ffXml:
			{
//				CString str = "</NeoMem:Root>\r\n";
				CString str = "</root>\r\n";
				this->WriteString(str);
			}
			break;
		case ffHtml:
			break;
	}

}


