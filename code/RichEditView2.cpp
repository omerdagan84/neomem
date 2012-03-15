
// CRichEditView2


#include "precompiled.h"

#include "richeditview2.h"
#include "richeditstuff.h"
#include "FrameChild.h"
#include "NeoMem.h"
#include "PreviewViewEx.h"
#include "RichEditCtrlEx.h"
#include "PrintInfoMore.h"

#include "NeoDoc.h"
#include "ConstantsDatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//AFX_STATIC const UINT _afxMsgFindReplace2 = ::RegisterWindowMessage(FINDMSGSTRING);
// _T("commdlg_FindReplace");
extern const UINT _afxMsgFindReplace2;



// Macros
//--------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CRichEditView2, CCtrlView)

BEGIN_MESSAGE_MAP(CRichEditView2, CCtrlView)
	//{{AFX_MSG_MAP(CRichEditView2)
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_COMMAND(ID_FORMAT_FONT, OnFormatFont)
	ON_COMMAND(ID_EDIT_OPEN_HYPERLINK, OnEditOpenHyperlink)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(EN_SELCHANGE, OnSelChange)
	ON_REGISTERED_MESSAGE(_afxMsgFindReplace2, OnFindReplaceCmd)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()

// richedit buffer limit -- let's set it at 16M
// this is used with LimitText
AFX_DATADEF ULONG CRichEditView2::lMaxSize = 0xffffff;

// ole
//	ON_COMMAND(ID_EDIT_PASTE_SPECIAL, OnEditPasteSpecial)
//	ON_COMMAND(ID_OLE_EDIT_PROPERTIES, OnEditProperties)
//	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
//	ON_COMMAND(ID_EDIT_INSERT_OBJECT, OnEditInsertObject)

/*
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE_PLAIN, OnEditPastePlain)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
//	ON_UPDATE_COMMAND_UI(ID_EDIT_SPIKE, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_PLAIN, OnUpdateNeedClip)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateNeedFind) //?
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_PROPERTIES, OnUpdateEditProperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_SPECIAL, OnUpdateEditPasteSpecial)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_FORMAT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVE_TO, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SORT_SELECTED_TEXT, OnUpdateNeedSel)
*/


// Construction/Destruction
//--------------------------------------------------------------------------------------------------

//`CRichEditView2::CRichEditView2() : CCtrlView(_T("RICHEDIT"), AFX_WS_DEFAULT_VIEW |
//CRichEditView2::CRichEditView2() : CCtrlView(_T("RichEdit20A"), AFX_WS_DEFAULT_VIEW |
CRichEditView2::CRichEditView2() : CCtrlView(RICHEDIT_CLASS, AFX_WS_DEFAULT_VIEW |
	WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
	ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL | ES_SELECTIONBAR)
{
//	_afxRichEditState = new _AFX_RICHEDIT_STATE; //` added this
//	_afxRichEditState->pFindReplaceDlg = 0; //` added this
	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
	m_lpRichEditOle = NULL;
//	m_nBulletIndent = 720; // twips (1/2 inch)
	m_nBulletIndent = 360; // twips (1/4 inch)
	m_nWordWrap = WrapToWindow;
	m_nPasteType = 0;
	SetPaperSize(CSize(8*1440+720, 11*1440)); // twips //, change for english A4 etc?
	SetMargins(CRect(0,0,0,0));
//	m_nHeaderHeightTwips = 720; // twips
//	m_nFooterHeightTwips = 720; // twips
	m_nHeaderHeightTwips = 0;
	m_nFooterHeightTwips = 0;

//	::ZeroMemory(&m_charformat, sizeof(CHARFORMAT));//`
	::ZeroMemory(&m_charformat2, sizeof(CHARFORMAT2));//`
	::ZeroMemory(&m_paraformat, sizeof(PARAFORMAT));//`

//	m_charformat.cbSize = sizeof(CHARFORMAT);
	m_charformat2.cbSize = sizeof(CHARFORMAT2); //`
	m_paraformat.cbSize = sizeof(PARAFORMAT);
//	m_fScale = 1.0f; // delete
//	m_pwndOldParent = 0; //delete
}


CRichEditView2::~CRichEditView2()
{
	xTRACE("CRichEditView2::Destructor\n");
	//` added this
//	if (_afxRichEditState)
//		delete _afxRichEditState;
}


BOOL CRichEditView2::PreCreateWindow(CREATESTRUCT& cs)
{
//`	if (!AfxInitRichEdit())
//	if (!AfxInitRichEditEx())
	if (!AfxInitRichEdit2())
		return FALSE;

	CCtrlView::PreCreateWindow(cs);

	cs.lpszName = _T("");

	cs.cx = cs.cy = 100; // necessary to avoid bug with ES_SELECTIONBAR and zero for cx and cy
	cs.style |= WS_CLIPSIBLINGS;

	return TRUE;
}



int CRichEditView2::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CCtrlView::OnCreate(lpcs) != 0)
		return -1;

	GetRichEditCtrlEx().LimitText(lMaxSize);
//`	GetRichEditCtrlEx().SetEventMask(ENM_SELCHANGE | ENM_CHANGE | ENM_SCROLL);
	//` added notification for trying to modify protected text and click on link text
	GetRichEditCtrlEx().SetEventMask(ENM_SELCHANGE | ENM_CHANGE | ENM_SCROLL | 
						EN_PROTECTED | ENM_LINK);
	VERIFY(GetRichEditCtrlEx().SetOLECallback(&m_xRichEditOleCallback));
	m_lpRichEditOle = GetRichEditCtrlEx().GetIRichEditOle();
	DragAcceptFiles();
	GetRichEditCtrlEx().SetOptions(ECOOP_OR, ECO_AUTOWORDSELECTION);
	WrapChanged();
	ASSERT(m_lpRichEditOle != NULL);
	return 0;
}

void CRichEditView2::OnInitialUpdate()
{
	CCtrlView::OnInitialUpdate();
	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
}



// Document like functions
//--------------------------------------------------------------------------------------------------

void CRichEditView2::OnDestroy()
{
	xTRACE("CRichEditView2::OnDestroy\n");
	if (m_lpRichEditOle != NULL)
		m_lpRichEditOle->Release();
	CCtrlView::OnDestroy();
}


// Clear all contents
void CRichEditView2::DeleteContents()
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	SetWindowText(_T(""));
	GetRichEditCtrlEx().EmptyUndoBuffer();
	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
	ASSERT_VALID(this);
}


// This should be called when printing characteristics have changed (SetMargins or SetPaperSize).
void CRichEditView2::WrapChanged()
{
	CWaitCursor wait;
//`	CRichEditCtrl& ctrl = GetRichEditCtrl();
	CRichEditCtrlEx& ctrl = GetRichEditCtrlEx();
	if (m_nWordWrap == WrapNone)
		ctrl.SetTargetDevice(NULL, 1);
	else if (m_nWordWrap == WrapToWindow)
		ctrl.SetTargetDevice(NULL, 0);
	else if (m_nWordWrap == WrapToTargetDevice) // wrap to ruler
	{
		AfxGetApp()->CreatePrinterDC(m_dcTarget);
		if (m_dcTarget.m_hDC == NULL)
			m_dcTarget.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		ctrl.SetTargetDevice(m_dcTarget, GetPrintWidth());
	}
}



// Serialization support
//--------------------------------------------------------------------------------------------------

//` changed name from _afxRichEditCookie
class RichEditCookie
{
public:
	CArchive& m_ar;
	DWORD m_dwError;
	RichEditCookie(CArchive& ar) : m_ar(ar) {m_dwError=0;}
};



// Read and write CRichEditView2 object to archive, with length prefix.
void CRichEditView2::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	Stream(ar, FALSE);
	ASSERT_VALID(this);
}


void CRichEditView2::Stream(CArchive& ar, BOOL bSelection)
{
	EDITSTREAM es = {0, 0, EditStreamCallBack};
//`	_afxRichEditCookie cookie(ar);
	RichEditCookie cookie(ar);
	es.dwCookie = (DWORD)&cookie;
// ole 	int nFormat = GetDocument()->GetStreamFormat(); 
	int nFormat = SF_RTFNOOBJS; // ole - replaces ole objects with spaces (just in case they snuck in there)
	if (bSelection)
		nFormat |= SFF_SELECTION;
	if (ar.IsStoring())
		GetRichEditCtrlEx().StreamOut(nFormat, es);
	else
	{
		GetRichEditCtrlEx().StreamIn(nFormat, es);
		Invalidate();
	}
	if (cookie.m_dwError != 0)
		AfxThrowFileException(cookie.m_dwError);
}


// return 0 for no error, otherwise return error code
DWORD CALLBACK CRichEditView2::EditStreamCallBack(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
//`	_afxRichEditCookie* pCookie = (_afxRichEditCookie*)dwCookie;
	RichEditCookie* pCookie = (RichEditCookie*)dwCookie;
	CArchive& ar = pCookie->m_ar;
	ar.Flush();
	DWORD dw = 0;
	*pcb = cb;
	TRY
	{
		if (ar.IsStoring())
			ar.GetFile()->Write(pbBuff, cb);
		else
			*pcb = ar.GetFile()->Read(pbBuff, cb);
	}
	CATCH(CFileException, e)
	{
		*pcb = 0;
		pCookie->m_dwError = (DWORD)e->m_cause;
		dw = 1;
		DELETE_EXCEPTION(e); 
	}
	AND_CATCH_ALL(e)
	{
		*pcb = 0;
		pCookie->m_dwError = (DWORD)CFileException::genericException;
		dw = 1;
		DELETE_EXCEPTION(e);
	}
	END_CATCH_ALL
	return dw;
}






// XRichEditOleCallback
// This is the COM interface for the control!
//--------------------------------------------------------------------------------------------------

BEGIN_INTERFACE_MAP(CRichEditView2, CCtrlView)
	// we use IID_IUnknown because richedit doesn't define an IID
	INTERFACE_PART(CRichEditView2, IID_IUnknown, RichEditOleCallback)
END_INTERFACE_MAP()

STDMETHODIMP_(ULONG) CRichEditView2::XRichEditOleCallback::AddRef()
{
	METHOD_PROLOGUE_EX_(CRichEditView2, RichEditOleCallback)
	return (ULONG)pThis->InternalAddRef();
}

