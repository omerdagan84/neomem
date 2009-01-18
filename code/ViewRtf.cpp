
// CViewRtf



#include "precompiled.h"

#include "Constants.h"
#include "DialogInsertLink.h"
#include "Hint.h"
#include "NeoMem.h"
#include "RichEditCtrlEx.h"
#include "RichEditView2.h"
#include "ToolBarRtf.h"
#include "ViewRtf.h"
//#include "DialogEditLink.h"
#include "DialogGetLocation.h"

#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_RTF 93182 // random #


// not in vs6 include files (richedit3 messages)
#define EM_GETZOOM (WM_USER + 224)
#define EM_SETZOOM (WM_USER + 225)


// Macros
//--------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CViewRtf, CViewEx)

BEGIN_MESSAGE_MAP(CViewRtf, CViewEx)
	//{{AFX_MSG_MAP(CViewRtf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
//	ON_UPDATE_COMMAND_UI(ID_EDIT_SPIKE, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_PLAIN, OnUpdateNeedClip)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_FORMATTED, OnUpdateNeedClip)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_NEXT, OnUpdateNeedFind)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_BOLD, OnUpdateFormatBold)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ITALIC, OnUpdateFormatItalic)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_UNDERLINE, OnUpdateFormatUnderline)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_STRIKETHROUGH, OnUpdateFormatStrikethrough)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_LEFT, OnUpdateFormatLeft)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_CENTER, OnUpdateFormatCenter)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_RIGHT, OnUpdateFormatRight)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_BULLET, OnUpdateFormatBullet)
	ON_UPDATE_COMMAND_UI(ID_FONT_NAME, OnUpdateFormatFont) //. change id?
	ON_UPDATE_COMMAND_UI(ID_FONT_SIZE, OnUpdateFormatSize) //. change id?
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_CUT, OnCmdEditCut)
//	ON_COMMAND(ID_EDIT_SPIKE, OnCmdEditSpike)
	ON_COMMAND(ID_EDIT_COPY, OnCmdEditCopy)
	ON_COMMAND(ID_EDIT_PASTE_PLAIN, OnCmdEditPastePlain)
	ON_COMMAND(ID_EDIT_PASTE_FORMATTED, OnCmdEditPasteFormatted)
	ON_COMMAND(ID_EDIT_UNDO, OnCmdEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnCmdEditRedo)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_INSERT_LINE, OnCmdEditInsertLine)
	ON_COMMAND(ID_EDIT_DELETE_LINE, OnCmdEditDeleteLine)
	ON_COMMAND(ID_EDIT_FIND_NEXT, OnCmdEditFindNext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_NEXT, OnUpdateEditFindNext)
	ON_COMMAND(ID_EDIT_FIND, OnCmdEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnCmdEditReplace)
	ON_COMMAND(ID_EDIT_SHIFT_LEFT, OnCmdEditShiftLeft)
	ON_COMMAND(ID_EDIT_SHIFT_RIGHT, OnCmdEditShiftRight)
	ON_WM_KILLFOCUS()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnCmdEditSelectAll)
	ON_COMMAND(ID_EDIT_CLEAR, OnCmdEditClear)
	ON_COMMAND(ID_TEST, OnTest)
	ON_COMMAND(ID_EDIT_INSERT_OBJECT, OnCmdEditInsertObject)
	ON_COMMAND(ID_FORMAT_BACKCOLOR, OnFormatBackcolor)
	ON_COMMAND(ID_FORMAT_FORECOLOR, OnFormatForecolor)
	ON_COMMAND(ID_EDIT_INSERT_LINK, OnEditInsertLink)
	ON_COMMAND(ID_EDIT_CLEAR_FORMAT, OnEditClearFormat)
	ON_COMMAND(ID_EDIT_MOVE_TO, OnEditMoveTo)
	ON_COMMAND(ID_EDIT_SORT_SELECTED_TEXT, OnEditSortSelectedText)
	ON_COMMAND(ID_EDIT_INSERT_SYMBOL, OnEditInsertSymbol)
	ON_COMMAND(ID_FORMAT_FONT, OnFormatFont)
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_COMMAND(ID_EDIT_INSERT_DATE, OnEditInsertDate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_FORMAT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVE_TO, OnUpdateNeedSel)	
	ON_UPDATE_COMMAND_UI(ID_EDIT_SORT_SELECTED_TEXT, OnUpdateNeedSel)	
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, OnUpdateViewZoom)
	//}}AFX_MSG_MAP
// 	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_CLEAR, ID_EDIT_REDO, OnUpdateEdit) // See defs in Afxres.h for order!
    ON_MESSAGE(CBN_CLICK, OnColorButtonClick)
	ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
	ON_COMMAND(ID_FORMAT_APPLY_BACKCOLOR, OnFormatApplyBackcolor)
	ON_COMMAND(ID_FORMAT_APPLY_FORECOLOR, OnFormatApplyForecolor)
//	ON_NOTIFY(TBN_DROPDOWN, IDR_TOOLBAR_RTF, OnToolbarDropDown)
	ON_NOTIFY(EN_LINK, ID_RTF, OnRtfLink)
	ON_COMMAND(ID_FORMAT_BOLD, OnFormatBold)
	ON_COMMAND(ID_FORMAT_ITALIC, OnFormatItalic)
	ON_COMMAND(ID_FORMAT_UNDERLINE, OnFormatUnderline)
	ON_COMMAND(ID_FORMAT_STRIKETHROUGH, OnFormatStrikethrough)
	ON_COMMAND(ID_FORMAT_LEFT, OnFormatLeft)
	ON_COMMAND(ID_FORMAT_CENTER, OnFormatCenter)
	ON_COMMAND(ID_FORMAT_RIGHT, OnFormatRight)
	ON_COMMAND(ID_FORMAT_BULLET, OnFormatBullet)
	// Standard printing commands
//	ON_COMMAND(ID_FILE_PRINT, CViewEx::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CViewEx::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CViewEx::OnFilePrintPreview)
	// Standard printing commands
//	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
END_MESSAGE_MAP()



// Construction/Destruction
//--------------------------------------------------------------------------------------------------

CViewRtf::CViewRtf()
{
	xTRACE("CViewRtf Constructor\n");
	m_pViewRtf = 0;
	m_prtf = 0;
	m_bMatchCase = FALSE;
	m_bWholeWord = FALSE;
//	m_nHyperlinks = 0;
	m_chrgMouseDown.cpMin = -1;
	m_chrgMouseDown.cpMax = -1;
	m_bZoom = FALSE;
}

CViewRtf::~CViewRtf()
{
	xTRACE("cviewrtf destructor\n");
//	m_wndMini.DestroyWindow(); // delete
}



int 
CViewRtf::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	xTRACE("CViewRtf OnCreate - create toolbar and rtf child view\n");
	
	// Putting this in constructor doesn't work cause g_ haven't been initialized yet!
//	m_clrForecolor = g_clrWindowText;
//	m_clrBackcolor = g_clrWindow;

	
	// Call base class
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create rtf view as child window
	//. maybe would be better to create the view with Create, then call some function to add it
	// to the document's list of views. it would be clearer code, anyway
	//... i think we can skip the richeditview thing and just create a control here
	// that way can use v2.0 also
	//, is it okay to subclass like this?? (using cast?)
	CRect r;
	GetClientRect(r);
//	m_pViewRtf = (CRichEditView2*) CreateChildView(this, RUNTIME_CLASS(CRichEditView2), m_pDoc, r, 0);
//	m_pViewRtf = (CRichEditView2*) CreateChildView(this, RUNTIME_CLASS(CRichEditView2), m_pDoc, r, ID_RTF);
//	m_pViewRtf = dynamic_cast<CRichEditView2*> (CViewEx::CreateChildView(this, RUNTIME_CLASS(CRichEditView2), m_pDoc, r, ID_RTF));
	m_pViewRtf = dynamic_cast<CRichEditView2*> (CreateChildView(RUNTIME_CLASS(CRichEditView2), m_pDoc, r, ID_RTF));
	if (m_pViewRtf) 
	{
		// Subclass the rtf control with our extended class (provides GetRtf and SetRtf methods)
		//. Okay to cast if don't have any extra variables in it?????
		// NO!! not ok to cast down the class chain!!??
//		m_prtf = (CRichEditCtrlEx*) &(m_pViewRtf->GetRichEditCtrl());
//		m_prtf = DYNAMIC_DOWNCAST(CRichEditCtrlEx, &(m_pViewRtf->GetRichEditCtrl())); // zero if invalid
//		m_prtf = STATIC_DOWNCAST(CRichEditCtrlEx, &(m_pViewRtf->GetRichEditCtrl())); // asserts if invalid
//		m_prtf = dynamic_cast<CRichEditCtrlEx*> (&(m_pViewRtf->GetRichEditCtrl())); 
		m_prtf = &(m_pViewRtf->GetRichEditCtrlEx());  //. now the cast is inside this call.... why is it valid though?
		ASSERT_VALID(m_prtf);

		// Tell CViewEx about child view
		SetChildView(m_pViewRtf);

		// Set the default font in the RTF control
		SetDefaultFont(theApp.m_fontRtfDefault);

		// Set default color
		SetDefaultColor();

		// Turn on url autodetection
		m_prtf->AutoURLDetect(TRUE);

	}
	else return -1;

	// Create Toolbar (CToolBarRtf object)
	if (m_tbrRtf.Create(0, CRect(0,0,0,0), this, IDR_TOOLBAR_RTF))
	{
		// Set dropdown buttons
		m_tbrRtf.SetButtonDropDown(ID_FORMAT_FORECOLOR);
		m_tbrRtf.SetButtonDropDown(ID_FORMAT_BACKCOLOR);

		// Must call this to initialize the toolbar position correctly
		m_tbrRtf.Position();

		// Tell the view that there is a toolbar attached to it
		SetToolBar(&m_tbrRtf);
	}

	// Load custom accelerators for this view
	LoadViewAccelerators(IDR_VIEW_RTF);

	return 0;
}




