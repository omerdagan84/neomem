
// CDialogImport



#include "precompiled.h"
#include "neomem.h"
#include "DialogImport.h"
#include "FileDialogEx.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





BEGIN_MESSAGE_MAP(CDialogImport, CDialog)
	//{{AFX_MSG_MAP(CDialogImport)
	ON_BN_CLICKED(IDC_BTN_SELECT_FILE, OnBtnSelectFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



CDialogImport::CDialogImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogImport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogImport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



BOOL CDialogImport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Add types to listbox
	m_lst.AddString("Text (.txt)");
	m_lst.AddString("Rich Text Format (.rtf)");
	m_lst.AddString("Tab Delimited Text");
	m_lst.AddString("Microsoft Access Database");
	m_lst.AddString("Microsoft Excel Spreadsheet");
	m_lst.AddString("Lotus 1-2-3 Spreadsheet");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDialogImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogImport)
	DDX_Control(pDX, IDC_BTN_SELECT_FILE, m_btnSelectFile);
	DDX_Control(pDX, IDC_TXT_FILE, m_txtFile);
	DDX_Control(pDX, IDC_LST, m_lst);
	//}}AFX_DATA_MAP
}





void CDialogImport::OnBtnSelectFile() 
{
	// Get filename

	m_txtFile.GetWindowText(m_strFilename);

	CFileDialogEx dlg(TRUE, _T("rtf"), _T(""), OFN_HIDEREADONLY, 
		theApp.m_strImportFilter, AfxGetMainWnd());
	dlg.m_ofn.lpstrTitle = _T("Import File");
	if (dlg.DoModal() == IDOK)
	{
		m_strFilename = dlg.GetPathName();
		m_txtFile.SetWindowText(m_strFilename);
//		CString strFilename = dlg.GetPathName();
//		CString strFileTitle = dlg.GetFileTitle();
	}
}



void CDialogImport::OnOK() 
{
	// Get filename
	m_txtFile.GetWindowText(m_strFilename);

	//. Important For better performance when accessing external databases, including ISAM 
	// databases and ODBC data sources, it is recommended that you attach external database 
	// tables to a Microsoft Jet engine database (.MDB) rather than connecting directly to the data source.

	try
	{
		// Open database
		//. name was marked as #pragma deprecated - move to OLE DB Consumer classes 
		CDaoDatabase db;
		// Note: this may throw an exception if database is locked
//		db.Open(strFile, FALSE, FALSE); // open db shared, NOT read-only
		db.Open(m_strFilename, FALSE, TRUE); // open db shared, read-only

		// Create tabledef object
		CDaoTableDef tbl(&db);

		// Walk through table definitions
		CDaoTableDefInfo tdi;
		CDaoFieldInfo fieldInfo;
		int nTables = db.GetTableDefCount();
		for (int i = 0; i < nTables; i++)
		{
			// Exclude system tables
			db.GetTableDefInfo(i, tdi);
			if (!(tdi.m_lAttributes & dbSystemObject))
			{
				// Get table information (fields)
				CString strTableName = tdi.m_strName;
				tbl.Open(strTableName);

				short nFields = tbl.GetFieldCount();
				for (short i = 0; i < nFields; i++)
				{
					tbl.GetFieldInfo(i, fieldInfo, AFX_DAO_SECONDARY_INFO);
//					fieldInfo.m_nType
					// dbBoolean
					// dbByte
					// dbInteger
					// dbLong
					// dbCurrency 
					// dbSingle 
					// dbDouble 
					// dbDate 
					// dbText (check size)
					// dbLongBinary 
					// dbMemo 
					// dbGUID
//					fieldInfo.m_lSize;
//					fieldInfo.m_nOrdinalPosition;
				}

				// Close table
				tbl.Close();
			}
		}

		// Close database
		db.Close();
	}
	catch (CDaoException* e)
	{
		// invalid database file
		// database unavailable
		// Do nothing. Used to catch security violations opening tables.
		e->Delete( );
	}


//, import now

	CDialog::OnOK();
}







//-----------------------------------------------------------------------------------------------------------------


/*
// is it even legal to use the office converters?
//			For what it is worth, the sample application offers to import the following formats on my PC:
//			· Word 6.0/95 for Windows & Macintosh
//			· Word 97
//			· Word 2.x for Windows
//			· Word 4.0 - 5.1 for Macintosh
//			· Recover Text from Any File
//			· Lotus 1-2-3
//			· WordPerfect 5.x
//			· WordPerfect 6.x
//			· Microsoft Excel Worksheet
//			· Works 3.0 for Windows
//			· Works 4.0 for Windows
//			· HTML Document
//			· Windows Write
void ImportBtnClick()
{
	OpenDialog1->InitialDir = ".";
	OpenDialog1->Filter = ImportRcl->FilterList;
	OpenDialog1->Title = "Import";
	if (!OpenDialog1->Execute()) return;
	
	CString str;
	str = "Import " + OpenDialog1->FileName + " from " +
		ImportRcl->Description->Strings[OpenDialog1->FilterIndex - 1] +
		" format?";
	if (AfxMessageBox(str, "Import", MB_OKCANCEL) != IDOK) 
		return;

	try 
	{
		TRtfConverter* rtfconv = new TRtfConverter(Handle, 
			ImportRcl->LibraryPath->Strings[OpenDialog1->FilterIndex - 1]);
		
		// verify format (could use IsFormatCorrect())
		CString strFormatClass;
		try
		{
			strFormatClass = rtfconv->GetFormatClass(OpenDialog1->FileName);
		}
		catch (ERtfConverter& e) 
		{
			delete rtfconv;
			CString str = e.Message + " (" + CString(e.Code) + ")";
			AfxMessageBox(str, "Import Error", MB_OK);
			return;
		}
		
		// convert
		rtfconv->ForeignToRtf(RichEdit1, OpenDialog1->FileName);

		// and free converter
		delete rtfconv;
	}
	catch (...) 
	{
		AfxMessageBox("Conversion failed", "Import Error", MB_OK);
	}
}


void ExportBtnClick()
{
	SaveDialog1->InitialDir = ".";
	SaveDialog1->Title = "Export";
	SaveDialog1->Filter = ExportRcl->FilterList;
	if (!SaveDialog1->Execute()) return;
	AnsiString msg;
	msg = AnsiString("Export ") + SaveDialog1->FileName + " to " +
		ExportRcl->Description->Strings[SaveDialog1->FilterIndex - 1] +
		" format?";
	if (Application->MessageBox(msg.c_str(), "Export", MB_OKCANCEL)
		!= IDOK) return;
	try {
		TRtfConverter* rtfconv = new TRtfConverter(Handle,
			ExportRcl->LibraryPath->Strings[SaveDialog1->FilterIndex - 1]);
		AnsiString formatClass =
			ExportRcl->FormatClass->Strings[SaveDialog1->FilterIndex - 1];

		// convert
		FCE fce = rtfconv->RtfToForeign(RichEdit1, SaveDialog1->FileName,
			formatClass);

		// and free converter
		delete rtfconv;

		// and tell user on failure
		if (fce != fceNoErr) {
			AnsiString msg;
			msg = "Conversion failed (" + AnsiString(fce) + ")";
			Application->MessageBox(msg.c_str(), "Export Error", MB_OK);
			}
		}
	catch (...) {
		Application->MessageBox("Conversion failed", "Export Error", MB_OK);
		}
}


*/