STDMETHODIMP_(ULONG) CRichEditView2::XRichEditOleCallback::Release()
{
	METHOD_PROLOGUE_EX_(CRichEditView2, RichEditOleCallback)
	return (ULONG)pThis->InternalRelease();
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CRichEditView2, RichEditOleCallback)
	return (HRESULT)pThis->InternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::GetNewStorage(LPSTORAGE* ppstg)
{
//. ole
/*
	METHOD_PROLOGUE_EX_(CRichEditView2, RichEditOleCallback)

	// Create a flat storage and steal it from the client item.
	// The client item is only used for creating the storage.
	COleClientItem item;
	item.GetItemStorageFlat();
	*ppstg = item.m_lpStorage;
	HRESULT hRes = E_OUTOFMEMORY;
	if (item.m_lpStorage != NULL)
	{
		item.m_lpStorage = NULL;
		hRes = S_OK;
	}
	pThis->GetDocument()->InvalidateObjectCache();
	return hRes;
*/
	return S_FALSE;
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::GetInPlaceContext(
	LPOLEINPLACEFRAME* lplpFrame, LPOLEINPLACEUIWINDOW* lplpDoc,
	LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	METHOD_PROLOGUE_EX(CRichEditView2, RichEditOleCallback)
	return pThis->GetWindowContext(lplpFrame, lplpDoc, lpFrameInfo);
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::ShowContainerUI(BOOL fShow)
{
	METHOD_PROLOGUE_EX(CRichEditView2, RichEditOleCallback)
	return pThis->ShowContainerUI(fShow);
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::QueryInsertObject(
	LPCLSID /*lpclsid*/, LPSTORAGE /*pstg*/, LONG /*cp*/)
{
//. ole
/*
	METHOD_PROLOGUE_EX(CRichEditView2, RichEditOleCallback)
	pThis->GetDocument()->InvalidateObjectCache();
	return S_OK;
*/
	return S_FALSE;
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::DeleteObject(LPOLEOBJECT /*lpoleobj*/)
{
//.ole
/*
	METHOD_PROLOGUE_EX_(CRichEditView2, RichEditOleCallback)
	pThis->GetDocument()->InvalidateObjectCache();
	return S_OK;
*/
	return S_FALSE;
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::QueryAcceptData(
	LPDATAOBJECT lpdataobj, CLIPFORMAT* lpcfFormat, DWORD reco,
	BOOL fReally, HGLOBAL hMetaPict)
{
	METHOD_PROLOGUE_EX(CRichEditView2, RichEditOleCallback)
	return pThis->QueryAcceptData(lpdataobj, lpcfFormat, reco, fReally, hMetaPict);
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::ContextSensitiveHelp(BOOL /*fEnterMode*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CRichEditView2::XRichEditOleCallback::GetClipboardData(
	CHARRANGE* lpchrg, DWORD reco, LPDATAOBJECT* lplpdataobj)
{
	METHOD_PROLOGUE_EX(CRichEditView2, RichEditOleCallback)
	LPDATAOBJECT lpOrigDataObject = NULL;

	// get richedit's data object
	if (FAILED(pThis->m_lpRichEditOle->GetClipboardData(lpchrg, reco,
		&lpOrigDataObject)))
	{
		return E_NOTIMPL;
	}

	// allow changes
	HRESULT hRes = pThis->GetClipboardData(lpchrg, reco, lpOrigDataObject,
		lplpdataobj);

	// if changed then free original object
	if (SUCCEEDED(hRes))
	{
		if (lpOrigDataObject!=NULL)
			lpOrigDataObject->Release();
		return hRes;
	}
	else
	{
		// use richedit's data object
		*lplpdataobj = lpOrigDataObject;
		return S_OK;
	}
}


STDMETHODIMP CRichEditView2::XRichEditOleCallback::GetDragDropEffect(
	BOOL fDrag, DWORD grfKeyState, LPDWORD pdwEffect)
{
	if (!fDrag) // allowable dest effects
	{
		DWORD dwEffect;
		// check for force link
		if ((grfKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
			dwEffect = DROPEFFECT_LINK;
		// check for force copy
		else if ((grfKeyState & MK_CONTROL) == MK_CONTROL)
			dwEffect = DROPEFFECT_COPY;
		// check for force move
		else if ((grfKeyState & MK_ALT) == MK_ALT)
			dwEffect = DROPEFFECT_MOVE;
		// default -- recommended action is move
		else
			dwEffect = DROPEFFECT_MOVE;
		if (dwEffect & *pdwEffect) // make sure allowed type
			*pdwEffect = dwEffect;
	}
	return S_OK;
}


STDMETHODIMP CRichEditView2::XRichEditOleCallback::GetContextMenu(
	WORD seltype, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg,
	HMENU* lphmenu)
{
	METHOD_PROLOGUE_EX(CRichEditView2, RichEditOleCallback)
	HMENU hMenu = pThis->GetContextMenu(seltype, lpoleobj, lpchrg);
	if (hMenu == NULL)
		return E_NOTIMPL;
	*lphmenu = hMenu;
	return S_OK;
}




// Command helpers
//--------------------------------------------------------------------------------------------------

/*void CRichEditView2::OnCharEffect(DWORD dwMask, DWORD dwEffect)
{
	//` using charformat2 structure
//	GetCharFormatSelection();
//	if (m_charformat.dwMask & dwMask) // selection is all the same
//		m_charformat.dwEffects ^= dwEffect;
//	else
//		m_charformat.dwEffects |= dwEffect;
//	m_charformat.dwMask = dwMask;
//	SetCharFormat(m_charformat);
	GetCharFormatSelection2();
	if (m_charformat2.dwMask & dwMask) // selection is all the same
		m_charformat2.dwEffects ^= dwEffect;
	else
		m_charformat2.dwEffects |= dwEffect;
	m_charformat2.dwMask = dwMask;
	SetCharFormat2(m_charformat2);
}
*/


// called by cviewrtf
void CRichEditView2::OnUpdateCharEffect(CCmdUI* pCmdUI, DWORD dwMask, DWORD dwEffect)
{
//	GetCharFormatSelection();
//	pCmdUI->SetCheck((m_charformat.dwMask & dwMask) ?
//		((m_charformat.dwEffects & dwEffect) ? 1 : 0) : 2);
	GetCharFormatSelection2();
	pCmdUI->SetCheck((m_charformat2.dwMask & dwMask) ?
		((m_charformat2.dwEffects & dwEffect) ? 1 : 0) : 2);
}


// called by cviewrtf
void CRichEditView2::OnParaAlign(WORD wAlign)
{
	GetParaFormatSelection();
	m_paraformat.dwMask = PFM_ALIGNMENT;
	m_paraformat.wAlignment = wAlign;
	SetParaFormat(m_paraformat);
}


// called by cviewrtf
void CRichEditView2::OnUpdateParaAlign(CCmdUI* pCmdUI, WORD wAlign)
{
	GetParaFormatSelection();
	// disable if no word wrap since alignment is meaningless
	pCmdUI->Enable( (m_nWordWrap == WrapNone) ?
		FALSE : TRUE);
	pCmdUI->SetCheck( (m_paraformat.dwMask & PFM_ALIGNMENT) ?
		((m_paraformat.wAlignment == wAlign) ? 1 : 0) : 2);
}




/*
// Needs selection to be enabled
void CRichEditView2::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	long nStartChar, nEndChar;
	GetRichEditCtrlEx().GetSel(nStartChar, nEndChar);
	pCmdUI->Enable(nStartChar != nEndChar);
	ASSERT_VALID(this);
}


// Needs something on clipboard
void CRichEditView2::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(CanPaste());
}


// Needs text in the control
void CRichEditView2::OnUpdateNeedText(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetTextLength() != 0);
}


void CRichEditView2::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetRichEditCtrlEx().CanUndo());
}

void CRichEditView2::OnUpdateNeedFind(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	pCmdUI->Enable(GetTextLength() != 0 && !pEditState->strFind.IsEmpty());
}


void CRichEditView2::OnEditCut()
{
	ASSERT_VALID(this);
	GetRichEditCtrlEx().Cut();
}


void CRichEditView2::OnEditCopy()
{
	ASSERT_VALID(this);
	GetRichEditCtrlEx().Copy();
}

void CRichEditView2::OnEditPasteFormatted()
{
	ASSERT_VALID(this);
	m_nPasteType = 0;
	GetRichEditCtrlEx().Paste();
}

void CRichEditView2::OnEditClear()
{
	ASSERT_VALID(this);
	GetRichEditCtrlEx().Clear();
}

void CRichEditView2::OnEditUndo()
{
	ASSERT_VALID(this);
	GetRichEditCtrlEx().Undo();
	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
	m_bSyncCharFormat2 = TRUE; //`
}

void CRichEditView2::OnEditSelectAll()
{
	ASSERT_VALID(this);
	GetRichEditCtrlEx().SetSel(0, -1);
}


void CRichEditView2::OnEditRepeat()
{
	ASSERT_VALID(this);
	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	if (!FindText(pEditState))
		TextNotFound(pEditState->strFind);
}


  void CRichEditView2::OnCharBold()
{
	OnCharEffect(CFM_BOLD, CFE_BOLD);
}

void CRichEditView2::OnUpdateCharBold(CCmdUI* pCmdUI)
{
	OnUpdateCharEffect(pCmdUI, CFM_BOLD, CFE_BOLD);
}

void CRichEditView2::OnCharItalic()
{
	OnCharEffect(CFM_ITALIC, CFE_ITALIC);
}

void CRichEditView2::OnUpdateCharItalic(CCmdUI* pCmdUI)
{
	OnUpdateCharEffect(pCmdUI, CFM_ITALIC, CFE_ITALIC);
}

void CRichEditView2::OnCharUnderline()
{
	OnCharEffect(CFM_UNDERLINE, CFE_UNDERLINE);
}

void CRichEditView2::OnUpdateCharUnderline(CCmdUI* pCmdUI)
{
	OnUpdateCharEffect(pCmdUI, CFM_UNDERLINE, CFE_UNDERLINE);
}

void CRichEditView2::OnParaCenter()
{
	OnParaAlign(PFA_CENTER);
}

void CRichEditView2::OnUpdateParaCenter(CCmdUI* pCmdUI)
{
	OnUpdateParaAlign(pCmdUI, PFA_CENTER);
}

void CRichEditView2::OnParaLeft()
{
	OnParaAlign(PFA_LEFT);
}

void CRichEditView2::OnUpdateParaLeft(CCmdUI* pCmdUI)
{
	OnUpdateParaAlign(pCmdUI, PFA_LEFT);
}

void CRichEditView2::OnParaRight()
{
	OnParaAlign(PFA_RIGHT);
}

void CRichEditView2::OnUpdateParaRight(CCmdUI* pCmdUI)
{
	OnUpdateParaAlign(pCmdUI, PFA_RIGHT);
}

void CRichEditView2::OnBullet()
{
	GetParaFormatSelection();
	if (m_paraformat.dwMask & PFM_NUMBERING && m_paraformat.wNumbering == PFN_BULLET)
	{
		m_paraformat.wNumbering = 0;
		m_paraformat.dxOffset = 0;
		m_paraformat.dxStartIndent = 0;
		m_paraformat.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
	else
	{
		m_paraformat.wNumbering = PFN_BULLET;
		m_paraformat.dwMask = PFM_NUMBERING;
		if (m_paraformat.dxOffset == 0)
		{
			m_paraformat.dxOffset = m_nBulletIndent;
			m_paraformat.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}
	SetParaFormat(m_paraformat);
}

void CRichEditView2::OnUpdateBullet(CCmdUI* pCmdUI)
{
	GetParaFormatSelection();
	pCmdUI->SetCheck( (m_paraformat.dwMask & PFM_NUMBERING) ? ((m_paraformat.wNumbering & PFN_BULLET) ? 1 : 0) : 2);
}


void CRichEditView2::OnColorPick(COLORREF cr)
{
//	GetCharFormatSelection();
//	m_charformat.dwMask = CFM_COLOR;
//	m_charformat.dwEffects = NULL;
//	m_charformat.crTextColor = cr;
//	SetCharFormat(m_charformat);
	GetCharFormatSelection2();
	m_charformat2.dwMask = CFM_COLOR;
	m_charformat2.dwEffects = NULL;
	m_charformat2.crTextColor = cr;
	SetCharFormat2(m_charformat2);
}

void CRichEditView2::OnColorDefault()
{
//	GetCharFormatSelection();
//	m_charformat.dwMask = CFM_COLOR;
//	m_charformat.dwEffects = CFE_AUTOCOLOR;
//	SetCharFormat(m_charformat);
	GetCharFormatSelection2();
	m_charformat2.dwMask = CFM_COLOR;
	m_charformat2.dwEffects = CFE_AUTOCOLOR;
	SetCharFormat2(m_charformat2);
}
*/







/*
// ole
void CRichEditView2::OnCancelEditCntr()
{
	m_lpRichEditOle->InPlaceDeactivate();
}


// ole
void CRichEditView2::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	CWaitCursor wait;

	CRichEditCntrItem* pItem = NULL;
	TRY
	{
		// create item from dialog results
		pItem = GetDocument()->CreateClientItem();
		pItem->m_bLock = TRUE;
		if (!dlg.CreateItem(pItem))
		{
			pItem->m_bLock = FALSE;
			AfxThrowMemoryException();  // any exception will do
		}

		HRESULT hr = InsertItem(pItem);
		pItem->UpdateItemType();

		pItem->m_bLock = FALSE;

		if (hr != NOERROR)
			AfxThrowOleException(hr);

		// if insert new object -- initially show the object
		if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE);
	}
	END_CATCH
}





// ole
void CRichEditView2::OnEditProperties()
{
	ASSERT(m_lpRichEditOle != NULL);
	CRichEditCntrItem* pSelection = GetSelectedItem();

	// make sure item is in sync with richedit's item
	CReObject reo;
	m_lpRichEditOle->GetObject(REO_IOB_SELECTION, &reo,
		REO_GETOBJ_NO_INTERFACES);

	pSelection->SyncToRichEditObject(reo);
	COlePropertiesDialog dlg(pSelection);
	dlg.DoModal();
}


// ole
void CRichEditView2::OnUpdateEditProperties(CCmdUI* pCmdUI)
{
//	pCmdUI->Enable(GetSelectedItem() != NULL);
}


// ole
void CRichEditView2::OnEditPasteSpecial()
{
	COlePasteSpecialDialog dlg;
	dlg.AddStandardFormats();
	dlg.AddFormat(_oleData.cfRichTextFormat, TYMED_HGLOBAL, AFX_IDS_RTF_FORMAT, FALSE, FALSE);
	dlg.AddFormat(CF_TEXT, TYMED_HGLOBAL, AFX_IDS_TEXT_FORMAT, FALSE, FALSE);

	if (dlg.DoModal() != IDOK)
		return;

	DVASPECT dv = dlg.GetDrawAspect();
	HMETAFILE hMF = (HMETAFILE)dlg.GetIconicMetafile();
	CLIPFORMAT cf =
		dlg.m_ps.arrPasteEntries[dlg.m_ps.nSelectedIndex].fmtetc.cfFormat;

	CWaitCursor wait;
	SetCapture();

	// we set the target type so that QueryAcceptData know what to paste
	m_nPasteType = dlg.GetSelectionType();
	GetRichEditCtrlEx().PasteSpecial(cf, dv, hMF);
	m_nPasteType = 0;

	ReleaseCapture();
}

// ole
void CRichEditView2::OnUpdateEditPasteSpecial(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(CanPaste());
}


// ole
void CRichEditView2::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFileName[_MAX_PATH];
	UINT nFileCount = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	ASSERT(nFileCount != 0);
	CHARRANGE cr;
	GetRichEditCtrlEx().GetSel(cr);
	int nMin = cr.cpMin;
	for (UINT i=0;i<nFileCount;i++)
	{
		::DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
		InsertFileAsObject(szFileName);
		GetRichEditCtrlEx().GetSel(cr);
		cr.cpMin = cr.cpMax;
		GetRichEditCtrlEx().SetSel(cr);
		UpdateWindow();
	}
	cr.cpMin = nMin;
	GetRichEditCtrlEx().SetSel(cr);
	::DragFinish(hDropInfo);
}

// ole
void CRichEditView2::OnEditInsertObject() 
{
	OnInsertObject();
}

void CRichEditView2::OnDevModeChange(LPTSTR lpDeviceName)
{
	// WM_DEVMODECHANGE forwarded by the main window of the app
	CDC dc;
	AfxGetApp()->CreatePrinterDC(dc);
	OnPrinterChanged(dc);
}
*/



void CRichEditView2::OnEditFind()
{
	ASSERT_VALID(this);
	OnEditFindReplace(TRUE);
}

void CRichEditView2::OnEditReplace()
{
	ASSERT_VALID(this);
	OnEditFindReplace(FALSE);
}




// 
void CRichEditView2::OnFormatFont()
{
	//, this will need some fixing up to get working
//	GetCharFormatSelection();
//	CFontDialog dlg(m_logfont, CF_BOTH | CF_NOOEMFONTS);
//	if (dlg.DoModal() == IDOK)
//	{
//		dlg.GetCharFormat(m_charformat);
//		SetCharFormat(m_charformat);
//	}
}



// CRichEditView2 attributes
//--------------------------------------------------------------------------------------------------

BOOL AFX_CDECL CRichEditView2::IsRichEditFormat(CLIPFORMAT cf)
{
//.ole
/*
	return ((cf == _oleData.cfRichTextFormat) ||
		(cf == _oleData.cfRichTextAndObjects) || (cf == CF_TEXT));
*/
	return ((cf == CF_TEXT) || (cf = theApp.m_cfRtf));
}

BOOL CRichEditView2::CanPaste() const
{
//. ole  _oledata
	return (CountClipboardFormats() != 0) &&
		(IsClipboardFormatAvailable(CF_TEXT) ||
//		IsClipboardFormatAvailable(_oleData.cfRichTextFormat) ||
//		IsClipboardFormatAvailable(_oleData.cfEmbedSource) ||
//		IsClipboardFormatAvailable(_oleData.cfEmbeddedObject) ||
//		IsClipboardFormatAvailable(_oleData.cfFileName) ||
//		IsClipboardFormatAvailable(_oleData.cfFileNameW) ||
//		IsClipboardFormatAvailable(CF_METAFILEPICT) ||
//		IsClipboardFormatAvailable(CF_DIB) ||
//		IsClipboardFormatAvailable(CF_BITMAP) ||
		IsClipboardFormatAvailable(theApp.m_cfRtf) || 
		GetRichEditCtrlEx().CanPaste());
}


/*
CHARFORMAT& CRichEditView2::GetCharFormatSelection()
{
	if (m_bSyncCharFormat)
	{
		GetRichEditCtrlEx().GetSelectionCharFormat(m_charformat);
		m_bSyncCharFormat = FALSE;
	}
	return m_charformat;
}
*/


CHARFORMAT2& CRichEditView2::GetCharFormatSelection2() //`
{
	if (m_bSyncCharFormat)
	{
		GetRichEditCtrlEx().GetSelectionCharFormat2(m_charformat2);
		TRACE("getcharformatselection yheight=%d\n", m_charformat2.yHeight);
		m_bSyncCharFormat = FALSE;
	}
	return m_charformat2;
}



PARAFORMAT& CRichEditView2::GetParaFormatSelection()
{
	if (m_bSyncParaFormat)
	{
		GetRichEditCtrlEx().GetParaFormat(m_paraformat);
		m_bSyncParaFormat = FALSE;
	}
	return m_paraformat;
}

/*
void CRichEditView2::SetCharFormat(CHARFORMAT cf)
{
	CWaitCursor wait;
	GetRichEditCtrlEx().SetSelectionCharFormat(cf);
	m_bSyncCharFormat = TRUE;
}
*/


void CRichEditView2::SetCharFormat2(CHARFORMAT2 cf2)
{
	CWaitCursor wait;
	ASSERT(cf2.cbSize == sizeof(CHARFORMAT2));
//	GetRichEditCtrlEx().SetSelectionCharFormat(cf);
//	GetRichEditCtrlEx().SendMessage(EM_SETCHARFORMAT, SCF_WORD | SCF_SELECTION, (LPARAM) &cf2);
	::SendMessage(m_hWnd, EM_SETCHARFORMAT, SCF_WORD | SCF_SELECTION, (LPARAM) &cf2);
	m_bSyncCharFormat = TRUE;
}



BOOL CRichEditView2::SetParaFormat(PARAFORMAT& pf)
{
	BOOL bRetVal;
	CWaitCursor wait;
	bRetVal = GetRichEditCtrlEx().SetParaFormat(pf);
	m_bSyncParaFormat = TRUE;
	return bRetVal;
}


CRichEditCntrItem* CRichEditView2::GetSelectedItem() const
{
//. ole
/*
	ASSERT(m_lpRichEditOle != NULL);
	CRichEditDoc* pDoc = GetDocument();
	CRichEditCntrItem* pItem = NULL;

	CReObject reo;
	HRESULT hr = m_lpRichEditOle->GetObject(REO_IOB_SELECTION, &reo, REO_GETOBJ_ALL_INTERFACES);
	// reo's interfaces are all in UNICODE
	if (GetScode(hr) == S_OK)
	{
		pItem = pDoc->LookupItem(reo.poleobj);
		if (pItem == NULL)
			pItem = pDoc->CreateClientItem(&reo);
		ASSERT(pItem != NULL);
	}
	return pItem;
*/
	return NULL;
}


CRichEditCntrItem* CRichEditView2::GetInPlaceActiveItem() const
{
//. ole
/*
	ASSERT(m_lpRichEditOle != NULL);
	CRichEditDoc* pDoc = GetDocument();
	CRichEditCntrItem* pItem = NULL;

	CReObject reo;
	HRESULT hr = m_lpRichEditOle->GetObject(REO_IOB_SELECTION, &reo, REO_GETOBJ_ALL_INTERFACES);
	// reo's interfaces are all in UNICODE
	if (GetScode(hr) == S_OK && (reo.dwFlags & REO_INPLACEACTIVE))
	{
		pItem = pDoc->LookupItem(reo.poleobj);
		if (pItem == NULL)
			pItem = pDoc->CreateClientItem(&reo);
		ASSERT(pItem != NULL);
	}
	return pItem;
*/
	return NULL;
}




// Operations
//--------------------------------------------------------------------------------------------------

HRESULT CRichEditView2::InsertItem(CRichEditCntrItem* pItem)
{
//. ole
/*
	ASSERT(m_lpRichEditOle != NULL);
	CReObject reo(pItem);
	reo.cp = REO_CP_SELECTION;

	HRESULT hr = m_lpRichEditOle->InsertObject(&reo);

	CHARRANGE cr;
	GetRichEditCtrlEx().GetSel(cr);
	cr.cpMin = cr.cpMax -1;
	GetRichEditCtrlEx().SetSel(cr);
	return hr;
*/
	return NULL;
}


void CRichEditView2::InsertFileAsObject(LPCTSTR lpszFileName)
{
//. ole
/*
	CString str = lpszFileName;
	CWaitCursor wait;
	CRichEditCntrItem* pItem = NULL;
	TRY
	{
		// create item from dialog results
		pItem = GetDocument()->CreateClientItem();
		pItem->m_bLock = TRUE;
		if (!pItem->CreateFromFile(str))
			AfxThrowMemoryException();  // any exception will do
		pItem->UpdateLink();
		InsertItem(pItem);
		pItem->m_bLock = FALSE;
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			pItem->m_bLock = FALSE;
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
	}
	END_CATCH
*/
}


void CRichEditView2::DoPaste(COleDataObject& dataobj, CLIPFORMAT cf, HMETAFILEPICT hMetaPict)
{
//. ole
/*
	CWaitCursor wait;

	CRichEditCntrItem* pItem = NULL;
	TRY
	{
		// create item from dialog results
		pItem = GetDocument()->CreateClientItem();
		pItem->m_bLock = TRUE;

		if (m_nPasteType == COlePasteSpecialDialog::pasteLink)      // paste link
		{
			if (!pItem->CreateLinkFromData(&dataobj))
				AfxThrowMemoryException();  // any exception will do
		}
		else if (m_nPasteType == COlePasteSpecialDialog::pasteNormal)
		{
			if (!pItem->CreateFromData(&dataobj))
				AfxThrowMemoryException();      // any exception will do
		}
		else if (m_nPasteType == COlePasteSpecialDialog::pasteStatic)
		{
			if (!pItem->CreateStaticFromData(&dataobj))
				AfxThrowMemoryException();      // any exception will do
		}
		else
		{
			// paste embedded
			if (!pItem->CreateFromData(&dataobj) &&
				!pItem->CreateStaticFromData(&dataobj))
			{
				AfxThrowMemoryException();      // any exception will do
			}
		}

		if (cf == 0)
		{
			// copy the current iconic representation
			FORMATETC fmtetc;
			fmtetc.cfFormat = CF_METAFILEPICT;
			fmtetc.dwAspect = DVASPECT_ICON;
			fmtetc.ptd = NULL;
			fmtetc.tymed = TYMED_MFPICT;
			fmtetc.lindex = 1;
			HGLOBAL hObj = dataobj.GetGlobalData(CF_METAFILEPICT, &fmtetc);
			if (hObj != NULL)
			{
				pItem->SetIconicMetafile(hObj);
				// the following code is an easy way to free a metafile pict
				STGMEDIUM stgMed;
				memset(&stgMed, 0, sizeof(stgMed));
				stgMed.tymed = TYMED_MFPICT;
				stgMed.hGlobal = hObj;
				ReleaseStgMedium(&stgMed);
			}

			// set the current drawing aspect
			hObj = dataobj.GetGlobalData((CLIPFORMAT)_oleData.cfObjectDescriptor);
			if (hObj != NULL)
			{
				ASSERT(hObj != NULL);
				// got CF_OBJECTDESCRIPTOR ok.  Lock it down and extract size.
				LPOBJECTDESCRIPTOR pObjDesc = (LPOBJECTDESCRIPTOR)GlobalLock(hObj);
				ASSERT(pObjDesc != NULL);
				((COleClientItem*)pItem)->SetDrawAspect((DVASPECT)pObjDesc->dwDrawAspect);
				GlobalUnlock(hObj);
				GlobalFree(hObj);
			}
		}
		else
		{
			if (hMetaPict != NULL)
			{
				pItem->SetIconicMetafile(hMetaPict);
				((COleClientItem*)pItem)->SetDrawAspect(DVASPECT_ICON);
			}
			else
				((COleClientItem*)pItem)->SetDrawAspect(DVASPECT_CONTENT);
		}


		HRESULT hr = InsertItem(pItem);
		pItem->UpdateItemType();

		pItem->m_bLock = FALSE;

		if (hr != NOERROR)
			AfxThrowOleException(hr);

	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			pItem->m_bLock = FALSE;
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
	}
	END_CATCH
*/
}





// Virtuals (events)
//--------------------------------------------------------------------------------------------------

// This is typically called by the view when it gets a WM_DEVMODECHANGE.
// Also called during page setup.
void CRichEditView2::OnPrinterChanged(const CDC& dcPrinter)
{
	CSize size;
	if (dcPrinter.m_hDC != NULL)
	{
		// This will fill in the page size
		size.cx = MulDiv(dcPrinter.GetDeviceCaps(PHYSICALWIDTH), 1440,
			dcPrinter.GetDeviceCaps(LOGPIXELSX));
		size.cy = MulDiv(dcPrinter.GetDeviceCaps(PHYSICALHEIGHT), 1440,
			dcPrinter.GetDeviceCaps(LOGPIXELSY));
	}
	else
		size = CSize(8*1440+720, 11*1440); // 8.5" by 11"
	if (GetPaperSize() != size)
	{
		SetPaperSize(size);
		if (m_nWordWrap == WrapToTargetDevice) //wrap to ruler
			WrapChanged();
	}
}


BOOL CRichEditView2::OnPasteNativeObject(LPSTORAGE)
{
//. ole
	// use this function to pull out native data from an embedded object
	// one would typically do this by create a COleStreamFile and attaching it
	// to an archive
	return FALSE;
}


HRESULT CRichEditView2::GetClipboardData(CHARRANGE* /*lpchrg*/, DWORD /*reco*/,
	LPDATAOBJECT /*lpRichDataObj*/, LPDATAOBJECT* /*lplpdataobj*/)
{
//. ole?
	return E_NOTIMPL;
}


HRESULT CRichEditView2::QueryAcceptData(LPDATAOBJECT lpdataobj,
	CLIPFORMAT* lpcfFormat, DWORD /*dwReco*/, BOOL bReally, HGLOBAL hMetaPict)
{
	ASSERT(lpcfFormat != NULL);
	if (!bReally) // not actually pasting
		return S_OK;
	// if direct pasting a particular native format allow it
	if (IsRichEditFormat(*lpcfFormat))
		return S_OK;

	COleDataObject dataobj;
	dataobj.Attach(lpdataobj, FALSE);
	// if format is 0, then force particular formats if available
	if (*lpcfFormat == 0 && (m_nPasteType == 0))
	{
//. ole
/*
		if (dataobj.IsDataAvailable((CLIPFORMAT)_oleData.cfRichTextAndObjects)) // native avail, let richedit do as it wants
			return S_OK;
		else if (dataobj.IsDataAvailable((CLIPFORMAT)_oleData.cfRichTextFormat))
		{
			*lpcfFormat = (CLIPFORMAT)_oleData.cfRichTextFormat;
			return S_OK;
		}
*/
		if (dataobj.IsDataAvailable((CLIPFORMAT) theApp.m_cfRtf))
		{
			*lpcfFormat = (CLIPFORMAT) theApp.m_cfRtf;
			return S_OK;
		}
		else if (dataobj.IsDataAvailable(CF_TEXT))
		{
			*lpcfFormat = CF_TEXT;
			return S_OK;
		}
	}

	// Paste COM formats
	DoPaste(dataobj, *lpcfFormat, hMetaPict);
	return S_FALSE;
//	return S_OK;
}


HRESULT CRichEditView2::GetWindowContext(LPOLEINPLACEFRAME* lplpFrame,
	LPOLEINPLACEUIWINDOW* lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
//. ole
/*
	CRichEditCntrItem* pItem = GetSelectedItem();
	if (pItem == NULL)
		return E_FAIL;
	pItem->m_pView = this;
	HRESULT hr = pItem->GetWindowContext(lplpFrame, lplpDoc, lpFrameInfo);
	pItem->m_pView = NULL;
	return hr;
*/
	return E_FAIL;
}


HRESULT CRichEditView2::ShowContainerUI(BOOL b)
{
//. ole
/*
	CRichEditCntrItem* pItem = GetSelectedItem();
	if (pItem == NULL)
		return E_FAIL;
	if (b)
		pItem->m_pView = this;
	HRESULT hr = pItem->ShowContainerUI(b);
	if (FAILED(hr) || !b)
		pItem->m_pView = NULL;
	return hr;
*/
	return E_FAIL;
}




// Find & Replace
//--------------------------------------------------------------------------------------------------


void CRichEditView2::AdjustDialogPosition(CDialog* pDlg)
{
	ASSERT(pDlg != NULL);
	long lStart, lEnd;
	GetRichEditCtrlEx().GetSel(lStart, lEnd);
	CPoint point = GetRichEditCtrlEx().GetCharPos(lStart);
	ClientToScreen(&point);
	CRect rectDlg;
	pDlg->GetWindowRect(&rectDlg);
	if (rectDlg.PtInRect(point))
	{
		if (point.y > rectDlg.Height())
			rectDlg.OffsetRect(0, point.y - rectDlg.bottom - 20);
		else
		{
			int nVertExt = GetSystemMetrics(SM_CYSCREEN);
			if (point.y + rectDlg.Height() < nVertExt)
				rectDlg.OffsetRect(0, 40 + point.y - rectDlg.top);
		}
		pDlg->MoveWindow(&rectDlg);
	}
}


void CRichEditView2::OnEditFindReplace(BOOL bFindOnly)
{
	ASSERT_VALID(this);
	m_bFirstSearch = TRUE;
	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	
	if (pEditState->pFindReplaceDlg != NULL)
	{
		if (pEditState->bFindOnly == bFindOnly)
		{
			pEditState->pFindReplaceDlg->SetActiveWindow();
			pEditState->pFindReplaceDlg->ShowWindow(SW_SHOW);
			return;
		}
		else
		{
			ASSERT(pEditState->bFindOnly != bFindOnly);
			pEditState->pFindReplaceDlg->SendMessage(WM_CLOSE);
			ASSERT(pEditState->pFindReplaceDlg == NULL);
			ASSERT_VALID(this);
		}
	}

	// Get find and replace text
	// if selection is empty or spans multiple lines use old find text
	CString strFind = GetRichEditCtrlEx().GetSelText();
	if (strFind.IsEmpty() || (strFind.FindOneOf(_T("\n\r")) != -1))
		strFind = pEditState->strFind;
	CString strReplace = pEditState->strReplace;

	// Create new find/replace dialog
	pEditState->pFindReplaceDlg = new CFindReplaceDialog;
	ASSERT(pEditState->pFindReplaceDlg != NULL);
	DWORD dwFlags = NULL;
	if (pEditState->bNext) dwFlags |= FR_DOWN;
	if (pEditState->bCase) dwFlags |= FR_MATCHCASE;
	if (pEditState->bWord) dwFlags |= FR_WHOLEWORD;
	dwFlags |= FR_HIDEUPDOWN; // hide stuff that RichEdit doesn't support
	if (!pEditState->pFindReplaceDlg->Create(bFindOnly, strFind, strReplace, dwFlags, this))
	{
		pEditState->pFindReplaceDlg = NULL;
		ASSERT_VALID(this);
		return;
	}

	// Show the dialog
	ASSERT(pEditState->pFindReplaceDlg != NULL);
	pEditState->bFindOnly = bFindOnly;
	pEditState->pFindReplaceDlg->SetActiveWindow();
	pEditState->pFindReplaceDlg->ShowWindow(SW_SHOW);
	ASSERT_VALID(this);
}


void CRichEditView2::OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase, BOOL bWord)
{
	ASSERT_VALID(this);

	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	pEditState->strFind = lpszFind;
	pEditState->bCase = bCase;
	pEditState->bWord = bWord;
	pEditState->bNext = bNext;

	if (!FindText(pEditState))
		TextNotFound(pEditState->strFind);
	else
		AdjustDialogPosition(pEditState->pFindReplaceDlg);
	ASSERT_VALID(this);
}


void CRichEditView2::OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,
	BOOL bWord, LPCTSTR lpszReplace)
{
	ASSERT_VALID(this);
	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	pEditState->strFind = lpszFind;
	pEditState->strReplace = lpszReplace;
	pEditState->bCase = bCase;
	pEditState->bWord = bWord;
	pEditState->bNext = bNext;

	if (!SameAsSelected(pEditState->strFind, pEditState->bCase, pEditState->bWord))
	{
		if (!FindText(pEditState))
			TextNotFound(pEditState->strFind);
		else
			AdjustDialogPosition(pEditState->pFindReplaceDlg);
		return;
	}

	GetRichEditCtrlEx().ReplaceSel(pEditState->strReplace);
	if (!FindText(pEditState))
		TextNotFound(pEditState->strFind);
	else
		AdjustDialogPosition(pEditState->pFindReplaceDlg);
	ASSERT_VALID(this);
}


void CRichEditView2::OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase, BOOL bWord)
{
	ASSERT_VALID(this);
	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	pEditState->strFind = lpszFind;
	pEditState->strReplace = lpszReplace;
	pEditState->bCase = bCase;
	pEditState->bWord = bWord;
	pEditState->bNext = TRUE;

	CWaitCursor wait;
	// no selection or different than what looking for
	if (!SameAsSelected(pEditState->strFind, pEditState->bCase, pEditState->bWord))
	{
		if (!FindText(pEditState))
		{
			TextNotFound(pEditState->strFind);
			return;
		}
	}

	GetRichEditCtrlEx().HideSelection(TRUE, FALSE);
	do
	{
		GetRichEditCtrlEx().ReplaceSel(pEditState->strReplace);
	} while (FindTextSimple(pEditState));
	TextNotFound(pEditState->strFind);
	GetRichEditCtrlEx().HideSelection(FALSE, FALSE);

	ASSERT_VALID(this);
}


LRESULT CRichEditView2::OnFindReplaceCmd(WPARAM, LPARAM lParam)
{
	ASSERT_VALID(this);

	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier(lParam);
	ASSERT(pDialog != NULL);
	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	ASSERT(pDialog == pEditState->pFindReplaceDlg);
	
	if (pDialog->IsTerminating())
		pEditState->pFindReplaceDlg = NULL;
	else if (pDialog->FindNext())
	{
		OnFindNext(pDialog->GetFindString(), pDialog->SearchDown(),
			pDialog->MatchCase(), pDialog->MatchWholeWord());
	}
	else if (pDialog->ReplaceCurrent())
	{
		ASSERT(!pEditState->bFindOnly);
		OnReplaceSel(pDialog->GetFindString(),
			pDialog->SearchDown(), pDialog->MatchCase(), pDialog->MatchWholeWord(),
			pDialog->GetReplaceString());
	}
	else if (pDialog->ReplaceAll())
	{
		ASSERT(!pEditState->bFindOnly);
		OnReplaceAll(pDialog->GetFindString(), pDialog->GetReplaceString(),
			pDialog->MatchCase(), pDialog->MatchWholeWord());
	}
	ASSERT_VALID(this);
	return 0;
}


BOOL CRichEditView2::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase, BOOL /*bWord*/)
{
	// check length first
	size_t nLen = lstrlen(lpszCompare);
	long lStartChar, lEndChar;
	GetRichEditCtrlEx().GetSel(lStartChar, lEndChar);
	if (nLen != (size_t)(lEndChar - lStartChar))
		return FALSE;

	// length is the same, check contents
	CString strSelect = GetRichEditCtrlEx().GetSelText();
	return (bCase && lstrcmp(lpszCompare, strSelect) == 0) ||
		(!bCase && lstrcmpi(lpszCompare, strSelect) == 0);
}


BOOL CRichEditView2::FindText(_AFX_RICHEDIT_STATE* pEditState)
{
	ASSERT(pEditState != NULL);
	return FindText(pEditState->strFind, pEditState->bCase, pEditState->bWord);
}


BOOL CRichEditView2::FindText(LPCTSTR lpszFind, BOOL bCase, BOOL bWord)
{
	ASSERT_VALID(this);
	CWaitCursor wait;
	return FindTextSimple(lpszFind, bCase, bWord);
}


BOOL CRichEditView2::FindTextSimple(_AFX_RICHEDIT_STATE* pEditState)
{
	ASSERT(pEditState != NULL);
	return FindTextSimple(pEditState->strFind, pEditState->bCase, pEditState->bWord);
}


BOOL CRichEditView2::FindTextSimple(LPCTSTR lpszFind, BOOL bCase, BOOL bWord)
{
	USES_CONVERSION;
	ASSERT(lpszFind != NULL);
	FINDTEXTEX ft;

	GetRichEditCtrlEx().GetSel(ft.chrg);
	if (m_bFirstSearch)
	{
		m_lInitialSearchPos = ft.chrg.cpMin;
		m_bFirstSearch = FALSE;
	}

	ft.lpstrText = T2A((LPTSTR)lpszFind);
	if (ft.chrg.cpMin != ft.chrg.cpMax) // i.e. there is a selection
	{

#ifndef _UNICODE
		// If byte at beginning of selection is a DBCS lead byte,
		// increment by one extra byte.
		TEXTRANGE textRange;
		TCHAR ch[2];
		textRange.chrg.cpMin = ft.chrg.cpMin;
		textRange.chrg.cpMax = ft.chrg.cpMin + 1;
		textRange.lpstrText = ch;
		GetRichEditCtrlEx().SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&textRange);
		if (_istlead(ch[0]))
		{
			ASSERT(ft.chrg.cpMax - ft.chrg.cpMin >= 2);
			ft.chrg.cpMin++;
		}
#endif

		ft.chrg.cpMin++;
	}

	if (m_lInitialSearchPos >= 0)
		ft.chrg.cpMax = GetTextLength();
	else
		ft.chrg.cpMax = GetTextLength()+m_lInitialSearchPos;

	DWORD dwFlags = bCase ? FR_MATCHCASE : 0;
	dwFlags |= bWord ? FR_WHOLEWORD : 0;
	dwFlags |= FR_DOWN; //` This is needed for RichEdit 2.0 and later!

	// if we find the text return TRUE
	if (FindAndSelect(dwFlags, ft) != -1)
		return TRUE;
	// if the original starting point was not the beginning of the buffer
	// and we haven't already been here
	else if (m_lInitialSearchPos > 0)
	{
		ft.chrg.cpMin = 0;
		ft.chrg.cpMax = m_lInitialSearchPos;
		m_lInitialSearchPos = m_lInitialSearchPos - GetTextLength();
		return FindAndSelect(dwFlags, ft) != -1;
	}
	// not found
	else
		return FALSE;
}


long CRichEditView2::FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft)
{
	dwFlags |= FR_DOWN; //` This is needed for RichEdit 2.0 and later!
	long index = GetRichEditCtrlEx().FindText(dwFlags, &ft);
	if (index != -1) // i.e. we found something
		GetRichEditCtrlEx().SetSel(ft.chrgText);
	return index;
}


void CRichEditView2::TextNotFound(LPCTSTR lpszFind)
{
	ASSERT_VALID(this);
	m_bFirstSearch = TRUE;
	OnTextNotFound(lpszFind);
}


void CRichEditView2::OnTextNotFound(LPCTSTR lpszFind)
{
//	MessageBeep(MB_ICONHAND);
	//` Replace the beep with a message box
//	LPCTSTR pszMsg = " was not found.";
//	TCHAR *lpszBuf = new TCHAR[_tcslen(lpszMsg) + _tcslen(lpszFind)];
//	wsprintf( lpszBuf, "%s %s", lpszFind, lpszMsg );
//	AfxMessageBox( lpszBuf );
	CString strMsg;
	strMsg.Format("The text '%s' was not found.", lpszFind);
	AfxMessageBox(strMsg, MB_ICONINFORMATION);
}



// Diagnostics
//----------------------------------------------------------------------------

#ifdef _DEBUG
void CRichEditView2::AssertValid() const
{
	CCtrlView::AssertValid();
	ASSERT_VALID(&m_aPageStart);
	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	//, bug: this was bombing on find. seem to have fixed it.
	if (pEditState->pFindReplaceDlg != NULL)
		ASSERT_VALID(pEditState->pFindReplaceDlg);
}


void CRichEditView2::Dump(CDumpContext& dc) const
{
	CCtrlView::Dump(dc);
	AFX_DUMP1(dc, "\nm_aPageStart ", &m_aPageStart);
	AFX_DUMP0(dc, "\n Static Member Data:");
	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
	if (pEditState->pFindReplaceDlg != NULL)
	{
		AFX_DUMP1(dc, "\npFindReplaceDlg = ",
			(void*)pEditState->pFindReplaceDlg);
		AFX_DUMP1(dc, "\nbFindOnly = ", pEditState->bFindOnly);
	}
	AFX_DUMP1(dc, "\nstrFind = ", pEditState->strFind);
	AFX_DUMP1(dc, "\nstrReplace = ", pEditState->strReplace);
	AFX_DUMP1(dc, "\nbCase = ", pEditState->bCase);
	AFX_DUMP1(dc, "\nbWord = ", pEditState->bWord);
	AFX_DUMP1(dc, "\nbNext = ", pEditState->bNext);
}


//. ole  CRichEditDoc* CRichEditView2::GetDocument() const // non-debug version is inline
CNeoDoc* CRichEditView2::GetDocument() // non-debug version is inline
{
//	ASSERT(FALSE);
//	return NULL;
//. ole 	ASSERT(theApp.m_pRedoc->IsKindOf(RUNTIME_CLASS(CRichEditDoc)));
//. ole 	return theApp.m_pRedoc;
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNeoDoc))); //`
	return (CNeoDoc*)m_pDocument;
}
#endif //_DEBUG