// Diagnostics
//--------------------------------------------------------------------------------------------------

#ifdef _DEBUG
void 
CViewRtf::AssertValid() const
{
	CViewEx::AssertValid();
}

void 
CViewRtf::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG




// Drawing
//--------------------------------------------------------------------------------------------------

void 
CViewRtf::OnDraw(CDC* pDC) 
{
	CViewEx::OnDraw(pDC);
}


BOOL 
CViewRtf::OnEraseBkgnd(CDC* pDC) 
{
	return CViewEx::OnEraseBkgnd(pDC);
}


// Printing
//---------------------------------------------------------------------------


BOOL 
CViewRtf::DoPrint(BOOL bPreview)
{
	xTRACE("CViewRtf::DoPrint - send message to m_pViewRtf\n");
	if (bPreview)
		m_pViewRtf->SendMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW, 0);
	else
		m_pViewRtf->SendMessage(WM_COMMAND, ID_FILE_PRINT, 0);
	// Can't use this view because all the print code is in CRichEditViewEx!!
//	if (bPreview)
//		CViewEx::OnFilePrintPreview(); // in cviewex
//	else
//		CViewEx::OnFilePrint(); // in file viewprnt.cpp
	return TRUE;
}


// we could send a message to this window,
// as in ID_FILE_PRINT_PREVIEW, and have the handler call OnFilePrint or OnFilePrintPreview,
// which could pass the message on to the child view
/*
void 
CViewRtf::OnFilePrint()
{
	xTRACE("CViewRtf OnFilePrint - send message to m_pViewRtf\n");
	m_pViewRtf->SendMessage(WM_COMMAND, ID_FILE_PRINT, 0);
}

// This gets called from CFrameMain ID_FILE_PRINT_PREVIEW handler
void 
CViewRtf::OnFilePrintPreview()
{
	xTRACE("CViewRtf OnFilePrintPreview - send message to m_pViewRtf\n");
	// Hand to CRichEditView
	// Note: Can't call OnFilePrintPreview directly since it's protected
	// Therefore, we made our own function
//	m_pViewRtf->PrintPreview();
	m_pViewRtf->SendMessage(WM_COMMAND, ID_FILE_PRINT_PREVIEW, 0);
}
*/


BOOL 
CViewRtf::OnPreparePrinting(CPrintInfo* pInfo)
{
	xTRACE("CViewRtf OnPreparePrinting\n");
	// default preparation
	// If you don't call SetMaxPage the framework assumes the document is only one page long
//	pInfo->SetMaxPage(10);
	return DoPreparePrinting(pInfo);
}

// Override to allocate fonts and other resources required for printing
void 
CViewRtf::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	xTRACE("CViewRtf OnBeginPrinting\n");
	// TODO: add extra initialization before printing
//	m_pViewRtf->OnBeginPrinting(pDC, pInfo);
}


// Called once for each page of the printed document.
// Also called before screen repaints.
// Perform print-time pagination by overriding OnPrepareDC and setting pInfo->m_bContinuePrinting
// to TRUE or FALSE each time it's called. FALSE terminates the print job.
void 
CViewRtf::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	xTRACE("CViewRtf OnPrepareDC\n");
	CViewEx::OnPrepareDC(pDC, pInfo);
//	if (pDC->IsPrinting())
//	{
//		pInfo->m_nCurPage; // current page number
//	}
}


// Called before each page is printed. Override to print headers, footers, and other page 
// elements that aren't drawn by OnDraw or to print without relying on OnDraw.
// Note: This doesn't get called because we let CRichEditView2 handle printing.
void 
CViewRtf::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	xTRACE("CViewRtf OnPrint\n");

	// Call base class (verifies DC and calls OnDraw)
//	CViewEx::OnPrint(pDC, pInfo);

	// don't need to call OnDraw
	ASSERT_VALID(pDC);
}




// Deallocate resources allocated in OnBeginPrinting
void 
CViewRtf::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	xTRACE("CViewRtf OnEndPrinting\n");
	// TODO: add cleanup after printing
//	m_pViewRtf->OnEndPrinting(pDC, pInfo);
}





// Message Handlers
//--------------------------------------------------------------------------------------------------


void 
CViewRtf::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	TRACE("    CViewRtf::OnUpdate %s\n", theApp.GetHintName(lHint));

	switch (lHint)
	{
//		case hintAdd:				// Added an object
//		case hintDelete:			// Deleted an object
//		case hintRename:			// Renamed an object
//		case hintMove:				// Moved an object
//		case hintLoadTree:	// Reload the entire file
//		case hintSelect:	// Object was selected on the left
		
		case hintLoad:
			{
				// Display contents of BObject's .RtfText property
				BObject* pobj = (BObject*) pHint;
				ASSERT_VALID(pobj);

				// Turn off drawing to prevent flickering during loading and setting cursor position
				m_prtf->SetRedraw(FALSE);

//				CString strRtf = pobj->GetPropertyText(propRtfText);
//				if (strRtf.GetLength() > 0)
//					m_prtf->SetRtf(strRtf, FALSE);
//				else
//					m_prtf->SetWindowText("");


				// Get rtf text
				LPCTSTR pszRtf = pobj->GetPropertyText(propRtfText);
				m_prtf->SetRtf(pszRtf, FALSE);

/*
//. test!!!!!!! trying to track down weird insertion of blank lines!!
if (strlen(pszRtf) > 0)
{
//	CString strRtf = m_prtf->GetRtf(FALSE);
//	ASSERT(strRtf.Compare(pszRtf) == 0);
	CString str1;
	CString str2;
	m_prtf->GetWindowText(str1);
	theApp.ConvertRtfToPlain(pszRtf, str2);
	if (str1.Compare(str2) != 0)
	{
		ASSERT(0);
	}
}
*/

				// Set the default font if it's blank
				// Bug: This wasn't returning 0 even for new items because of blanks that got in it in SetRtf!!
				//    Caused screwy behaviour with default fonts - took forever to track down!!!!
				if (m_prtf->GetWindowTextLength() == 0)
					SetDefaultFont(theApp.m_fontRtfDefault);

				// Set the default color (black)
				SetDefaultColor();

				// Load view: Set first visible line
				int nTopLineChar = (int) pobj->GetPropertyLong(propRtfTopLineChar);
				int nTopLine = m_prtf->LineFromChar(nTopLineChar);
				int nCurrentTopLine = m_prtf->GetFirstVisibleLine(); // zero-based index of top line
				int nLinesToScroll = nTopLine - nCurrentTopLine; // calc number of lines to scroll up or down
				m_prtf->LineScroll(nLinesToScroll, 0); // scroll
//				nCurrentTopLine = m_prtf->GetFirstVisibleLine(); // see where we are now

				// Load view: Get insertion point
				//. maybe lands on invalid character so fails sometimes? 
				//. it's not because nchar is too large - that just puts it at the end of the text
				// so nchar must be zero... 
				//. we'll try putting it down here, AFTER setting top line. maybe that will fix things... 2004-04-13
				long nStartChar = (long) pobj->GetPropertyLong(propRtfInsertionPoint);
				m_prtf->SetSel(nStartChar, nStartChar);  // returns void
				TRACE("    get pos %u\n", nStartChar);

				// Clear the control's modified flag
				//, doesn't setrtf do this?
//				rRtf.SetModify(FALSE);
				m_prtf->SetModify(FALSE);

				// Zoom if flag is on
				SetZoom();

				// Turn on drawing again
				m_prtf->SetRedraw(TRUE);

				break;
			}

		case hintSave:			
			{
				// Save changes made to rtf, if any, and position of caret 

				BObject* pobj = (BObject*) pHint;
				ASSERT_VALID(pobj);

				// Save insertion point
				// Note: We pass FALSE to SetProperty so that the document's modified flag is not set.
				// This is so that if the user just clicks around, the document won't think it's been modified.
				long nStartChar;
				long nEndChar;
				m_prtf->GetSel(nStartChar, nEndChar);
				pobj->SetPropertyLong(propRtfInsertionPoint, nStartChar, FALSE); // just save start char
				TRACE("    save pos %u\n", nStartChar);

				// Save top line position also
				int nTopLine = m_prtf->GetFirstVisibleLine(); // zero-based index of top line
				int nTopLineChar = m_prtf->LineIndex(nTopLine);
				pobj->SetPropertyLong(propRtfTopLineChar, nTopLineChar, FALSE);

				// If text was modified, save it
				//, make sure this only copies string once - use lpctstr instead?
				if (m_prtf->GetModify())
				{
					SaveOleObjects();
					CString strRtf = m_prtf->GetRtf(FALSE);
					pobj->SetPropertyText(propRtfText, strRtf, TRUE, FALSE);
				}

				// Clear the control's modified flag
				m_prtf->SetModify(FALSE);

				break;
			}

		case hintDefaultFontChanged:
			{
				// Update the rtf control's default font if it's blank
				if (m_prtf->GetWindowTextLength() == 0)
					SetDefaultFont(theApp.m_fontRtfDefault);
				break;
			}

		case hintSelect:
			{
				// Bug: This was a pain in the ass - the rtf control wasn't refreshing in some instances, finally
				// got it to redraw using SetRect, which according to the documentation, will force
				// a redraw of the text. 
				xTRACE("CViewRtf::OnUpdate hintSelect - redraw window\n");
				AdjustWindow();
				break;
			}

		case hintPropertyChange:
			{
				// Refresh display if text property changed! (eg if user drag dropped text onto this object)
				CHint* ph = (CHint*) pHint;
				ASSERT_VALID(ph);
				BObject* pobj = ph->m_pobjObject;
				ASSERT_VALID(pobj);
				BObject* pobjCurrent = m_pDoc->GetCurrentObject();
				// Check if this event refers to the current object and rtf text property
				if ((ph->m_lngPropertyID == propRtfText) && (pobj == pobjCurrent))
				{
					LPCTSTR pszRtf = pobj->GetPropertyText(propRtfText);
					m_prtf->SetRtf(pszRtf, FALSE);

					// Set the default font if it's blank
					if (m_prtf->GetWindowTextLength() == 0)
						SetDefaultFont(theApp.m_fontRtfDefault);
				}
				break;
			}

//		default:
			// Call base class (invalidates entire client area)
//			CRichEditView::OnUpdate(pSender, lHint, pHint);
	}	
}




