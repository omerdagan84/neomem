
// CPageObjectGeneral


#include "precompiled.h"

#include "Hint.h"
#include "NeoMem.h"
#include "PageObjectGeneral.h"
#include "StringEx.h"
#include "Constants.h"

#include "ConstantsDatabase.h"
#include "NeoDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CPageObjectGeneral property page

IMPLEMENT_DYNCREATE(CPageObjectGeneral, CPropertyPage)

CPageObjectGeneral::CPageObjectGeneral() : CPropertyPage(CPageObjectGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPageObjectGeneral)
	//}}AFX_DATA_INIT
//	m_bDirty = FALSE;
	m_pobj = 0;
	m_bNameChanged = FALSE;
	m_bDescriptionChanged = FALSE;
	m_bIDChanged = FALSE;
}

CPageObjectGeneral::~CPageObjectGeneral()
{
}

void CPageObjectGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageObjectGeneral)
	DDX_Control(pDX, IDC_LBL_OBJECTID, m_lblObjectID);
	DDX_Control(pDX, IDC_TXT_OBJECTID, m_txtObjectID);
	DDX_Control(pDX, IDC_TXT_FLAGS, m_txtFlags);
	DDX_Control(pDX, IDC_TXT_SIZE, m_txtSize);
	DDX_Control(pDX, IDC_TXT_NAME, m_txtName);
	DDX_Control(pDX, IDC_TXT_LOCATION, m_txtLocation);
	DDX_Control(pDX, IDC_TXT_DESCRIPTION, m_txtDescription);
	DDX_Control(pDX, IDC_TXT_CLASS, m_txtClass);
	DDX_Control(pDX, IDC_TXT_CHILDREN, m_txtChildren);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageObjectGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPageObjectGeneral)
	ON_EN_CHANGE(IDC_TXT_NAME, OnChangeTxtName)
	ON_EN_CHANGE(IDC_TXT_DESCRIPTION, OnChangeTxtDescription)
	ON_EN_CHANGE(IDC_TXT_OBJECTID, OnChangeTxtObjectID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageObjectGeneral message handlers




BOOL CPageObjectGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	ASSERT_VALID(m_pobj);

	m_txtName.SetWindowText(m_pobj->GetPropertyText(propName));
	m_txtClass.SetWindowText(m_pobj->GetPropertyText(propClassName));
	m_txtLocation.SetWindowText(m_pobj->GetPropertyText(propParentName));
	m_txtDescription.SetWindowText(m_pobj->GetPropertyText(propDescription));

	CStringEx str;

//	str.Format("%d", m_pobj->GetChildCount(FALSE));
	str.Format("%s", (LPCTSTR) fc(m_pobj->GetChildCount(FALSE)));
	m_txtChildren.SetWindowText(str);

	ULONG lngMem = m_pobj->GetMemoryUsed(FALSE);
//	str.Format("%d Bytes", m_pobj->GetMemoryUsed(FALSE));
	if (m_pobj->m_paChildren)
	{
		ULONG lngMemRecurse = m_pobj->GetMemoryUsed(TRUE);
//		str.Format("%d Bytes (%d with children)", lngMem, lngMemRecurse);
		str.Format("%s Bytes (%s with children)", (LPCTSTR) fc(lngMem), (LPCTSTR) fc(lngMemRecurse));
	}
	else
//		str.Format("%d Bytes", lngMem);
		str.Format("%s Bytes", (LPCTSTR) fc(lngMem));
//	str.FormatThousands("%t Bytes", m_pobj->GetMemoryUsed(FALSE));
	m_txtSize.SetWindowText(str);

	// Get flags
	BDataFlags datFlags;
	datFlags.m_lngFlags = m_pobj->m_lngFlags;
	m_txtFlags.SetWindowText(datFlags.GetBDataText(m_pobj->m_pDoc, propFlags));

	// Get icon?
//	m_pobj->GetIconID();

	// If in admin mode, show ObjectID, otherwise hide it
//	if (theApp.m_bAdmin)
//	{
//		str.Format("%d", m_pobj->m_lngObjectID);
//		m_txtObjectID.SetWindowText(str);
//	}
//	else
//	{
		m_txtObjectID.ShowWindow(SW_HIDE);
		m_lblObjectID.ShowWindow(SW_HIDE);
//	}

	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
	return TRUE;  
}




void CPageObjectGeneral::OnChangeTxtName() 
{
	m_bNameChanged = TRUE;
	SetModified(TRUE); // enables apply button
}

void CPageObjectGeneral::OnChangeTxtDescription() 
{
	m_bDescriptionChanged = TRUE;
	SetModified(TRUE); // enables apply button
}

void CPageObjectGeneral::OnChangeTxtObjectID() 
{
	m_bIDChanged = TRUE;
	SetModified(TRUE); // enables apply button
}



// Note: This gets called even if user didn't change anything and apply button is disabled
BOOL CPageObjectGeneral::OnApply() 
{
	xTRACE(" On Apply!\n");

	if (m_bNameChanged)
	{
		CString str;
		m_txtName.GetWindowText(str);
//		m_pobj->SetObjectText(str); //. this broadcasts hintPropertyChanged
		m_pobj->SetPropertyText(propName, str); // this updates all views
		m_bNameChanged = FALSE;
	}

	if (m_bDescriptionChanged)
	{
		CString str;
		m_txtDescription.GetWindowText(str);
		m_pobj->SetPropertyText(propDescription, str); // this updates all views
		m_bDescriptionChanged = FALSE;
	}

	if (m_bIDChanged)
	{
		CString str;
		m_txtObjectID.GetWindowText(str);
		ULONG lngObjectID = atol(str);
		m_pobj->m_lngObjectID = lngObjectID;

		CHint h;
		h.m_pobjObject = m_pobj;
		h.m_lngPropertyID = propObjectID;
		m_pobj->m_pDoc->UpdateAllViewsEx(NULL, hintPropertyChange, &h);

		m_bIDChanged = FALSE;
	}

	SetModified(FALSE);
	return CPropertyPage::OnApply();
}