// OLE Client support and commands
//--------------------------------------------------------------------------------------------------

BOOL CRichEditView2::IsSelected(const CObject* pDocItem) const
{
//. ole
//	return (pDocItem == GetSelectedItem());
	return FALSE;
}




//--------------------------------------------------------------------------------------------------

/*
#ifndef _AFX_ENABLE_INLINES

static const char _szAfxWinInl[] = "afxrich.inl";
#undef THIS_FILE
#define THIS_FILE _szAfxWinInl
#define _AFXRICH_INLINE
#include "afxrich.inl"

#endif //_AFX_ENABLE_INLINES
*/


//#ifdef AFX_INIT_SEG
//#pragma code_seg(AFX_INIT_SEG)
//#endif







//` Message Handlers
//----------------------------------------------------------------------------





void CRichEditView2::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	xTRACE("CRichEditView2::OnActivateView(bActivate=%d)\n", bActivate);

	// Call base class
	// i think this sends the setfocus message to this window also,
	// as OnSetFocus gets called at this point
//	CRichEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	CCtrlView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	// Activate this view's parent, the cviewrtf window
	// if we activate cviewrtf here, it will set focus to this control
	// so cviewrtf will be the active view, but this window will have the focus...
	if (bActivate)
	{
		xTRACE("  set CViewRtf to be the active view\n");
		CFrameChild* pChild = (CFrameChild*) GetParentFrame();
		ASSERT_VALID(pChild);
		CView* pParentView = (CView*) GetParent(); // cviewrtf
		pChild->SetActiveView(pParentView, TRUE);
	}
}