void 
CViewRtf::OnSize(UINT nType, int cx, int cy) 
{
	xTRACE("CViewRtf OnSize\n"); 

	// This will position the toolbar and child view correctly
	CViewEx::OnSize(nType, cx, cy);

	// nType == SIZE_RESTORED (0)

	// Now also make sure rtf edit area fills its window
	AdjustWindow();
}



void 
CViewRtf::OnCmdEditCut() 
{
	ASSERT_VALID(m_prtf);
	m_prtf->Cut();
}

void 
CViewRtf::OnCmdEditSpike() 
{
	ASSERT_VALID(m_prtf);
	m_prtf->Spike();
}


void 
CViewRtf::OnCmdEditCopy() 
{
	ASSERT_VALID(m_prtf);
	m_prtf->Copy();
}


void 
CViewRtf::OnCmdEditPasteFormatted() 
{
	ASSERT_VALID(m_prtf);
	
	// Paste the data from the clipboard
//	m_prtf->Paste();
//	m_prtf->PasteSpecial(theApp.m_cfRtf);
//	m_prtf->PasteSpecial(CF_TEXT);

	// this didn't work
//	UINT cfRtfWithoutObjects = ::RegisterClipboardFormat("Rich Text Format Without Objects");
//	m_prtf->PasteSpecial(cfRtfWithoutObjects);

	// defined in richedit.h
	// clipboard formats - use as parameter to RegisterClipboardFormat()
//#define CF_RTF 			TEXT("Rich Text Format")
//#define CF_RTFNOOBJS 	TEXT("Rich Text Format Without Objects")
//#define CF_RETEXTOBJ 	TEXT("RichEdit Text and Objects")

	// i should be able to override whatever those ole functions are, and say sorry, no 
	// storage available, right?

	// this just sends a WM_PASTE message, so it's the control itself handling all of this. 
	m_prtf->Paste();

	/*
	BOOL bRtf = ::IsClipboardFormatAvailable(theApp.m_cfRtf);
	if (bRtf)
	{
		// Can we get the raw rtf data and parse it to remove the objects?
		// If the application is performing a paste or paste special operation, you should 
		// create a COleDataObject object and call its AttachClipboard member function. 
		// This associates the data object with the data on the Clipboard. You can then use 
		// this data object in your paste function.
		COleDataObject odo;
		if (odo.AttachClipboard())
		{
			// Get global memory block
			HGLOBAL hGlobal = odo.GetGlobalData(theApp.m_cfRtf);
			if (hGlobal)
			{
				// Get text from global memory into a cstring object
				CString strText;
				LPCSTR psz = (LPCSTR) ::GlobalLock(hGlobal);
				if (psz)
					strText = psz;
				::GlobalUnlock(hGlobal);
				
				// is it possible to add another format to the clipboard, ie add
				// rich text format without objects?
				// then we could paste using that format
				// i don't think windows will let you do that
//				UINT cfRtfWithoutObjects = ::RegisterClipboardFormat("Rich Text Format Without Objects");
//				if (::OpenClipboard())
//				{
//					HGLOBAL hGlobal;
//					char* buffer;
//					::EmptyClipboard();
//					clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
//					buffer = (char*) ::GlobalLock(clipbuffer);
//					strcpy(buffer, LPCSTR(source));
//					::GlobalUnlock(clipbuffer);
//					SetClipboardData(CF_TEXT,clipbuffer);
//					::CloseClipboard();
//				}

  			

//				m_prtf->PasteSpecial(cfRtfWithoutObjects);
				m_prtf->Paste();

			}
		}
	}
	else
		// Paste normally (plain text)
		m_prtf->Paste();
*/

}

// Paste clipboard contents as plain text
void 
CViewRtf::OnCmdEditPastePlain() 
{
	ASSERT_VALID(m_prtf);
	//. for some reason this loses the trailing newline. 
	// can't fix it cause this just sends a EM_PASTESPECIAL message with
	// nformat = CF_TEXT to the dang control. thanks misosoft
	m_prtf->PasteSpecial(CF_TEXT);
}



void 
CViewRtf::OnCmdEditClear() 
{
	ASSERT_VALID(m_prtf);
	m_prtf->Clear();
}




void 
CViewRtf::OnFormatBold()
{
	ASSERT_VALID(m_pViewRtf);
	CHARFORMAT2 cf2;
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2 = m_pViewRtf->GetCharFormatSelection2();
	if (!(cf2.dwMask & CFM_BOLD) || !(cf2.dwEffects & CFE_BOLD))
		cf2.dwEffects = CFE_BOLD;
	else
		cf2.dwEffects = 0;
	cf2.dwMask = CFM_BOLD;
	m_pViewRtf->SetCharFormat2(cf2);
}


void 
CViewRtf::OnFormatItalic()
{
	ASSERT_VALID(m_pViewRtf);
	CHARFORMAT2 cf2;
	cf2 = m_pViewRtf->GetCharFormatSelection2();
	if (!(cf2.dwMask & CFM_ITALIC) || !(cf2.dwEffects & CFE_ITALIC))
		cf2.dwEffects = CFE_ITALIC;
	else
		cf2.dwEffects = 0;
	cf2.dwMask = CFM_ITALIC;
	m_pViewRtf->SetCharFormat2(cf2);
}


void 
CViewRtf::OnFormatUnderline()
{
	ASSERT_VALID(m_pViewRtf);
	CHARFORMAT2 cf2;
	cf2 = m_pViewRtf->GetCharFormatSelection2();
	if (!(cf2.dwMask & CFM_UNDERLINE) || !(cf2.dwEffects & CFE_UNDERLINE))
		cf2.dwEffects = CFE_UNDERLINE;
	else
		cf2.dwEffects = 0;
	cf2.dwMask = CFM_UNDERLINE;
	m_pViewRtf->SetCharFormat2(cf2);
}


void 
CViewRtf::OnFormatStrikethrough()
{
	ASSERT_VALID(m_pViewRtf);
	CHARFORMAT2 cf2;
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2 = m_pViewRtf->GetCharFormatSelection2();
	if (!(cf2.dwMask & CFM_STRIKEOUT) || !(cf2.dwEffects & CFM_STRIKEOUT))
		cf2.dwEffects = CFE_STRIKEOUT;
	else
		cf2.dwEffects = 0;
	cf2.dwMask = CFM_STRIKEOUT;
	m_pViewRtf->SetCharFormat2(cf2);
}


void 
CViewRtf::OnFormatLeft()
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnParaAlign(PFA_LEFT);
}


void 
CViewRtf::OnFormatCenter()
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnParaAlign(PFA_CENTER);
}


void 
CViewRtf::OnFormatRight()
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnParaAlign(PFA_RIGHT);
}


void 
CViewRtf::OnFormatBullet()
{
	// Toggle this on/off

	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf = m_pViewRtf->GetParaFormatSelection();
	if (pf.dwMask & PFM_NUMBERING && pf.wNumbering == PFN_BULLET)
	{
		pf.wNumbering = 0;
		pf.dxOffset = 0;
		pf.dxStartIndent = 0;
		pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
	else
	{
		pf.wNumbering = PFN_BULLET;
		pf.dwMask = PFM_NUMBERING;
		if (pf.dxOffset == 0)
		{
			pf.dxOffset = 360; //720; //m_nBulletIndent; //, could set this in options - bullet indent amount
			pf.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}
	m_pViewRtf->SetParaFormat(pf);

}






// Change the selection to the specified font
// This is called from the toolbar
void 
CViewRtf::ChangeFont(CString& strFontName)
{
	ASSERT_VALID(m_pViewRtf);

	CHARFORMAT2 cf2;
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = CFM_FACE;
//	::lstrcpy(cf2.szFaceName, strFontName); // on NT this is implemented as Unicode
	::lstrcpyn(cf2.szFaceName, strFontName, LF_FACESIZE); //. on NT this is implemented as Unicode
	m_pViewRtf->SetCharFormat2(cf2);
}


// Change the selection to the specified font size
// This is called from the toolbar
void 
CViewRtf::ChangeSize(int nSize)
{
	ASSERT_VALID(m_pViewRtf);
	CHARFORMAT2 cf2;
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = CFM_SIZE;
	cf2.yHeight = nSize; // character height in twips
	m_pViewRtf->SetCharFormat2(cf2);
}




void 
CViewRtf::OnUpdateFormatBold(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnUpdateCharEffect(pCmdUI, CFM_BOLD, CFE_BOLD);
}

void 
CViewRtf::OnUpdateFormatItalic(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnUpdateCharEffect(pCmdUI, CFM_ITALIC, CFE_ITALIC);
}

void 
CViewRtf::OnUpdateFormatUnderline(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnUpdateCharEffect(pCmdUI, CFM_UNDERLINE, CFE_UNDERLINE);
}

void 
CViewRtf::OnUpdateFormatStrikethrough(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnUpdateCharEffect(pCmdUI, CFM_STRIKEOUT, CFE_STRIKEOUT);
}

void 
CViewRtf::OnUpdateFormatLeft(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnUpdateParaAlign(pCmdUI, PFA_LEFT);
}

void 
CViewRtf::OnUpdateFormatCenter(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnUpdateParaAlign(pCmdUI, PFA_CENTER);
}

void 
CViewRtf::OnUpdateFormatRight(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	m_pViewRtf->OnUpdateParaAlign(pCmdUI, PFA_RIGHT);
}

void 
CViewRtf::OnUpdateFormatBullet(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_NUMBERING;
	pf = m_pViewRtf->GetParaFormatSelection();
//	if (pf.wNumbering)
//		pCmdUI->SetCheck(1);
//	else
//		pCmdUI->SetCheck(0);
	pCmdUI->SetCheck( (pf.dwMask & PFM_NUMBERING) ? ((pf.wNumbering & PFN_BULLET) ? 1 : 0) : 2);
}



// Don't do this if toolbar cbo is dropped down
//. fix prob with selecting item
void 
CViewRtf::OnUpdateFormatFont(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);
	CHARFORMAT2 cf2;
	cf2 = m_pViewRtf->GetCharFormatSelection2();
	CComboBox& rcbo = m_tbrRtf.m_cboFontName;
	// If combo is dropped down then don't set its value
	if (rcbo.GetDroppedState() == 0)
	{
		int nIndex = rcbo.FindStringExact(-1, cf2.szFaceName);
		if (nIndex == CB_ERR)
		{
			rcbo.SetCurSel(-1);
		}
		else
		{
			// Select if not already selected
			if (rcbo.GetCurSel() != nIndex)
			{
				rcbo.SetCurSel(nIndex);
			}
		}
	}
}


// Update font size combo based on current selection.
void 
CViewRtf::OnUpdateFormatSize(CCmdUI* pCmdUI)
{
	ASSERT_VALID(m_pViewRtf);

	// If combo is dropped down then don't set its value
	CComboBox& rcbo = m_tbrRtf.m_cboFontSize; // get reference to combo
	if (rcbo.GetDroppedState() == 0)
	{
		// Get current selection's font size
		CHARFORMAT2 cf2;
		cf2 = m_pViewRtf->GetCharFormatSelection2();
		int nHeightPoints = cf2.yHeight / 20; // convert twips to points

		// Find the current point size in the combo
		CString strHeightPoints;
		strHeightPoints.Format(_T("%d"), nHeightPoints); 
		int nIndex = rcbo.FindStringExact(-1, strHeightPoints);
		if (nIndex == CB_ERR)
		{
			// Not found so select nothing
			rcbo.SetCurSel(-1);
		}
		else
		{
			// Select if not already selected
			if (rcbo.GetCurSel() != nIndex)
			{
				rcbo.SetCurSel(nIndex);
			}
		}
	}
}




// ctrl-y handler
void 
CViewRtf::OnCmdEditDeleteLine() 
{
	// first need to select the current line
	// equiv of HOME, Shift+END

}


// Find next occurrence of search text
// This is the F3 handler
void 
CViewRtf::OnCmdEditFindNext() 
{
	m_pViewRtf->FindNext();
}



// Find next occurrence of string, starting at end of current selection.
// Returns zero-based character position of next match, or -1 if not found.
// This is called from CViewSearch when stepping through search results.
long 
CViewRtf::FindNext(CString& strFindText, BOOL bMatchCase, BOOL bWholeWord)
{
	// Store find text in rtfviewex so that F3 will work for it.
	//, yes this is kludgy - would like to merge this with cricheditviewex class - very messy.
	m_pViewRtf->SetFindText(strFindText, bMatchCase, bWholeWord);

	// Search for text starting at current position
	long nPos = m_prtf->FindNext(strFindText, bMatchCase, bWholeWord);
	if (nPos != -1) // found text!
	{	
		xTRACE("CViewRtf::FindNext - found text!!\n");
		// Try to center the text vertically (otherwise it's usually at the bottom of the screen)
		m_prtf->CenterSelectionVertically();
		// Make sure selection is highlighted if this is being called from the search view
		m_prtf->HideSelection(FALSE, FALSE);
	}
	return nPos;
}



//. Find Next should be disabled if no Find has been run to set the search text
void 
CViewRtf::OnUpdateEditFindNext(CCmdUI* pCmdUI) 
{
//	if (_afxRichEditState->strFind.
	pCmdUI->Enable(TRUE);
}




// We want this to always be the active view, 
// but the CRichEditView2 to have the focus,
// hence all these gyrations

void 
CViewRtf::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	xTRACE("CViewRtf OnActivateView %d\n", bActivate);
	CViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);
	// If this view is being activated, set the focus to the child control
	if (bActivate)
	{
		xTRACE("  set focus to CRichEditView2 window\n");
		m_pViewRtf->SetFocus();
	}
}


void 
CViewRtf::OnSetFocus(CWnd* pOldWnd) 
{
	xTRACE("CViewRtf OnSetFocus - set focus to CRichEditView2 window\n");
	CViewEx::OnSetFocus(pOldWnd);	
	if (m_pViewRtf)
	{
		ASSERT_VALID(m_pViewRtf);
		m_pViewRtf->SetFocus();
	}
	// Set the target object to be the currently selected object.
	m_pDoc->SetTargetObject(m_pDoc->GetCurrentObject());
}


void 
CViewRtf::OnKillFocus(CWnd* pNewWnd) 
{
	xTRACE("CViewRtf OnKillFocus - do nothing\n");
	CViewEx::OnKillFocus(pNewWnd);
}



/*
void 
CViewRtf::OnEditDelete() 
{
	// Clear the current selection. Can be undone.
	m_pViewRtf->GetRichEditCtrl().Clear();
}
*/






void 
CViewRtf::OnCmdEditSelectAll() 
{
	xTRACE("CViewRtf::SelectAll\n");
	// Select all text
//	m_pViewRtf->GetRichEditCtrl().SetSel(0, -1);
	m_prtf->SetSel(0, -1);
}


// Set selection to default text color
void 
CViewRtf::SetDefaultColor()
{
	CHARFORMAT2 cf2;
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = CFM_COLOR;
	cf2.dwEffects = CFE_AUTOCOLOR;
	cf2.crTextColor = 0; // this is ignored
	m_pViewRtf->SetCharFormat2(cf2);
}






void 
CViewRtf::OnTest() 
{
/*

	// bug: this code shows that the print preview problem is out of my hands - it's due to the 
	// rtf control's screwy rendering! took a while to figure this out - thought i had screwed
	// up the foundation framework somehow and the scaling was messed up

	// Get printer DC
	CPrintDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		// Create a printer device context (DC) based on the information
		// selected from the Print dialog.
		HDC hPrinterDC = dlg.GetPrinterDC();

		// Create window and draw rtf text into it
		CRect r;
		r.SetRect(0, 0, 900, 700);
		CString strClassName = AfxRegisterWndClass(NULL, 0, (HBRUSH) ::GetStockObject(WHITE_BRUSH), 0);
		m_wnd.CreateEx(NULL, strClassName, "Test", WS_VISIBLE | WS_OVERLAPPEDWINDOW, r, NULL, 0);
		
		CDC* pDC = m_wnd.GetDC();

		// Scale screen
		HDC hScreenDC = pDC->m_hDC;
		int nView = 500; // scaling factor out of 1000
		::SetMapMode(hScreenDC, MM_ANISOTROPIC);
		::SetWindowExtEx(hScreenDC, 1000, 1000, NULL);
		::SetViewportExtEx(hScreenDC, nView, nView, NULL);

		pDC->TextOut(0, 0, strClassName);

		// get page rect
		CRect r2;
		CSize szPaper = theApp.m_sizePaper; // twips
		CRect rMargin = theApp.m_rectPageMargins; // twips
		r2.SetRect(rMargin.left, rMargin.top, szPaper.cx - rMargin.right, szPaper.cy - rMargin.bottom);

		FORMATRANGE fr;
		fr.hdcTarget = hPrinterDC;
		fr.hdc = hScreenDC;
		fr.chrg.cpMin = 0; // nIndexStart; // start character
		fr.chrg.cpMax = -1; // nIndexStop; // stop character
		fr.rc = r2;
		fr.rcPage = r2;

		// Now render the rich edit contents to the device context
		long lRes = m_prtf->FormatRange(&fr, TRUE);

		// release printer dc
		::DeleteDC(hPrinterDC);
		m_wnd.ReleaseDC(pDC);
	}   

	::GlobalFree(dlg.m_pd.hDevMode);
	::GlobalFree(dlg.m_pd.hDevNames);
*/
}