void CRichEditView2::OnSetFocus(CWnd* pOldWnd) 
{
	xTRACE("CRichEditView2::OnSetFocus - do nothing\n");
//	CRichEditView::OnSetFocus(pOldWnd);
	CCtrlView::OnSetFocus(pOldWnd);
}


// if user was typing something in and clicks on search tab, this is the only kill focus
// received of the three rtf windows
// we could send a hintSave message at this point?
void CRichEditView2::OnKillFocus(CWnd* pNewWnd) 
{
	xTRACE("CRichEditView2::OnKillFocus - do nothing\n");
//	CRichEditView::OnKillFocus(pNewWnd);
	CCtrlView::OnKillFocus(pNewWnd);
}








// Printing support
//--------------------------------------------------------------------------------------------------


// copied from mfc
void CRichEditView2::OnFilePrintPreview()
{
	// In derived classes, implement special window handling here.
	// Be sure to Unhook Frame Window close if hooked.

	// test - make this view a child of the child frame
//	m_pwndOldParent = SetParent(theApp.GetChildFrame());

	// Must not create this on the frame - must outlive this function.
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that Print preview 
	// succeeded or failed, but rather what actions are necessary at this point:
	// If DoPrintPreview returns TRUE, it means that OnEndPrintPreview will be 
	//		(or has already been) called and the pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview WILL NOT 
	//		be called and that cleanup, including deleting pState must be done here.
	//` use our own toolbar and view class
//	if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this, RUNTIME_CLASS(CPreviewView), pState))
	if (!DoPrintPreview(IDD_PREVIEW_TOOLBAR, this, RUNTIME_CLASS(CPreviewViewEx), pState))
	{
		// In derived classes, reverse special window handling here for Preview failure case
		TRACE0("Error: DoPrintPreview failed.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		delete pState;      // preview failed to initialize, delete State now
	}
}




// OnPreparePrinting
// Called by the framework before a document is printed or previewed. 
// The default implementation does nothing.
// You must override this function to enable printing and print preview. 
BOOL CRichEditView2::OnPreparePrinting(CPrintInfo* pInfo) 
{
	xTRACE("CRichEditView2::OnPreparePrinting\n");

	// Get margins from app object (saved in registry)
	// Note that the margins used by PrintPage are relative to the physical page, not the logical 
	// page. Thus, margins of zero will often clip the text since many printers have unprintable 
	// areas on the page. To avoid clipping your text, you should call use SetMargins to set 
	// reasonable printer margins before printing.
//	CRichEditView::SetMargins(theApp.m_rectPageMargins);
	// this sets the m_rectMargin rect;
	SetMargins(theApp.m_rectPageMargins);

	// Tell CRichEditView about any changes to the printer...
	// WordPad does this through printer change notification message, but that was a little overkill...
	// This is necessary so if printer orientation changes, for instance, the printable area gets updated
	CDC dc;
	AfxGetApp()->CreatePrinterDC(dc);
	OnPrinterChanged(dc);

	// Get print dialog structure from pInfo
	CPrintDialog* pPD = pInfo->m_pPD;
	PRINTDLG* ppd = &pPD->m_pd;

	// Enable "Selection" option
//	// Disable "Page Nums" option and enable "Selection" option
//	ppd->Flags |= PD_NOPAGENUMS;
	ppd->Flags &= ~PD_NOSELECTION;

	// Select "All" or "Selection" option based on if any text is selected in rtf control
	WORD wSelType = GetRichEditCtrlEx().GetSelectionType();
	BOOL bTextSelected = (wSelType != SEL_EMPTY);
	if (!bTextSelected)
		ppd->Flags |= PD_ALLPAGES;
	else
		ppd->Flags |= PD_SELECTION;

	// Causes the system to display the Print Setup dialog box rather than the Print dialog box. 
	// PD_PRINTSETUP 


	// DoPreparePrinting displays the Print dialog box and creates a printer device context. 
	// If you want to initialize the Print dialog box with values other than the defaults, assign 
	// values to the members of pInfo. For example, if you know the length of the document, 
	// pass the value to the SetMaxPage member function of pInfo before calling DoPreparePrinting. 
	// This value is displayed in the To: box in the Range portion of the Print dialog box. 
	// Call DoPreparePrinting to invoke the Print dialog box	
	BOOL bRet = DoPreparePrinting(pInfo);

//	if (pInfo->m_pPD->PrintSelection())
//	{
//		AfxMessageBox("printing selection only");
		// Get current selection range
//		CHARRANGE cr;
//		GetRichEditCtrlEx().GetSel(cr);
//	}
//	pInfo->m_pPD->m_pd.hDevMode

	// Return nonzero to begin printing; 0 if the print job has been canceled.
	return bRet;
}



// OnBeginPrinting
// Called by the framework at the beginning of a print or print preview job, after OnPreparePrinting 
// has been called. 
//` code adapted from viewrich.cpp
void CRichEditView2::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	xTRACE("CRichEditView2::OnBeginPrinting\n");

	ASSERT_VALID(this);
//  ASSERT_VALID(pDC);

	// initialize page start vector
//	ASSERT(m_aPageStart.GetSize() == 0);
//	m_aPageStart.Add(0);
//	ASSERT(m_aPageStart.GetSize() > 0);
//	GetRichEditCtrlEx().FormatRange(NULL, FALSE); // required by RichEdit to clear out cache
//	ASSERT_VALID(this);

	// test Create window and draw rtf text into it
//	CRect r;
//	r.SetRect(0, 0, 900, 700);
//	CString strClassName = AfxRegisterWndClass(NULL, 0, (HBRUSH) ::GetStockObject(WHITE_BRUSH), 0);
//	m_wnd.CreateEx(NULL, strClassName, "Test", WS_VISIBLE | WS_OVERLAPPEDWINDOW, r, NULL, 0);
//	CDC* pDC2 = m_wnd.GetDC();
//	pDC2->TextOut(0, 0, strClassName);
//	m_wnd.ReleaseDC(pDC2);

	//` Initialize fonts margins etc
	// have to make this view derive from cviewex to do this
	// can't because this is derived from cctrlview!
	// all the routine does is initialize the print info object...
//	CViewEx::OnBeginPrinting(pDC, pInfo);
	CPrintInfoMore& rpim = theApp.m_printinfo;
	CNeoDoc* pDoc = CNeoDoc::GetDoc();
	rpim.Initialize(pDC);
	rpim.InitFonts(pDC);
	rpim.m_strViewName = _T("Text View"); //. get from view object!
	rpim.m_strObjectName = pDoc->GetCurrentObject()->GetPropertyText(propName);

	// Get header and footer height - used by GetPrintRect() (convert pixels to twips)
//	m_nHeaderHeightTwips = rpim.m_nHeaderLines * rpim.m_nPrintLineHeight * 1440 / rpim.m_nPixelsPerInchY;
//	m_nFooterHeightTwips = rpim.m_nFooterLines * rpim.m_nPrintLineHeight * 1440 / rpim.m_nPixelsPerInchY;
	m_nHeaderHeightTwips = rpim.m_nHeaderLines * rpim.m_nHeaderLineHeight * 1440 / rpim.m_nPixelsPerInchY;
	m_nFooterHeightTwips = rpim.m_nFooterLines * rpim.m_nHeaderLineHeight * 1440 / rpim.m_nPixelsPerInchY;

	// Get printable area rectangle (twips)
	m_rectPrintableArea.SetRect(
			m_rectMargin.left, 
			m_rectMargin.top + m_nHeaderHeightTwips, 
			m_sizePaper.cx - m_rectMargin.right, 
			m_sizePaper.cy - m_rectMargin.bottom - m_nFooterHeightTwips
			);

	// initialize page start vector
	ASSERT(m_aPageStart.GetSize() == 0);

	//` Get start and stop characters
	// with fixes for non-stop print with blank pages (using richedit2.0) adapted from 
	// http://www.codeguru.com/Cpp/controls/richedit/comments.php/c2389/?thread=30366
	// Apparently, Richedit 2.0 uses \r to indicate line break, while Richedit 1.0 uses \r\n. 
	// Richedit 2.0 emulates Richedit 1.0 in almost every function (streaming, getting text 
	// length etc. return "right result"), but FormatRange (i.e EM_FORMATRANGE) uses 
	// "real text length" (i.e new line is counted as one symbol)
	// So need to adjust the m_nPrintStopChar value...
	CString strRich;
	if (pInfo->m_pPD->PrintSelection())
	{
		// User has elected to print just the current selection, so get selection range
		GetRichEditCtrlEx().GetSel(m_nPrintStartChar, m_nPrintStopChar);
		// get selected text, for error correction
		strRich = GetRichEditCtrlEx().GetSelText();
	}
	else
	{
		// Print entire document
		m_nPrintStartChar = 0;
		m_nPrintStopChar = GetTextLength();
		// get all text, for error correction
		GetRichEditCtrlEx().GetWindowText(strRich);
	}
	// correct for error caused by using richedit2.0
	int nLength = m_nPrintStopChar;
	int nS=0;
	while(nS!=-1)
	{
		nS=strRich.Find("\r\n",nS);
		if(nS!=-1)
		{
			nLength--;
			nS++;
		}
	}
	m_nPrintStopChar = nLength;
	// end error correction

	m_aPageStart.Add(m_nPrintStartChar);	
	TRACE("  begin printing: start and stop are %d %d\n", m_nPrintStartChar, m_nPrintStopChar);

	ASSERT(m_aPageStart.GetSize() > 0);
	GetRichEditCtrlEx().FormatRange(NULL, FALSE); // required by RichEdit to clear out cache
	ASSERT_VALID(this);


//	m_nPrintStartChar = 0;
//	m_nPrintStopChar = GetTextLength();

	// Get printable area rectangle (twips)
//	m_rectPrintableArea.SetRect(
//			m_rectMargin.left, 
//			m_rectMargin.top, 
//			m_sizePaper.cx - m_rectMargin.right, 
//			m_sizePaper.cy - m_rectMargin.bottom
//			);

}