/*
// obsolete
void 
CViewRtf::ColorHyperlinks()
{
	// search through text for hyperlink prefixes
	// (www. )
	// color the relevant text
	// add to array of hyperlinks for mouseover code

	// could predefine this for speed, store an array of them!
	FINDTEXTEX ftPrefix;
	ftPrefix.chrg.cpMin = 0;
	ftPrefix.chrg.cpMax = -1;
	ftPrefix.lpstrText = "www."; // hyperlink type 1
	
	FINDTEXTEX ftSpace;
	ftSpace.chrg.cpMin = 0;
	ftSpace.chrg.cpMax = -1;
	ftSpace.lpstrText = " ";

	m_nHyperlinks = 0;

	// use sendmessage for speed
	long n = m_prtf->FindText(0, &ftPrefix);
	if (n != -1)
	{
		// Find the rest of the hyperlink by searching for the following " "
		CHARRANGE crHyperlink = ftPrefix.chrgText;
		crHyperlink.cpMax = ::SendMessage(m_prtf->m_hWnd, EM_FINDWORDBREAK, WB_RIGHT, crHyperlink.cpMin);
//		crHyperlink.cpMax = ::SendMessage(m_prtf->m_hWnd, EM_FINDWORDBREAK, WB_RIGHTBREAK, crHyperlink.cpMin);

//		ftSpace.chrg.cpMin = crHyperlink.cpMax;
//		long nChar = m_prtf->FindText(0, &ftSpace);
//		crHyperlink.cpMax = (nChar == -1) ? -1 : nChar - 1;
//		crHyperlink.cpMax = m_prtf->FindText(0, &ftSpace);
	// test at end of doc also!

		// walk through characters, checking for space, CR, eof, etc.
		
		// Color the hyperlink
		SetFormat(crHyperlink, m_cfHyperlinkNormal);

		// Save hyperlink type and start/stop positions to arrays
//		m_aHyperlinkType.Add(nHyperlinkType);
		m_aHyperlinkStart.Add(crHyperlink.cpMin);
		m_aHyperlinkStop.Add(crHyperlink.cpMax);
		m_nHyperlinks++;
	}

}

void 
CViewRtf::OnMouseMove(UINT nFlags, CPoint point) 
{
	CViewEx::OnMouseMove(nFlags, point);
	int nHyperlink = GetHyperlinkFromPos(&point);
	if (nHyperlink != -1)
	{
		// Highlight the hyperlink
		CHARRANGE cr;
		cr.cpMin = m_aHyperlinkStart.GetAt(nHyperlink);
		cr.cpMax = m_aHyperlinkStop.GetAt(nHyperlink);
		SetFormat(cr, m_cfHyperlinkHighlight);
	}
}

  
void 
CViewRtf::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CViewEx::OnLButtonDown(nFlags, point);
	// Check if clicked on a hyperlink
	int nHyperlink = GetHyperlinkFromPos(&point);
	if (nHyperlink != -1)
	{
		AfxMessageBox("Clicked on a hyperlink!!!");
	}
}

  
// See if the given point is over a hyperlink. 
// Returns the number of the hyperlink, or -1 if none. 
// Inline for speed.
inline int 
CViewRtf::GetHyperlinkFromPos(CPoint* point)
{
	if (m_nHyperlinks > 0)
	{
		// Get character that mouse is over
		// The return value specifies the zero-based character index of the character nearest the 
		// specified point. The return value indicates the last character in the edit control if the 
		// specified point is beyond the last character in the control. 
		// Note: Help says to use POINTL structure but it's the same as POINT!
		ULONG nChar = ::SendMessage(m_prtf->m_hWnd, EM_CHARFROMPOS, 0, (LPARAM) &point);

		// Look through array of hyperlink start/stop positions to see if mouse is over one
		for (int i = 0; i < m_nHyperlinks; i++)
		{
			if (nChar >= m_aHyperlinkStart.GetAt(i)) 
			{
				if (nChar <= m_aHyperlinkStop.GetAt(i))
					// Found hyperlink - return it
					return i;
			}
			else
				break; // break out of loop if nChar < hyperlinkstart
		}
	}
	return -1; // not over a hyperlink
}
*/



// Set the specified character range to the specified character format.
// Note: This is inline for speed (called during mousemove etc).
//, also uses SendMessage for speed.
inline void 
CViewRtf::SetFormat(CHARRANGE& cr, CHARFORMAT& cf)
{
	CHARRANGE crSavePosition;
	m_prtf->HideSelection(TRUE, FALSE); 
	m_prtf->GetSel(crSavePosition);
	m_prtf->SetSel(cr);
	m_prtf->SetSelectionCharFormat(cf);
	m_prtf->SetSel(crSavePosition);
	m_prtf->HideSelection(FALSE, FALSE); 
//	::SendMessage(m_prtf->m_hWnd, EM_EXGETSEL, 0, (LPARAM) &crSavePosition);
//	::SendMessage(m_prtf->m_hWnd, EM_EXSETSEL, 0, (LPARAM) &cr);
//	::SendMessage(m_prtf->m_hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
//	::SendMessage(m_prtf->m_hWnd, EM_EXSETSEL, 0, (LPARAM) &crSavePosition);
}





// Save any attached ole objects as hidden subobjects and remove them
void 
CViewRtf::SaveOleObjects()
{
	xTRACE("CViewRtf::SaveOleObjects()\n");

	// Calls to the IStorage::EnumElements method supplies a pointer to IEnumSTATSTG. 
	// The caller allocates an array of STATSTG structures and the IEnumSTATSTG methods fill in 
	// each structure with the statistics about one of the nested elements in the storage object. 
	// If present, the lpszName member of the STATSTG structure requires additional memory allocations 
	// through the IMalloc interface, and the caller is responsible for freeing this memory, if it is allocated, 
	// by calling the IMalloc::Free method. 
	// If the lpszName member is NULL, no memory is allocated, and, therefore, no memory needs to be freed.

	IRichEditOle* preo = m_prtf->GetIRichEditOle();
	if (preo)
	{
		int nObjects = preo->GetObjectCount();
		
		xTRACE("\nRichedit has %u objects....\n", nObjects);
		REOBJECT reo;
		::ZeroMemory(&reo, sizeof(REOBJECT));
		reo.cbStruct = sizeof(REOBJECT);
		for (int i = 0; i < nObjects; i++)
		{
			//. object, storage, site interfaces
			if (SUCCEEDED(preo->GetObject(i, &reo, REO_GETOBJ_PSTG)))
//			if (SUCCEEDED(preo->GetObject(i, &reo, REO_GETOBJ_POLESITE)))
//			if (SUCCEEDED(preo->GetObject(i, &reo, REO_GETOBJ_POLEOBJ)))
			{
				xTRACE("  Object %u: charpos %u, cx %u, cy %u\n", i, reo.cp, reo.sizel.cx, reo.sizel.cy);
/*
				if (reo.poleobj)
				{
					// The rich edit objects do not implement IPersistStream!
//					IPersistStream* pIPersistStream;
//					if (SUCCEEDED(reo.poleobj->QueryInterface(IID_IPersistStream, (void **) &pIPersistStream)))
//					{
//						IStream* pIStream;
//						::OleSaveToStream(pIPersistStream, pIStream);
//						pIPersistStream->Release();
//					}

					// but they do implement this!
					IPersistStorage* pIPersistStorage;
					if (SUCCEEDED(reo.poleobj->QueryInterface(IID_IPersistStorage, (void **) &pIPersistStorage)))
					{
						pIPersistStorage->
						pIPersistStorage->Release();
					}

					reo.poleobj->Release();
				}
*/

				if (reo.pstg)
				{

					// Open and read a stream!
					// Use IStorage::OpenStream to get an IStream interface
//					COleStreamFile osf;
//					if (osf.OpenStream(reo.pstg, "\001Ole"))
//					{
//						CFileStatus fs;
//						osf.GetStatus(fs);
//						DWORD dwLength = osf.GetLength();
//						BYTE Buffer[100];
//						osf.Read(&Buffer, 100);
//					}

					// Walk through elements of storage
					// The storage object must be open in read mode to allow the enumeration of its elements.

					STATSTG ss;
					if (SUCCEEDED(reo.pstg->Stat(&ss, STATFLAG_DEFAULT)))
					{
						xTRACE("    Storage: %S, size %u\n", ss.pwcsName, ss.cbSize.LowPart);
						CoTaskMemFree(ss.pwcsName);
					}

					IEnumSTATSTG* pEnum;
					if (SUCCEEDED(reo.pstg->EnumElements(0, NULL, 0, &pEnum)))
					{
						ULONG nFetched;
						LPCTSTR szTypes[] = {"(0)", "Storage", "Stream", "Lockbytes", "Property"};
						pEnum->Reset(); // go to beginning of list
						while (S_OK == pEnum->Next(1, &ss, &nFetched))
						{
							ASSERT(ss.type < 5);
							xTRACE("      %s %S, size %u\n", szTypes[ss.type], ss.pwcsName, ss.cbSize.LowPart);
//							if (ss.type == STGTY_STREAM)
//							{
//								TRACE("      Stream: %S, size %u\n", ss.pwcsName, ss.cbSize.LowPart);
//							}
//							else if (ss.type == STGTY_STORAGE)
//							{
//								TRACE("      Storage: %S, size %u\n", ss.pwcsName, ss.cbSize.LowPart);
//							}
							CoTaskMemFree(ss.pwcsName);
						}

						//, then save it to a hidden child object
						//, then delete it from the rtf control before saving (freeze screen before doing so)

						pEnum->Release();
					}
					reo.pstg->Release();
				}



			}
		}
		preo->Release();
	}


/*
	// Remove any OLE objects (images etc) and save as hidden binary child objects
	// On load will restore them
	COleDataObject odo;
	CRichEditCntrItem* pItem;
	CRichEditDoc* pRedoc = m_pViewRtf->GetDocument();
	//, for whatever reason this isn't working - pos = 0 
	// need to update the richeditdoc somehow??
	POSITION pos = pRedoc->GetStartPosition();
	while (pos != NULL)
	{
		pItem = DYNAMIC_DOWNCAST(CRichEditCntrItem, pRedoc->GetNextItem(pos));
		if (pItem)
		{
			pItem->AttachDataObject(odo);
//			HGLOBAL hGlobal = odo.GetGlobalData(_oleData.cfNative);
		}
	}
*/


}