// OnPrepareDC
// Called by the framework before the OnDraw member function is called for screen display 
// and before the OnPrint member function is called FOR EACH PAGE during printing or print preview. 
// If the function is called for printing, the default implementation examines the page information 
// stored in the pInfo parameter. If the length of the document has not been specified, 
// OnPrepareDC assumes the document to be one page long and stops the print loop after one 
// page has been printed. The function stops the print loop by setting the m_bContinuePrinting 
// member of the structure to FALSE.
// Override OnPrepareDC to test for the end of the document while it is being printed. 
// When there is no more of the document to be printed, set the m_bContinuePrinting member 
// of the CPrintInfo structure to FALSE. 
void CRichEditView2::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);  // overriding OnPaint -- never get this

	// Try to paginate to the current page
	pDC->SetMapMode(MM_TEXT); // pixels
	if (pInfo->m_nCurPage > (UINT)m_aPageStart.GetSize() && !PaginateTo(pDC, pInfo))
	{
		// Can't paginate to that page, thus cannot print it.
		pInfo->m_bContinuePrinting = FALSE;
	}
	ASSERT_VALID(this);

//	CCtrlView::OnPrepareDC(pDC, pInfo);
}




// Print a page
//` code adapted from viewrich.cpp
void CRichEditView2::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	xTRACE("CRichEditView2::OnPrint\n");

	// Check assumptions
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_bContinuePrinting);

	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage <= (UINT)m_aPageStart.GetSize());
	long nIndex = (long) m_aPageStart[nPage-1];

	// Print as much as possible in the current page.
	// Note: m_nPrintStopChar will be -1 normally, but if user is just printing selected 
	// text will be the last character in the range.
//	nIndex = PrintPage(pDC, nIndex, 0xFFFFFFFF);
	nIndex = PrintPage(pDC, nIndex, m_nPrintStopChar); // print page and get end character for page

	// Check if reached end of document
	if (nIndex >= m_nPrintStopChar)
	{
		TRACE0("End of Document\n");
		pInfo->SetMaxPage(nPage);
	}

	// Update pagination information for page just printed
	if (nPage == (UINT)m_aPageStart.GetSize())
	{
		if (nIndex < m_nPrintStopChar)
			m_aPageStart.Add(nIndex);
	}
	else
	{
		ASSERT(nPage+1 <= (UINT)m_aPageStart.GetSize());
		ASSERT(nIndex == (long)m_aPageStart[nPage+1-1]);
	}

	//` Draw header, footer and margins using printinfomore object
	CPrintInfoMore& rpim = theApp.m_printinfo;
	pDC->SetMapMode(MM_TEXT); // switch to pixel mode for all rpim methods
	pDC->SetViewportOrg(0, 0);
	rpim.PrintHeader(pDC, pInfo);
	rpim.PrintFooter(pDC, pInfo);
	rpim.DrawMargins(pDC, pInfo);

// CRichEditView::OnPrint(pDC, pInfo);
}






// Worker function for laying out text in a rectangle
long CRichEditView2::PrintPage(CDC* pDC, long nIndexStart, long nIndexStop)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	// test
//	HDC hDC = pDC->m_hAttribDC;
//	pDC->m_hAttribDC = pDC->m_hDC;
	
	// Save DC settings
//	int nSavedDC = pDC->SaveDC();
	
//	pDC->SetMapMode(MM_TWIPS);
//	int nMode = pDC->GetMapMode();
	// pDC is in MM_TEXT mode (pixels)


	// Offset by printing offset (pixels)
	// This specifies which device point corresponds to the window point (0, 0)
	pDC->SetViewportOrg(-pDC->GetDeviceCaps(PHYSICALOFFSETX), 
										-pDC->GetDeviceCaps(PHYSICALOFFSETY));

	// This scales from screen pixels to page pixels
	// (if you comment this out, the text prints out very small)
	// The ScaleWindowExtEx function modifies the window for a device context using the ratios 
	// formed by the specified multiplicands and divisors. 
	// This doesn't affect pagination - just the scaling of the output to the preview view.
	// In this case, we're modifying the screen DC window extent.
	// If we didn't do this, 16384 would correspond to 5420
	// When we do this, 8738 corresponds to 5420
	// So it zooms in 
	if (::GetDeviceCaps(pDC->m_hDC, TECHNOLOGY) != DT_METAFILE && pDC->m_hAttribDC != NULL)
	{
//		::ScaleWindowExtEx(pDC->m_hDC,
//			::GetDeviceCaps(pDC->m_hDC, LOGPIXELSX), // horiz num (pixels per inch)
//			::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSX), // horiz denom (pixels per inch)
//			::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), // vert num (pixels per inch)
//			::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSY), // vert denom (pixels per inch)
//			NULL); 
		int nX = ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSX); // eg 96
		int nX2 = ::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSX); // eg 180
		int nY = ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY); // eg 96
		int nY2 = ::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSY); //eg 180
		// ratio = 96 / 180 = 0.5333, so window ext of 16384 maps to 8738
//		nX = nY = 40; // makes font larger
//		nX = nY = 240; // makes font smaller
//		nX = nY = 120; 
		::ScaleWindowExtEx(pDC->m_hDC, nX, nX2, nY, nY2, NULL);

//		CSize sz;
//		CSize sz2;
//		::ScaleWindowExtEx(pDC->m_hDC, nX, nX2, nY, nY2, &sz);
//		::GetWindowExtEx(pDC->m_hDC, &sz2);
//		TRACE(" scale %d %d %d %d, old: %d %d, new: %d %d\n", nX, nX2, nY, nY2, sz.cx, sz.cy, sz2.cx, sz2.cy); // 16384, 16384 to ...		
//		CSize szA, szB, szC, szD;
//		::GetViewportExtEx(pDC->m_hDC, &szA);
//		::GetViewportExtEx(pDC->m_hAttribDC, &szB);
//		::GetWindowExtEx(pDC->m_hDC, &szC);
//		::GetWindowExtEx(pDC->m_hAttribDC, &szC);

		// i don't think this scaling is the problem, because the problem appears to be
		// related to the initial size of the preview window - this seems to scale correctly as the user
		// resizes it, but the font size is still screwy.
		// maximized, maps 0..8738 to 0..5420
		// restored, maps 0..8738 to 0..3558
		// wordpad does the same - seems to be alright
//		CSize sz;
//		::GetWindowExtEx(pDC->m_hDC, &sz);
//		sz.cx = (int) ((float) sz.cx * m_fScale);
//		sz.cy = (int) ((float) sz.cy * m_fScale);
//		::SetWindowExtEx(pDC->m_hDC, sz.cx, sz.cy, NULL);
	}

	// test
//	::SetMapMode(pDC->m_hDC, MM_TEXT); // this negates all the scaling stuff above

	// Remove all scaling and clipping - 
//	::SetWindowExtEx(pDC->m_hDC, 16384, 16384, NULL);
//	::SetViewportExtEx(pDC->m_hDC, 16384, 16384, NULL);
//	::SetWindowOrgEx(pDC->m_hDC, 0, 0, NULL);
//	::SetViewportOrgEx(pDC->m_hDC, 0, 0, NULL);
//	::IntersectClipRect(pDC->m_hDC, 0, 0, 16384, 16384);

//	CClientDC dc(this);

	// Set up FormatRange structure describing what to print and where
	// rc - Contains the area to render to. This member contains the rectangle that the text 
	// is formatted to fit in, and subsequently printed in. It also contains the margins, room 
	// for headers and footers, and so forth. The rc.bottom member may be changed after the 
	// message is sent. If it is changed, it must indicate the largest rectangle that can fit within 
	// the bounds of the original rectangle and still contain the specified text without printing 
	// partial lines. It may be necessary to reset this value after each page is printed. These 
	// dimensions are given in TWIPS. 
	// rcPage - Entire area of rendering device. Units are measured in twips. This area can 
	// be obtained using the GetDeviceCaps() function. These dimensions are given in TWIPS. 
	FORMATRANGE fr;
	fr.hdcTarget = pDC->m_hAttribDC; // printer dc! (target device to format for)
	fr.hdc = pDC->m_hDC; // screen dc! (device to render to)
//	fr.hdc = dc.m_hDC; // screen dc! (device to render to)
	fr.chrg.cpMin = nIndexStart; // start character
	fr.chrg.cpMax = nIndexStop; // stop character
	fr.rcPage = GetPageRect(); // full paper size
	fr.rc = GetPrintRect(); // paper size minus margins and header/footer
//	fr.rc = m_rectPrintableArea; // paper size minus margins and header/footer
//	CRect r;
//	r.SetRect(1400, 1400, 1400*5, 1400*5);
//	fr.rc = r;
//	fr.rcPage = r;

	// Now render the rich edit contents to the device context
	long lRes = GetRichEditCtrlEx().FormatRange(&fr, TRUE);

	// Restore DC settings
//	pDC->RestoreDC(nSavedDC);

//	pDC->m_hAttribDC = hDC; //test

	// test	