/*
// Don't delete this - good for debugging
BOOL 
CViewRtf::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pnm = (NMHDR*) lParam;
//	UINT nIDFrom = pnm->idFrom;
//	UINT nCode = pnm->code;
//	CString str = theApp.GetNotifyCodeString(nCode);
//	TRACE("Cviewrtf OnNotify id %d, code %d %s\n", nIDFrom, nCode, (LPCTSTR) str);

	if (nCode == TBN_DROPDOWN)
	{
//		AfxMessageBox("dropdown!");
		// All you have to do is send the toolbar a TB_GETRECT message, which gets the button 
		// rectangle from a command ID instead of zero-index (as TB_GETITEMRECT does). 
		// Then use the coordinates to display your popup menu in the right place. 
	}

	return CViewEx::OnNotify(wParam, lParam, pResult);
}
*/





/*
// User clicked on a toolbar dropdown button, so bring up appropriate menu or dialog.
// TBN_DROPDOWN handler.
void 
CViewRtf::OnToolbarDropDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTOOLBAR* pNmtb = (NMTOOLBAR*) pNMHDR;
//	UINT nButtonID = m_tbrRtf.IndexToCommand(pNmtb->iItem); 
	UINT nButtonID = pNmtb->iItem; // Bug: Confusing - thought iItem was 0 based index of button!

	switch (nButtonID)
	{
		case ID_FORMAT_FORECOLOR:
			GetNewForecolor();
			break;
		case ID_FORMAT_BACKCOLOR:
			GetNewBackcolor();
			break;
	}

	// Return one of the following values:
	// TBDDRET_DEFAULT The drop-down was handled. 
	// TBDDRET_NODEFAULT The drop-down was not handled. 
	// TBDDRET_TREATPRESSED The drop-down was handled, but treat the button like a regular button. 
//	*pResult = 0;
	*pResult = TBDDRET_DEFAULT;
//	*pResult = TBDDRET_TREATPRESSED;
}
*/


// Apply current text color
void 
CViewRtf::OnFormatApplyForecolor()
{
	CHARFORMAT2 cf2;
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = CFM_COLOR;
	cf2.dwEffects = 0;
	cf2.crTextColor = theApp.m_clrTextViewForecolor;
	if (theApp.m_clrTextViewForecolor == CLR_DEFAULT)
		cf2.dwEffects = CFE_AUTOCOLOR;
	m_pViewRtf->SetCharFormat2(cf2);

	// load icon into memory structure
	// modify the color table for a specific color index
	// use this icon for the image
	// actually gets image from image index?
//	TBBUTTONINFO tbbi;
//	m_tbrRtf.SetButtonInfo(nID, &tbbi);
//	TB_REPLACEBITMAP
//	HBITMAP hbmNew;
//	HINSTANCE hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
//	HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);
//	hbmNew = AfxLoadSysColorBitmap(m_hInstImageWell, m_hRsrcImageWell);
//	HBITMAP
//	m_tbrRtf.AddReplaceBitmap(hbmNew);
}


// Apply current background color
void 
CViewRtf::OnFormatApplyBackcolor()
{	
	CHARFORMAT2 cf2;
	cf2.cbSize = sizeof(CHARFORMAT2);
	cf2.dwMask = CFM_BACKCOLOR;
	cf2.dwEffects = 0;
	cf2.crBackColor = theApp.m_clrTextViewBackcolor;
	if (theApp.m_clrTextViewBackcolor == CLR_DEFAULT)
		cf2.dwEffects = CFE_AUTOBACKCOLOR;
	m_pViewRtf->SetCharFormat2(cf2); 
}



// Get a new text color
void 
CViewRtf::OnFormatForecolor()
{
	m_tbrRtf.m_ctlForecolor.DropDown(FALSE);
/*
	ASSERT_VALID(m_pViewRtf);

//	CColorDialog dlg(m_clrForecolor);
//	dlg.m_cc.lpCustColors = &theApp.m_clrCustomColors[0];
//	if (dlg.DoModal() == IDOK)
//	{
//		m_clrForecolor = dlg.GetColor();
//		OnFormatForecolor();
//	}

	CRect r;
	UINT uIndex = m_tbrRtf.CommandToIndex(ID_FORMAT_FORECOLOR);
	VERIFY(m_tbrRtf.GetItemRect(uIndex, &r));
	ClientToScreen(&r);

	CString strDefaultText = "Automatic";
	CString strCustomText = "More Colors...";

	// Control will send CPN_SELENDOK message on click
	new CColourPopup(CPoint(r.left, r.bottom), theApp.m_clrTextViewForecolor, this, strDefaultText, strCustomText);
*/
}


//void CViewRtf::GetNewBackcolor() 
void 
CViewRtf::OnFormatBackcolor() 
{
	m_tbrRtf.m_ctlBackcolor.DropDown(FALSE);

//	ASSERT_VALID(m_pViewRtf);
//	CColorDialog dlg(theApp.m_clrTextViewBackcolor);
//	dlg.m_cc.lpCustColors = &theApp.m_clrCustomColors[0];
//	if (dlg.DoModal() == IDOK)
//	{
//		theApp.m_clrTextViewBackcolor = dlg.GetColor();
//		OnFormatBackcolor();
//	}
}



// Color was selected in color window.
LRESULT 
CViewRtf::OnSelEndOK(WPARAM wParam, LPARAM lParam) 
{
	COLORREF clr = (COLORREF) wParam;
	UINT uID = (UINT) lParam;

	CWnd *pParent = GetParent();
//	if (uID == ID_FORMAT_FORECOLOR)
	if (uID == ID_FORMAT_APPLY_FORECOLOR)
	{
		if (clr != theApp.m_clrTextViewForecolor)
		{
			if (clr == CLR_DEFAULT)
				theApp.m_clrTextViewForecolor = g_clrWindowText;
			else
				theApp.m_clrTextViewForecolor = clr;
			OnFormatApplyForecolor();
		}
	}
//	else if (uID == ID_FORMAT_BACKCOLOR)
	else if (uID == ID_FORMAT_APPLY_BACKCOLOR)
	{
		if (clr != theApp.m_clrTextViewBackcolor)
		{
//			if (clr == CLR_DEFAULT)
//				theApp.m_clrTextViewBackcolor = g_clrWindow;
//			else
				theApp.m_clrTextViewBackcolor = clr;
			OnFormatApplyBackcolor();
		}
	}

	// Keep focus on rtf
	m_pViewRtf->SetFocus();

    return TRUE;
}



// User clicked on a color button.
LRESULT 
CViewRtf::OnColorButtonClick(WPARAM wParam, LPARAM lParam) 
{
	COLORREF clr = (COLORREF) wParam;
	UINT uID = (UINT) lParam;

	switch (uID)
	{
/*
		case ID_FORMAT_FORECOLOR:
			if (clr != theApp.m_clrTextViewForecolor)
			{
				theApp.m_clrTextViewForecolor = clr;
				OnFormatApplyForecolor();
			}
			break;
		case ID_FORMAT_BACKCOLOR:
			if (clr != theApp.m_clrTextViewBackcolor)
			{
				theApp.m_clrTextViewBackcolor = clr;
				OnFormatApplyBackcolor();
			}
			break;
*/
	case ID_FORMAT_APPLY_FORECOLOR:
			OnFormatApplyForecolor();
			break;
		case ID_FORMAT_APPLY_BACKCOLOR:
			OnFormatApplyBackcolor();
			break;
	}

	// Keep focus on rtf
	m_pViewRtf->SetFocus();

    return TRUE;
}




void 
CViewRtf::OnEditClearFormat() 
{
	// Set selected text to default font, size, and clear all effects
	//. only do if text is selected

	// Get default font and apply it to the selection
	CHARFORMAT2 cf2;
	theApp.m_fontRtfDefault.GetCharFormat2(cf2);
	m_pViewRtf->SetCharFormat2(cf2);
}



void 
CViewRtf::OnEditInsertLink() 
{
	//. bring up a dialog to select link type and link object
	// file, email, website, ftp, news
	// could insert a link to an image? eg store a thumbnail of it, click on to open real file?
	// might be cool

	// When Rich Edit locates a URL, it changes the URL text color, underlines the text, 
	// and notifies the client using EN_LINK.
	// Rich Edit also recognizes standard path names that start with \\. 

	CDialogInsertLink dlg;
	if (IDOK == dlg.DoModal())
	{
		m_prtf->ReplaceSel("mailto:bburns@neomem.org", TRUE);
	}

}


void 
CViewRtf::OnCmdEditInsertObject() 
{
}