//	GetRichEditCtrlEx().FormatRange(NULL, FALSE); // required by RichEdit to clear out cache
//	CDC* pDC2 = m_wnd.GetDC();
//	fr.hdcTarget = pDC->m_hAttribDC; // printer dc! (target device to format for)
//	fr.hdc = pDC2->m_hDC; // screen dc! (device to render to)
//	fr.chrg.cpMin = nIndexStart; // start character
//	fr.chrg.cpMax = nIndexStop; // stop character
//	fr.rcPage = GetPrintRect(); // full paper size
//	fr.rc = GetPrintRect(); // paper size minus margins and header/footer
//	GetRichEditCtrlEx().FormatRange(&fr, TRUE);
//	m_wnd.ReleaseDC(pDC2);
	
	GetRichEditCtrlEx().FormatRange(NULL, FALSE); // required by RichEdit to clear out cache

	return lRes;
}



// Attempts pagination to pInfo->m_nCurPage.
// Returns True if successful.
// Note: This routine doesn't necessarily get called!
BOOL CRichEditView2::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	// Save current settings
	CRect rectSave = pInfo->m_rectDraw;
	UINT nPageSave = pInfo->m_nCurPage;
	ASSERT(nPageSave > 1);
	ASSERT(nPageSave >= (UINT)m_aPageStart.GetSize());
	VERIFY(pDC->SaveDC() != 0);

	pDC->IntersectClipRect(0, 0, 0, 0);
	pInfo->m_nCurPage = m_aPageStart.GetSize();
	while (pInfo->m_nCurPage < nPageSave)
	{
		ASSERT(pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize());
		OnPrepareDC(pDC, pInfo);
		ASSERT(pInfo->m_bContinuePrinting);
		
		// Get printable page area, normally smaller than 8.5x11" (pixels to twips)
		pInfo->m_rectDraw.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&pInfo->m_rectDraw); // device to logical coords (pixels to twips)

		// Try to print the specified page
		OnPrint(pDC, pInfo);

		if (pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize())
			break;
		++pInfo->m_nCurPage;
	}

	// Restore settings
	BOOL bResult = pInfo->m_nCurPage == nPageSave;
	pDC->RestoreDC(-1);
	pInfo->m_nCurPage = nPageSave;
	pInfo->m_rectDraw = rectSave;

	ASSERT_VALID(this);
	return bResult;
}


//` changed these from inline to regular
int CRichEditView2::GetPrintWidth() const
{
	return m_sizePaper.cx - m_rectMargin.left - m_rectMargin.right;
}


CRect CRichEditView2::GetPrintRect() const
{ 
//	return CRect(m_rectMargin.left, m_rectMargin.top, m_sizePaper.cx - m_rectMargin.right, 
//			m_sizePaper.cy - m_rectMargin.bottom); }
	return CRect(
			m_rectMargin.left, 
			m_rectMargin.top + m_nHeaderHeightTwips, 
			m_sizePaper.cx - m_rectMargin.right, 
			m_sizePaper.cy - m_rectMargin.bottom - m_nFooterHeightTwips
//			m_sizePaper.cx - m_rectMargin.right - m_rectMargin.left, 
//			m_sizePaper.cy - (m_rectMargin.top + m_nHeaderHeightTwips) - m_rectMargin.bottom - m_nFooterHeightTwips
			);
}


CRect CRichEditView2::GetPageRect() const
{ 
	return CRect(CPoint(0,0), m_sizePaper); 
}



/*
void CRichEditView2::DrawHeaderFooter(CDC* pDC)
{
	// Draw header
//	pInfo->m_nCurPage; // current page number to print
//	pInfo->m_bPreview; // in print preview?
//	pInfo->m_nNumPreviewPages; // number of print preview pages displayed

	// See if we are print previewing for a black and white printer
//	BOOL bMono = (pDC->GetDeviceCaps(NUMCOLORS) == 2) && (pDC->m_hDC != pDC->m_hAttribDC);
	// Convert rgb colors into shades of gray with this formula:
	// rgb = (red * 0.30) + (green * 0.59) + (blue * 0.11)

	// Print header
//	m_pViewRtf->PrintPage(pDC, nStartChar, nStopChar);
//	m_pViewRtf->OnPrint(pDC, pInfo);

	// Set to twips mode
//	pDC->SetMapMode(MM_TWIPS);

	// Get string to show as "filename" in header/footer 
	LPCTSTR pszFileName = GetDocument()->GetPathName();
	if (pszFileName[0] == 0) 
		pszFileName = GetDocument()->GetTitle();

	// Format the header and footer 
	CString strHeader;
	CString strFooter; 
	strHeader = "";
	strFooter = pszFileName;
//	dlgPageSetup.FormatHeader(strHeader, m_timeHeader, pszFileName, pInfo->m_nCurPage); 
//	dlgPageSetup.FormatFooter(strFooter, m_timeFooter, pszFileName, pInfo->m_nCurPage); 
	
	TEXTMETRIC tm; 
	pDC->GetTextMetrics(&tm);
	int cyChar = tm.tmHeight;
//	CRect rectPage = pInfo->m_rectDraw;
	CRect rPage = GetPrintRect();
	
	// Draw and exclude space for header 
	if (!strHeader.IsEmpty())
	{ 
		// Print header
		pDC->TextOut(rPage.left, rPage.top - m_nHeaderHeightTwips, strHeader);
		rPage.top += cyChar + cyChar / 4; 
		// Draw line under header
//		pDC->MoveTo(rPage.left, rPage.top); pDC->LineTo(rPage.right, rPage.top); 
//		rPage.top += cyChar / 4; 
	} 

	// Set back to text
//	pDC->SetMapMode(MM_TEXT);

	// Allow space for footer 
	pInfo->m_rectDraw = rectPage; 
	if (!strFooter.IsEmpty()) 
		pInfo->m_rectDraw.bottom -= cyChar + cyChar/4 + cyChar/4; 

	// Draw body text 
	CViewEx::OnPrint(pDC, pInfo); 

	// Draw footer 
	if (!strFooter.IsEmpty()) 
	{ 
		rectPage.bottom -= cyChar; 
		pDC->TextOut(rectPage.left, rectPage.bottom, strFooter); 
		rectPage.bottom -= cyChar / 4; 
		pDC->MoveTo(rectPage.left, rectPage.bottom); 
		pDC->LineTo(rectPage.right, rectPage.bottom); 
		rectPage.bottom -= cyChar / 4; 
	} 

}
*/


/*
// Draw margins for print preview page.
// Code adapted from WordPad.
//, this code has been copied to cviewex also
void CRichEditView2::DrawMargins(CDC* pDC, CPrintInfo* pInfo)
{
	if (pDC->m_hAttribDC != NULL)
	{
		CRect& rMargins = theApp.m_rectPageMargins;

		// Get margin positions in twips
		CRect rect;
		rect.left = rMargins.left;
		rect.right = m_sizePaper.cx - rMargins.right;
		rect.top = rMargins.top;
		rect.bottom = m_sizePaper.cy - rMargins.bottom;

		// Get pixels per inch
		int logx = ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSX);
		int logy = ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);

		// Convert margins to pixels
		rect.left = MulDiv(rect.left, logx, 1440);
		rect.right = MulDiv(rect.right, logx, 1440);
		rect.top = MulDiv(rect.top, logy, 1440);
		rect.bottom = MulDiv(rect.bottom, logy, 1440);

		// Create and select pen into device context
//		CPen pen(PS_DOT, 0, pDC->GetTextColor());
		COLORREF color = 0x00d0d0d0; // light gray
		CPen pen(PS_DOT, 0, color);
		CPen* poldpen = pDC->SelectObject(&pen);

		// Draw margins
		pDC->MoveTo(0, rect.top); pDC->LineTo(12000, rect.top);
		pDC->MoveTo(rect.left, 0); pDC->LineTo(rect.left, 12000);
		pDC->MoveTo(0, rect.bottom); pDC->LineTo(12000, rect.bottom);
		pDC->MoveTo(rect.right, 0); pDC->LineTo(rect.right, 12000);

		// Deselect pen
		pDC->SelectObject(poldpen);
	}
}
*/



// OnEndPrinting
// Called by the framework after a document has been printed or previewed. 
void CRichEditView2::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	xTRACE("CRichEditView2::OnEndPrinting\n");
	ASSERT_VALID(this);
	// It is very important to free cached information after the last time you use FormatRange 
	// by specifying NULL in lParam!!!
	GetRichEditCtrlEx().FormatRange(NULL, FALSE); // required by RichEdit to clear out cache
	m_aPageStart.RemoveAll();
	theApp.m_printinfo.Terminate(); //` delete gdi objects

	// test- restore parent
//	if (m_pwndOldParent)
//		SetParent(m_pwndOldParent);
//	m_wnd.DestroyWindow(); //test
}




// note: this is not used in the print routines!!!
// Call this function to format a range of text in a rich edit control to fit within 
// rectLayout for the device specified by pDC. Typically, this call is followed by a call 
// to CRichEditCtrl::DisplayBand which generates the output.
long CRichEditView2::PrintInsideRect(CDC* pDC, RECT& rectLayout, long nIndexStart, long nIndexStop, BOOL bOutput)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	// Adjust DC because richedit doesn't do things like MFC
	if (::GetDeviceCaps(pDC->m_hDC, TECHNOLOGY) != DT_METAFILE && pDC->m_hAttribDC != NULL)
	{
		::ScaleWindowExtEx(pDC->m_hDC,
			::GetDeviceCaps(pDC->m_hDC, LOGPIXELSX),
			::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSX),
			::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY),
			::GetDeviceCaps(pDC->m_hAttribDC, LOGPIXELSY), NULL);
	}

	// Set up FormatRange structure describing what to print and where
	FORMATRANGE fr;
	fr.hdcTarget = pDC->m_hAttribDC;
	fr.hdc = pDC->m_hDC;
	// convert rect to twips
	fr.rcPage = rectLayout;
	fr.rc = rectLayout;
	fr.chrg.cpMin = nIndexStart;
	fr.chrg.cpMax = nIndexStop;

	// Note: If bOutput is FALSE, we only measure the text (ie it's not rendered to the DC).
	GetRichEditCtrlEx().FormatRange(NULL, FALSE); // required by RichEdit to clear out cache
	long lres = GetRichEditCtrlEx().FormatRange(&fr, bOutput);
	GetRichEditCtrlEx().FormatRange(NULL, FALSE); // required by RichEdit to clear out cache

	rectLayout = fr.rc;
	return lres;
}







//-----------------------------------------------------------------------------------------------------------------



//` added this to be called from cviewrtf on user hitting F3
BOOL CRichEditView2::FindNext()
{
	_AFX_RICHEDIT_STATE* pState = _afxRichEditState;
//`	CRichEditCtrlEx* prtf = (CRichEditCtrlEx*) &GetRichEditCtrlEx();
	CRichEditCtrlEx* prtf = &GetRichEditCtrlEx();
	long nPos = prtf->FindNext(pState->strFind, pState->bCase, pState->bWord);
	if (nPos != -1)
	{
		// Found text!
		return TRUE;
	}
	else
	{
		// No more matches found, need to let viewsearch know!
		AfxMessageBox(_T("Reached end of document."), MB_ICONINFORMATION);
		return FALSE;
	}
}



//` added this kludge code so f3 will work when you enter text view from search view
void CRichEditView2::SetFindText(CString &str, BOOL bMatchCase, BOOL bWholeWord)
{
	_AFX_RICHEDIT_STATE* pState = _afxRichEditState;
	pState->strFind = str;
	pState->bCase = bMatchCase;
	pState->bWord = bWholeWord;
}


BOOL CRichEditView2::DestroyWindow() 
{
	xTRACE("CRichEditView2::DestroyWindow()\n");
	
	return CCtrlView::DestroyWindow();
}








//-----------------------------------------------------------------------------------------------------------------


// Show popup menu
void CRichEditView2::OnRButtonDown(UINT nFlags, CPoint point) 
{
	xTRACE("CRichEditView2::OnRButtonDown(nFlags=%d, point.x=%d, point.y=%d)\n", nFlags, point.x, point.y);

	// Get current selection
	CHARRANGE cr;
	WORD seltyp = SEL_EMPTY;
	GetRichEditCtrlEx().GetSel(cr);
	if (cr.cpMax != cr.cpMin)
		seltyp = SEL_TEXT;

	// Get char pos of mouse point
	WPARAM wParam = 0;
	POINTL p;
	p.x = point.x;
	p.y = point.y;
	LPARAM lParam = (LPARAM) &p;
	LRESULT r = GetRichEditCtrlEx().SendMessage(EM_CHARFROMPOS, wParam, lParam);
	long nChar = (long) r;

	// If text is selected but you click outside of that selection, cancel the selection.
	// ie set the cursor to the mouse click (this is how Word does it).
	if (seltyp == SEL_TEXT) 
	{
		if ((nChar < cr.cpMin) || (nChar > cr.cpMax))
		{
			GetRichEditCtrlEx().SetSel(nChar, nChar);
		}
	}

	// If no text is selected, set the cursor to the mouse click.
	if (seltyp == SEL_EMPTY)
	{
		GetRichEditCtrlEx().SetSel(nChar, nChar);
	}

	// Convert point to screen coords		
	ClientToScreen(&point);

	// Now display the appropriate popup menu
	ShowPopup(point, seltyp);

//	CRichEditView::OnRButtonDown(nFlags, point); // can't access protected member (private msg handler)
}



// This is an Advanced Overridable
// just ignore this thing since we're overriding the rbuttondown and oncontext menu events anyway - 
// this only allows hmenus and we want to use bcmenus!
// see OnContextMenu
HMENU CRichEditView2::GetContextMenu(WORD seltyp, LPOLEOBJECT lpoleobj, CHARRANGE* lpchrg)
{
	xTRACE("CRichEditView2::GetContextMenu seltyp %d\n", seltyp);
//	return CRichEditView::GetContextMenu(seltyp, lpoleobj, lpchrg); // just returns NULL
	return NULL;
}



// Handle context menu key ("Application Key").
// Note: This does not get called on right button click! See OnRButtonDown
void CRichEditView2::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	xTRACE("CRichEditView2::OnContextMenu(point.x=%d, point.y=%d)\n", point.x, point.y);

	// Get current selection
	long nStart, nStop;
	GetRichEditCtrlEx().GetSel(nStart, nStop);
	WORD seltyp = SEL_EMPTY;
	if (nStart != nStop)
		seltyp = SEL_TEXT;

	// If user pressed context menu key ("Application Key") x and y will be -1
	// Use point under beginning of selection if visible, or upper left corner of window if not
	// (that's how Word does it)
	if (point.x == -1 && point.y == -1)
	{
		// Get position of current selection in client coords
		point = GetRichEditCtrlEx().GetCharPos(nStart);

		// Shift it down by one line
		int nLineHeight = 20;
		point.y += nLineHeight;

		// If it's out of bounds of window, just use upper left corner
		if (point.x < 0 || point.y < 0)
		{
			CRect r;
			GetClientRect(&r);
			point.x = r.left + 10;
			point.y = r.top + 10;
		}
		
		// Convert to screen coords		
		ClientToScreen(&point);
		xTRACE("  convert to %d %d\n", point.x, point.y);
	}

	// Show popup menu
	// Rather than give control to the rich edit control at this point, which would ignore the new screen
	// position, handle the popup menu ourselves, getting the menu from the GetContextMenu method.
	// Also need to position the cursor to where the mouse click occurred.
	ShowPopup(point, seltyp);

//	CRichEditView::OnContextMenu(pWnd, point);
}





void CRichEditView2::OnEditOpenHyperlink() 
{
	// Get hyperlink text then open it with Windows
	// need to 

}





// Show the rtf popup menu at specified position.
void CRichEditView2::ShowPopup(CPoint point, WORD seltyp)
{
//	HMENU hMenu = GetContextMenu(seltyp, NULL, &cr);
//	CView* pView = (CView*) GetParent();
//	::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, NULL, pView->m_hWnd, NULL);

/*
	// See if on a hyperlink
	// Note: link style isn't set if sel length is 0, but if 1, then it is set!
	// So need to select next char, get format, then remove selection.
	CHARFORMAT2 cf2;
	if (seltyp == SEL_EMPTY)
	{
		CHARRANGE crSave;
		CHARRANGE crTemp;
		GetRichEditCtrlEx().GetSel(crSave);
		crTemp = crSave;
		crTemp.cpMax = crTemp.cpMin + 1;
		GetRichEditCtrlEx().SetSel(crTemp);
		cf2 = GetCharFormatSelection2();
		GetRichEditCtrlEx().SetSel(crSave);
	}
	else
	{
		cf2 = GetCharFormatSelection2();
	}
	BOOL bOnHyperlink = cf2.dwEffects & CFE_LINK;

	if (bOnHyperlink)
	{
		// Show hyperlink menu
//		if (menu.LoadMenu(IDR_POPUP_HYPERLINK))
//		{
//			CMenu* pPopup = menu.GetSubMenu(0);
//			menu.SetDefaultItem(ID_EDIT_OPEN_HYPERLINK);
//			hMenu = pPopup->Detach();
//			menu.Detach(); // so that destructor will not destroy main window
//		}
		return NULL;
	}
	else
	{
*/
	
	BCMenu menu;
	menu.LoadMenu(IDR_POPUP_RTF);
	menu.LoadToolbar(IDR_MAINFRAME);
	menu.LoadToolbar(IDR_TOOLBAR_RTF);
	menu.LoadToolbar(IDR_TOOLBAR_OTHER);
	// load any other toolbars here for more images
	BCMenu* pPopup = (BCMenu*) menu.GetSubMenu(0);
	if (pPopup)
	{			
		// Enable/disable menu commands
		if (seltyp == SEL_EMPTY)
		{
			pPopup->EnableMenuItem(ID_EDIT_CUT, MF_BYCOMMAND | MF_GRAYED);
//			pPopup->EnableMenuItem(ID_EDIT_SPIKE, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_COPY, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_CLEAR, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_CLEAR_FORMAT, MF_BYCOMMAND | MF_GRAYED);
			pPopup->EnableMenuItem(ID_EDIT_MOVE_TO, MF_BYCOMMAND | MF_GRAYED);
//			pPopup->EnableMenuItem(ID_EDIT_SHIFT_LEFT, MF_BYCOMMAND | MF_GRAYED);
//			pPopup->EnableMenuItem(ID_EDIT_SHIFT_RIGHT, MF_BYCOMMAND | MF_GRAYED);
//			pPopup->EnableMenuItem(ID_EDIT_CREATE_SUBITEM, MF_BYCOMMAND | MF_GRAYED);
//			pPopup->EnableMenuItem(ID_EDIT_SORT_SELECTED_TEXT, MF_BYCOMMAND | MF_GRAYED);
		}
		BOOL bUndo = GetRichEditCtrlEx().CanUndo();
		BOOL bRedo = GetRichEditCtrlEx().CanRedo();
		pPopup->EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND | (bUndo ? 0 : MF_GRAYED));
		pPopup->EnableMenuItem(ID_EDIT_REDO, MF_BYCOMMAND | (bRedo ? 0 : MF_GRAYED));

		// Now display popup menu - command messages will go to pView (CViewRtf)
		CView* pView = (CView*) GetParent();
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pView);
	}

}





// Selection has changed.
// The EN_SELCHANGE message notifies a rich edit control's parent window that the current 
// selection has changed. A rich edit control sends this notification message in the form of 
// a WM_NOTIFY message.
void CRichEditView2::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == EN_SELCHANGE);
	UNUSED(pNMHDR); // not used in release builds

/*
	static BOOL bByCode = FALSE;
	// Skip this if it was triggered by the SetSel below (otherwise would recurse)
	if (bByCode == FALSE)
	{
		SELCHANGE* psc = (SELCHANGE*) pNMHDR;
		if (psc->seltyp && SEL_TEXT)
		{
			//, need to trap when user selects with Shift+Down Arrow, and go backwards if
			// at end of paragraph
			// If user hits Shift+Down, this gets fired AFTER the text has been already highlighted.
			// get selected text
			// this might work, BUT if you're trying to select lots of text, it would probably slow things down a lot.
			// better if we could change the rtf behavior somehow?
			// what you need to trap is:
			// starting at beginning of a line, hit shift+down, AND next line is blank.
			// so maybe trap shift+down?
			// then if next line is blank could reduce selection by one line
//`			CRichEditCtrlEx* prtf = (CRichEditCtrlEx*) &GetRichEditCtrlEx();

			// 1.2b bug: GetSelText will BOMB if you have "too much" text in the control!
			// it fails with a Stack Overflow. so commented this, and this whole section 
			// out, which wasn't being used anyway.
//			CRichEditCtrlEx* prtf = &GetRichEditCtrlEx();
//			CString strText = prtf->GetSelText();

	//		CString strRtf = prtf->GetRtf(TRUE);
			// see if ends with (something)(cr)
//			CString s = strText.Right(2);
//			psc->chrg.cpMax--;
//			bByCode = TRUE;
//			prtf->SetSel(psc->chrg);
//			bByCode = FALSE;
		}
	}
*/

	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;
	*pResult = 0;
}



void CRichEditView2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Shift+F10 (= Context Menu)
	if (nChar == VK_F10 && GetKeyState(VK_SHIFT) < 0)
	{
		CRect rect;
		GetClientRect(rect);
		CPoint pt = rect.CenterPoint();
		SendMessage(WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM(pt.x, pt.y));
	}

/*
	// Shift+Down
	// this works but causes flicker in taskbar!
	else if(nChar == VK_DOWN && GetKeyState(VK_SHIFT) < 0)
	{
		// Trap Shift+Down because of stupid richedit control - old version used to let you select lines
		// one at a time but new version has weird behavior that I'm trying to get around. 

		// only do this if running richedit 2.0 or 3.0?? just 3.0??
		// i think since we specify the classname as riched20a or whatever that we'll always be using 2.0 or 3.0
		// is there a difference between 2.0 and 3.0? i'm using 5.0
		
		//. have option to turn this on/off?

		// Get current position
		// The EM_LINEINDEX message retrieves the character index of the first character of a 
		// specified line in a multiline edit control. A wParam value of –1 specifies the current line 
		// number (the line that contains the caret). lParam not used.
		long nStartChar, nEndChar;
//`		CRichEditCtrlEx* prtf = (CRichEditCtrlEx*) &GetRichEditCtrlEx();
		CRichEditCtrlEx* prtf = &GetRichEditCtrlEx();
		prtf->GetSel(nStartChar, nEndChar); // gets current selection
		BOOL bNoSelection = (nStartChar == nEndChar);
		long nFirstCharInLine = prtf->SendMessage(EM_LINEINDEX, (WPARAM) -1, 0);
		BOOL bAtStartOfLine = (nStartChar == nFirstCharInLine);

		// If no selection AND at start of line, replace shift+down with shift+end
		if (bNoSelection && bAtStartOfLine)
		{
			// Do Shift+End
			SendMessage(WM_KEYDOWN, (WPARAM) VK_END, nFlags);
			SendMessage(WM_KEYUP, (WPARAM) VK_END, nFlags);
		}
		else if (!bNoSelection)
		{
			// If already have selection then do shift+down then shift+end 
			CCtrlView::OnKeyDown(nChar, nRepCnt, nFlags); // shift+down
			SendMessage(WM_KEYDOWN, (WPARAM) VK_END, nFlags);
			SendMessage(WM_KEYUP, (WPARAM) VK_END, nFlags);

		}
		else
		{
			CCtrlView::OnKeyDown(nChar, nRepCnt, nFlags);
		}

	}

	// Shift+Up
	else if(nChar == VK_UP && GetKeyState(VK_SHIFT) < 0)
	{
		//. get this working

		// Get current position
		long nStartChar, nEndChar;
//`		CRichEditCtrlEx* prtf = (CRichEditCtrlEx*) &GetRichEditCtrlEx();
		CRichEditCtrlEx* prtf = &GetRichEditCtrlEx();
		prtf->GetSel(nStartChar, nEndChar); // gets current selection
		BOOL bNoSelection = (nStartChar == nEndChar);
		long nFirstCharInLine = prtf->SendMessage(EM_LINEINDEX, (WPARAM) -1, 0);
		BOOL bAtStartOfLine = (nStartChar == nFirstCharInLine);

		// current line blank?
		long nLine = prtf->LineFromChar(nEndChar); // get current line
		const nChars = 1024;
		TCHAR szLine[nChars];
		prtf->GetLine(nLine, szLine, nChars);
		BOOL bCurrentBlank = (szLine[0] == 0x0d);

		// prev line blank?
		nLine--; // get prev line
		prtf->GetLine(nLine, szLine, nChars);
		BOOL bPrevBlank = (szLine[0] == 0x0d);

		// if curline blank and prevline nonblank, replace shift+up with shift+left
		if (bCurrentBlank && !bPrevBlank)
		{
			SendMessage(WM_KEYDOWN, (WPARAM) VK_LEFT, nFlags);
			SendMessage(WM_KEYUP, (WPARAM) VK_LEFT, nFlags);
		}
		else
		{
			CCtrlView::OnKeyDown(nChar, nRepCnt, nFlags);
		}
	}
*/		
	else
		CCtrlView::OnKeyDown(nChar, nRepCnt, nFlags);
}