void 
CViewRtf::OnCmdEditInsertLine() 
{
	ASSERT_VALID(m_pViewRtf);

	// Get the current font
	CFontEx font;
	VERIFY(m_prtf->GetCurrentFont(font));

	// Get width of 100 characters
	// Note: The character '–' looks better but rtf renders it as "\endash" and it doesn't work for all fonts.
	const TCHAR cDash = '-';
	int nMeasureChars = 100;
	CString strMeasure(cDash, nMeasureChars);
	CClientDC dc(this);
	CRect rSize;
	// bug: GetTextExtent was returning (0, 1) and couldn't understand why - it was because 
	// the font was miniscule as a result of not multiplying the point size by 10 in CFontEx code.
	//. Still screwy for Arial font - does rtf control render text differently than gettextextent??
	
	CFont* pfontOld = dc.SelectObject(&font);
	CSize sz = dc.GetTextExtent(strMeasure);
	dc.SelectObject(pfontOld);
	int nMeasureWidth = sz.cx;
	ASSERT(nMeasureWidth != 0);

	// Get the number of characters required to fill x% of the rtf width
	CRect r;
	GetClientRect(r);
	int nPercentWidth = 90;
	int nDesiredWidth = r.right * nPercentWidth / 100; // pixels
	int nChars = nDesiredWidth * nMeasureChars / nMeasureWidth;

	// Create line string
	CString strLine(cDash, nChars);
	strLine += "\n"; // add carriage return to end

	// If in the middle of a line, prepend a carriage return!
	if (!m_prtf->IsAtStartOfLine())
		strLine = "\n" + strLine;

	// Replace current selection with line
	m_prtf->ReplaceSel(strLine, TRUE);


/*
	// what if we just added characters one by one until one of them goes to another line?
	// that might be more robust than this approach.
	// then just back off a character or two.
	// only drawback is that undo only undoes part of the line now!!!

	// Note: For ReplaceSel, if there is no current selection, the replacement text is inserted 
	// at the insertion point, that is, the current caret location.

	// Clear current selection
	m_prtf->ReplaceSel("");

	// Insert a line feed if necessary so we start on a new line
	if (!m_prtf->IsAtStartOfLine())
		m_prtf->ReplaceSel("\n");

	long nStartChar;
	long nEndChar;
	m_prtf->GetSel(nStartChar, nEndChar); // gets current selection
	long nLine = m_prtf->LineFromChar(nStartChar);
	long nLine2 = nLine;

	// Now insert the ending line feed so won't get stuck with any other non-space characters.
	m_prtf->ReplaceSel("\n");
	m_prtf->GetSel(nStartChar, nEndChar); // gets current selection
	m_prtf->SetSel(nStartChar - 1, nStartChar - 1); // backup one char to before the CR!

	// Insert dashes until we reach the next line
	int nChars = 0;
	while (nLine == nLine2)
	{
		m_prtf->ReplaceSel("-");
		m_prtf->GetSel(nStartChar, nEndChar);
		nLine2 = m_prtf->LineFromChar(nStartChar);
		nChars++;
	}

	// Now back off 10% and delete dashes
	int nBack = nChars * 10 / 100; // eg 161 * 10 / 100 = 1610/100 = 16
	if (nBack == 0) nBack = 1;
	nStartChar = nStartChar - nBack; // use nstartchar left over from loop
	m_prtf->SetSel(nStartChar, nEndChar);
	m_prtf->ReplaceSel("");

	// Advance one character
	m_prtf->GetSel(nStartChar, nEndChar);
	m_prtf->SetSel(nStartChar + 1, nStartChar + 1);
*/

}




// To receive EN_LINK notifications, specify the ENM_LINK flag in the mask sent with the 
// EM_SETEVENTMASK message. 
// If you send the EM_AUTOURLDETECT message to enable automatic URL detection, 
// the rich edit control automatically sets the CFE_LINK effect for modified text that it identifies as a URL. 
//. does this mean we can set the cfe_link style ourselves for different text?? cool!
// could also link to other objects if we wanted! ie could say insert link to object, 
// store hidden text objectid, then regular text for object name!
// this code would intercept clicking on it!
void 
CViewRtf::OnRtfLink(NMHDR* pNMHDR, LRESULT* pResult)
{
	ENLINK* pLink = (ENLINK*) pNMHDR;

	xTRACE("CViewRtf::OnRtfLink(msg=0x%x)\n", pLink->msg);
	
	// If you return zero, the control proceeds with its normal handling of the message.
	// If you return a nonzero value, the control does not handle the message.
	*pResult = 0;

	if (pLink->msg == WM_LBUTTONDOWN)
	{
		// Remember charrange
		m_chrgMouseDown = pLink->chrg;
	}

	if (pLink->msg == WM_LBUTTONUP)
	{
		// Make sure user is still on same hyperlink (can just check start char)
		if (m_chrgMouseDown.cpMin == pLink->chrg.cpMin)
		{
			// Get text of hyperlink (eg "mailto:bburns@neomem.org", "file:c:\test.txt")
			CString strHyperlink;
			TEXTRANGE tr;
			tr.chrg = pLink->chrg;
			int nLength = tr.chrg.cpMax - tr.chrg.cpMin + 1;
			tr.lpstrText = strHyperlink.GetBuffer(nLength);
			m_prtf->SendMessage(EM_GETTEXTRANGE, 0, (LPARAM) &tr);
			strHyperlink.ReleaseBuffer();
//			AfxMessageBox(strHyperlink);
//			HINSTANCE h = ::ShellExecute(NULL, "open", strHyperlink, NULL, NULL, SW_SHOWNORMAL);

			// 1.1c use ShellExecuteEx to get better error messages if hyperlinking fails 
			// (as it was for a user)
			//! er, just use shellexecute and call HandleShellExecuteError(h)?
			SHELLEXECUTEINFO sei = {0}; //! cool - better than ZeroMemory!
			sei.cbSize = sizeof(SHELLEXECUTEINFO);
//			sei.fMask = SEE_MASK_NOCLOSEPROCESS;
			sei.hwnd = NULL;
			sei.lpVerb = "open";
			sei.lpFile = strHyperlink;
			sei.lpParameters = "";
			sei.lpDirectory = NULL;
//			sei.nShow = SW_SHOW;
			sei.nShow = SW_SHOWNORMAL;
			sei.hInstApp = NULL;
			// Returns TRUE if successful, or FALSE otherwise. Call GetLastError for error information. 
			if (FALSE==ShellExecuteEx(&sei))
			{
				// There was an error...
//				DWORD n = ERROR_FILE_NOT_FOUND;
//				CString s = GetErrorMessage(n);
				// this will call GetLastError and FormatMessage to get an error message
				CString s = GetErrorMessage(0);
				AfxMessageBox((LPCTSTR) s, MB_OK | MB_ICONINFORMATION);
			}
//			WaitForSingleObject(sei.hProcess,INFINITE);


		}
		// Clear charrange
		m_chrgMouseDown.cpMin = -1;
		m_chrgMouseDown.cpMax = -1;
	}

/*
	if (pLink->msg == WM_RBUTTONDOWN)
	{
		// this doesn't get triggered for some reason
//		AfxMessageBox("rdown hyperlink popup!");
	}
	if (pLink->msg == WM_RBUTTONUP)
	{
		//. bring up hyperlink popup menu here?
		//. this is really confusing interfacing with rbuttondown in cricheditview2!
//		AfxMessageBox("rup hyperlink popup!");

		CMenu menu;	
		CPoint ptScreen(::GetMessagePos()); // Get position of message source
		if (menu.LoadMenu(IDR_POPUP_HYPERLINK))
		{
			CMenu* pPopup = menu.GetSubMenu(0);
			if (pPopup)
			{
				pPopup->SetDefaultItem(ID_EDIT_OPEN_HYPERLINK);
//				pPopup->EnableMenuItem(ID_OBJ_CHANGE_OBJECT_CLASS, bNoModifyClass ? MF_GRAYED : MF_ENABLED);
//				::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, NULL, pView->m_hWnd, NULL);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
			}
		}
	}
*/

}



// Insert symbol character
void 
CViewRtf::OnEditInsertSymbol() 
{
	CWaitCursor wc;
	HINSTANCE h = ::ShellExecute(NULL, "open", "charmap", NULL, NULL, SW_SHOWNORMAL);
	HandleShellExecuteError(h);
}



// Needs selection
void 
CViewRtf::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	long nStartChar, nEndChar;
	m_prtf->GetSel(nStartChar, nEndChar);
	pCmdUI->Enable(nStartChar != nEndChar);
	ASSERT_VALID(this);
}


// Needs something on clipboard
void 
CViewRtf::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(m_prtf->CanPaste());
}


// Needs text in the rtf control
void 
CViewRtf::OnUpdateNeedText(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(m_prtf->GetTextLength() != 0);
}


// Needs find text
void 
CViewRtf::OnUpdateNeedFind(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
//	_AFX_RICHEDIT_STATE* pEditState = _afxRichEditState;
//	pCmdUI->Enable(m_prtf->GetTextLength() != 0 && !pEditState->strFind.IsEmpty());
	pCmdUI->Enable();
}




// Shift text right by one tab stop
void 
CViewRtf::OnCmdEditShiftRight() 
{
	// walk through selected text, inserting \\tabs after \\pars
	// getrtf text, walk through it and change it, then write it back
	//.. encapsulate this routine in some rtf handling class? put in ctrlex class?
//	CRichEditCtrlEx* prtf = (CRichEditCtrlEx*) &(m_pViewRtf->GetRichEditCtrl());

	CString strNew;
	CString strSelection = m_prtf->GetRtf(TRUE);
	AfxMessageBox(strSelection); //.

	// string will always start with "\pard" for whatever reason
	int nChar = strSelection.Find("\\pard", 0);
	if (nChar != -1)
	{
		// find next space
		nChar = strSelection.Find(" ", nChar);
		if (nChar != -1)
		{
			// found space, next char is first line of text where we want to insert tab
			strNew = strSelection.Left(nChar+1) + "\\tab " + strSelection.Mid(nChar+1);
			//.. now walk through finding "\\par " and inserting more "\\tab " s
			// replace "\\par\r\n" with "\\par\r\n\\tab "
			// make sure that character preceding \\par is not \, otherwise could be literal text, not a return code!
			// also ignore if followed by }, ie "\\par\r\n}"
			//. might have to read through char by char and parse to make sure it's safe!
			//0d0a = cr nl
//			strNew.Replace("\\par\r\n", "\\par\r\n\\tab ");
			int nStart = 0;
			nChar = strNew.Find("\\par\r\n", nStart);
			while (nChar != -1)
			{
				CString strPrev = strNew.Mid(nChar - 1, 1);
				CString strNext = strNew.Mid(nChar + 6, 1);
				if ((strPrev != "\\") && (strNext != "}"))
					strNew = strNew.Left(nChar+6) + "\\tab " + strNew.Mid(nChar+6);
				nStart = nChar + 6;
				nChar = strNew.Find("\\par\r\n", nStart);
			}
		}
	}

	AfxMessageBox(strNew); //.

	// Write new string back to rtf
	m_prtf->SetRtf(strNew, TRUE);
}


// Shift selected text left by one tab stop
void 
CViewRtf::OnCmdEditShiftLeft() 
{
}


// See CRichEditView2::OnRButtonDown
void 
CViewRtf::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	xTRACE("CViewRtf OnContextMenu %d %d\n", point.x, point.y);		
}



void 
CViewRtf::OnCmdEditFind() 
{
	// pass command on to cricheditview
//	m_pViewRtf->FindText(m_strFindText, m_bMatchCase, m_bWholeWord);
	m_pViewRtf->SendMessage(WM_COMMAND, ID_EDIT_FIND);
}


void 
CViewRtf::OnCmdEditReplace() 
{
	// pass command on to cricheditview
	m_pViewRtf->SendMessage(WM_COMMAND, ID_EDIT_REPLACE);
}


void 
CViewRtf::OnFormatFont() 
{
	// pass command on to cricheditview
	m_pViewRtf->SendMessage(WM_COMMAND, ID_FORMAT_FONT);
}




// Goto top of text
// Called by CViewSearch to make sure we start search at beginning of document
void 
CViewRtf::GotoStart()
{
	m_prtf->SetSel(0, 0);
}


// Undo last change to rtf contents.
void 
CViewRtf::OnCmdEditUndo() 
{
	ASSERT_VALID(m_prtf);
	m_prtf->Undo();

	//. now get current format to make sure it's current...
	// update is done in background, how do we do this?
}


// Redo last undo. 
void 
CViewRtf::OnCmdEditRedo() 
{
	ASSERT_VALID(m_prtf);
	m_prtf->Redo();
	//. now get current format to make sure it's current...
	// update is done in background, how do we do this?
}


void 
CViewRtf::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(m_prtf->CanUndo());
}


void 
CViewRtf::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(m_prtf->CanRedo());
}



// Toggle zoom on/off
// Requires 3.0
void 
CViewRtf::OnViewZoom() 
{
	m_bZoom = ! m_bZoom;
	SetZoom();
}

// Requires 3.0
void 
CViewRtf::OnUpdateViewZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bZoom ? 1 : 0);	
}


// 1.2 Set zoom for rtf control	
// Requires 3.0
void 
CViewRtf::SetZoom()
{
	return;

	int nNum = 0;
	int nDenom = 0;
	if (m_bZoom) 
	{
		nNum = 125;
		nDenom = 100;
	}
	m_prtf->SendMessage(EM_SETZOOM, (WPARAM) nNum, (LPARAM) nDenom);
}





void 
CViewRtf::AdjustWindow()
{
	// Bug: Window got stuck in some sizes and wouldn't update - turned out I was using
	// GetRect and SetRect instead of GetClientRect and SetRect to update the rtf window!

	// Bug: This seems to work alright, but if you load another file then close it, this window
	// gets resized twice, the first time smaller, the second time back to normal, 
	// but the second time doesn't seem to stick. 
	// Solution - pass 0 to SetRect and it will use default size - don't need to do GetRect and 
	// all that (found this in the EM_SETRECT message help). Fixed problem with getting stuck.

/*
	//	m_prtf->RedrawWindow();
//	m_prtf->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	CRect r;
//	m_prtf->GetRect(&r);
	m_prtf->GetClientRect(&r);
	// need to also subtract size of scroll bar
	int nScrollBarWidth = ::GetSystemMetrics(SM_CXVSCROLL);
	int nRightMargin = 4; // pixels
	// also a little margin
	r.right -= nScrollBarWidth - nRightMargin;
	m_prtf->SetRect(&r);
*/

	// Use default formatting rectangle
	m_prtf->SetRect(NULL);
}


// Move selected text to another object
void 
CViewRtf::OnEditMoveTo() 
{
	ASSERT_VALID(m_pDoc);
	BObject* pobjCurrent = m_pDoc->GetCurrentObject();
	ASSERT_VALID(pobjCurrent);
	ULONG lngOldParentID = pobjCurrent->GetObjectID();

	CDialogGetLocation dlg;
	dlg.m_strCaption = "Move Text To...";
//	dlg.m_strInstructions = "Select the new location for the text and hit OK. The text will be moved to the end of the selected object's Text View. Use right and left arrows to expand and collapse items. ";
	dlg.m_pobj = pobjCurrent;
	dlg.m_nHelpID = IDD_MOVE_TEXT_TO;
	if (IDOK == dlg.DoModal())
	{
		BObject* pobjNew = dlg.m_pobj;
		if (pobjNew)
		{
			ASSERT_VALID(pobjNew);
			xTRACE("Move text to %s\n", pobjNew->GetName(TRUE));
			CString strRtf;
			strRtf = m_prtf->GetRtf(TRUE); // get current selection
			m_prtf->Clear(); // clear current selection
			// If moving to current object save the text after deleting the selection.
			// Then AddRtf will add the new text to the end of the rtf.
			if (pobjNew == pobjCurrent)
			{
				UpdateView(NULL, hintSave, pobjCurrent); // save text to current object 
			}
			pobjNew->AddRtf(propRtfText, strRtf); // add selection to new location
		}	
	}

}


// Sort selected text alphabetically
void 
CViewRtf::OnEditSortSelectedText() 
{
	// walk through text, adding each paragraph to a list
	// then sort the list, spit it all out to a string, 
	// then replace the selection with the new string
	//.. encapsulate this routine in some rtf handling class? put in ctrlex class?
//	CRichEditCtrlEx* prtf = (CRichEditCtrlEx*) &(m_pViewRtf->GetRichEditCtrl());

	CString strNew;
	CStringEx strSelection = m_prtf->GetRtf(TRUE);
	AfxMessageBox(strSelection); //.


	// a bitch - looks like
	// {rtf codes blah blah first line\par
	// second line\par
	// third line\par
	// so would need to parse rtf shit correctly to find first line
	// do later

/*
//	strSelection.GetNextToken(pszDelim);
//	CString cs;
//	while(tok.Next(cs))
//		TRACE2("Token: '%s', Tail: '%s'\n", (LPCTSTR)cs, (LPCTSTR)tok.Tail());

	// string will always start with "\pard" for whatever reason
	int nChar = strSelection.Find("\\pard", 0);
	if (nChar != -1)
	{
		// find next space
		nChar = strSelection.Find(" ", nChar);
		if (nChar != -1)
		{
			// found space, next char is first line of text where we want to insert tab
			strNew = strSelection.Left(nChar+1) + "\\tab " + strSelection.Mid(nChar+1);
			//.. now walk through finding "\\par " and inserting more "\\tab " s
			// replace "\\par\r\n" with "\\par\r\n\\tab "
			// make sure that character preceding \\par is not \, otherwise could be literal text, not a return code!
			// also ignore if followed by }, ie "\\par\r\n}"
			//. might have to read through char by char and parse to make sure it's safe!
			//0d0a = cr nl
//			strNew.Replace("\\par\r\n", "\\par\r\n\\tab ");
			int nStart = 0;
			nChar = strNew.Find("\\par\r\n", nStart);
			while (nChar != -1)
			{
				CString strPrev = strNew.Mid(nChar - 1, 1);
				CString strNext = strNew.Mid(nChar + 6, 1);
				if ((strPrev != "\\") && (strNext != "}"))
					strNew = strNew.Left(nChar+6) + "\\tab " + strNew.Mid(nChar+6);
				nStart = nChar + 6;
				nChar = strNew.Find("\\par\r\n", nStart);
			}
		}
	}

	AfxMessageBox(strNew); //.

	// Write new string back to rtf
	m_prtf->SetRtf(strNew, TRUE);
*/

}



// Set the default font in the RTF control.
//, This doesn't do quite what you think it will - it sets the entire contents of the rtf to this font,
// even if some characters have explicit font settings! Sometimes!!?
// So only use it when the control is blank!?
void 
CViewRtf::SetDefaultFont(CFontEx& font)
{
	ASSERT_VALID(m_prtf);
 	CHARFORMAT2 cf2;
 	font.GetCharFormat2(cf2);
	TRACE("SetDefaultFont to %s, yHeight=%d\n", (LPCTSTR) font.GetAsText(), cf2.yHeight);
 	m_prtf->SetDefaultCharFormat2(cf2);
}



void 
CViewRtf::OnEditInsertDate() 
{
	ASSERT_VALID(m_pViewRtf);

	COleDateTime odt;
	odt = COleDateTime::GetCurrentTime();
	CString strDate = odt.Format();

	// Replace current selection with date string (it's how notepad works)
	m_prtf->ReplaceSel(strDate, TRUE);
}


