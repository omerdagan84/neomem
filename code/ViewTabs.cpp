
// CViewTabs



#include "precompiled.h"

#include "ViewTabs.h"
#include "Constants.h"
#include "DialogViews.h"
#include "FrameChild.h"
#include "Hint.h"
#include "NeoMem.h"
// Will need to include headers of all view files so we can add their runtime class objects
#include "ViewBookmarks.h"
#include "ViewContents.h"
#include "ViewGrid.h"
#include "ViewHome.h"
#include "ViewIndex.h"
#include "ViewPictures.h"
#include "ViewProperties.h"
#include "ViewRtf.h"
#include "ViewSearch.h"
#include "ViewTree.h"

#include "BObject.h"
#include "ConstantsDatabase.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





// Macros
//--------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNCREATE(CViewTabs, CViewEx)

BEGIN_MESSAGE_MAP(CViewTabs, CViewEx)
	//{{AFX_MSG_MAP(CViewTabs)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_VIEW_EDIT, OnViewEdit)
	ON_COMMAND(ID_TAB_HIDE, OnTabHide)
//	ON_COMMAND(ID_VIEW_DATA, OnCmdViewID)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_DATA, OnUpdateViewID)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_VIEW_FIRST, ID_VIEW_LAST, OnCmdViewID)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_FIRST, ID_VIEW_LAST, OnUpdateViewID)
	ON_COMMAND(ID_TAB_MERGE, OnTabMerge)
	ON_NOTIFY(NM_CLICK, IDC_TAB_CONTAINER, OnTabClick)
	ON_NOTIFY(NM_RCLICK, IDC_TAB_CONTAINER, OnTabRightClick)
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONTAINER, OnTabSelChange)
END_MESSAGE_MAP()



// Construction/Destruction
//--------------------------------------------------------------------------------------------------------------------

CViewTabs::CViewTabs()
{
	xTRACE("CViewTabs Constructor\n");

	// Initialize module variables
//	m_pSplitter = NULL;
//	pviewCurrent = NULL;
	m_lngMode = 0;
	m_lngCurrentViewID = 0;
	m_bDrag = FALSE;
	m_nTabRClick = 0;

//	m_pdatTabs = NULL;
	m_pdatViews = NULL;
	m_Splitters.SetSize(10);
//	m_aCurrentViews
//	m_pCurrentChildWnd = NULL;
	m_nCurrentTab = 0;

	m_bViewsChanged = FALSE;
	m_bViewsCopied = FALSE;
	m_bViewsSaved = FALSE;

	m_bytViewHeight = 0;
//	m_pDoc = NULL;
}


CViewTabs::~CViewTabs()
{
	// Destroy splitter windows
	for (int i = 0; i < m_Splitters.GetSize(); i++)
	{
		CSplitterWndEx* pSplitter = DYNAMIC_DOWNCAST(CSplitterWndEx, m_Splitters[i]);
		if (pSplitter)
		{
			delete pSplitter;
		}
	}
	// Delete copied view info if it was not saved
	DeleteCopiedViewInfo();

	// Delete BData object
	delete m_pdatViews;
}


/* done by cviewex now
BOOL CViewTabs::PreCreateWindow(CREATESTRUCT& cs) 
{
	// WS_CLIPCHILDREN   Excludes the area occupied by child windows when you draw 
	// within the parent window. Used when you create the parent window.
	// Helps with flickering
	cs.style |= WS_CLIPCHILDREN;
	return CViewEx::PreCreateWindow(cs);
}
*/


// Note: The context information is passed on from the framework
//	CCreateContext *pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;
int CViewTabs::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	xTRACE("CViewTabs OnCreate - create tab control\n");

	// Call base class
	if (CViewEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create the tab control
	CRect r;
	GetClientRect(r);
	if (m_tbc.Create(
							WS_VISIBLE | 
							WS_CHILD | 
							WS_CLIPCHILDREN |
//							WS_CLIPSIBLINGS |
							TCS_FOCUSNEVER | 
							TCS_TOOLTIPS | 
							TCS_TABS |
							0,						// Style
							r,					// Size and position
							this,				// Pointer to parent window
							IDC_TAB_CONTAINER	// Control ID
							))
	{
		// Set font
		m_tbc.SetFont(&theApp.m_fontControls);
	}
	else
		return -1;

	return 0;
}



// Drawing
//--------------------------------------------------------------------------------------------------------------------

void CViewTabs::OnDraw(CDC* pDC)
{
	xTRACE("CViewTabs OnDraw mode %d\n", m_lngMode);
	CViewEx::OnDraw(pDC);
}



// Diagnostics
//--------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
void CViewTabs::AssertValid() const
{
	CViewEx::AssertValid();
}

void CViewTabs::Dump(CDumpContext& dc) const
{
	CViewEx::Dump(dc);
}
#endif //_DEBUG





// Message Handlers
//--------------------------------------------------------------------------------------------------------------------


// Warning: The treeview gets OnUpdate BEFORE OnInitialUpdate!
void CViewTabs::OnInitialUpdate() 
{
	xTRACE("CViewTabs OnInitialUpdate - add tabs, select current tab, load that view\n");

	// If in navigation mode then add tabs and select view
	if (m_lngMode == modeNavigation) 
	{
		ASSERT (m_tbc.GetItemCount() == 0);

		// Get left side tab/view arrangement from document
		m_pdatViews = DYNAMIC_DOWNCAST(BDataViews, m_pDoc->m_datViewsLeft.CreateCopy());
		ASSERT_VALID(m_pdatViews);

		// Modify tab control to correspond with list of available views
		UpdateTabs();

		// Select view
		SelectView(viewData);

		// for some reason, if you don't pass pobjRoot, the tab ctrl doesn't draw itself, and
		// is transparent.
		// Invalidate doesn't help
		// it might be because when you load the tree, then change the object,
		// the tree selchange event is not being triggered
		// The problem was that we were telling the treeview to select an item which wasn't
		// in the treeview, so it wasn't triggering the selchange event, and
		// the other views weren't getting updated properly
		// The solution was to set m_pobjCurrent to the docroot object instead of the root object
		// in the OnNewDocument handler in CNeoDoc

		// If in admin mode, start with main root, otherwise start with user root
		//, this is also set in OnNewDocument - combine?
		BObject* pobjStart;
//		if (theApp.m_bAdmin)
//			pobjStart = pDoc->GetRoot();
//		else
//			pobjStart = pDoc->GetObject(rootUser);
		pobjStart = m_pDoc->GetRoot();
		ASSERT_VALID(pobjStart);

		// Load the treeview with the specified root item.
		// The hintLoadTree handler also selects the current object (pdoc->m_pobjCurrent).
		m_pDoc->UpdateAllViewsEx(this, hintLoadTree, pobjStart);

	}
	else
	{
		// If in contents mode, add tabs now?
		// could get current object from document, which should have set it by now(?)
//		m_tbc.SetExtendedStyle
//		BObject* pobjCurrent = pDoc->GetCurrentObject();
//		OnUpdate(this, hintSelect, pobjCurrent);
	}

	// Call base class
	// all this does is call OnUpdate with 0 0
//	CViewEx::OnInitialUpdate();
}



void CViewTabs::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	//, For now, just need to handle updates for right side
	if (m_lngMode != modeContents)
		return;

	TRACE("    CViewTabs(right side)::OnUpdate %s\n", theApp.GetHintName(lHint));

	switch (lHint)
	{
		// We can ignore these messages because they don't have anything to do with the tab control
//		case hintAdd:
//		case hintDelete:
//		case hintRename:
//		case hintLoadTree:
//		case hintResize:
//			RecalcLayout(TRUE);
//			SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS);
//			break;

		case hintSelect:

			// If this view is on the right side (contents mode) then add tabs as appropriate
			// to the selected object.
//			if (m_lngMode == modeContents)
			{
				// Get hint object
				BObject* pobj = DYNAMIC_DOWNCAST(BObject, pHint);
				ASSERT_VALID(pobj);

				// Load views associated with the object
				LoadViews(pobj);

			}
			break;

		case hintSave:
			{
				// Save changes to tab/view arrangement, if any

				// check if any splitter pane heights changed
				// do this because not able to get message when user resizes splitter bar, apparently

				//, for now, 
				// walk through panes in splitter
				// will need to handle this as they are resized, because panes might not be visible any more, etc
				// so in splitterwnd recalclayout, we need to modify the pdattabs,
				// setting the new height and setting the modified flag so that hintsave will know to save the new values

				if (m_bViewsChanged)
				{
					ASSERT(m_bViewsCopied); // this must be true if m_bViewsChanged is true
					ASSERT_VALID(m_pdatViews);

					BObject* pobj = DYNAMIC_DOWNCAST(BObject, pHint);
					ASSERT_VALID(pobj);
					
					// For v1, just store height of first view in the object
					// (each object gets its own height)
					pobj->SetViewHeight(m_bytViewHeight);

//					// Save view arrangement to class or object depending on what kind of object it is.
					// Always save to object's class, even for folders.
					//, v2 will save view heights etc to object.
//					if (pobj->GetClassID() == classFolder)
//					{
//						// Save view info to the object
//						pobj->SetPropertyData(propViewArrangement, m_pdatViews, FALSE, FALSE);
//					}
//					else
//					{
						// Save view info to the object's class
						ULONG lngClassID = pobj->GetClassID();
						BObject* pobjClass = m_pDoc->GetObject(lngClassID);
						ASSERT_VALID(pobjClass);
						pobjClass->SetPropertyData(propObjectViewArrangement, m_pdatViews, FALSE, FALSE);
//					}

					// Set/reset flags
					// Note: m_bViewsCopied will still be true
					m_bViewsSaved = TRUE; // m_pdatViews has been saved to a bobject
					m_bViewsChanged = FALSE;
				}

			}
			break;


		case hintAddView:
			// Add a view to the current view arrangement
			//, for now limit to 2 views per tab
			{
				BObject* pobjView = DYNAMIC_DOWNCAST(BObject, pHint);
				ASSERT_VALID(pobjView);
				ULONG lngViewID = pobjView->GetObjectID();
				ASSERT(lngViewID);
				AddView(lngViewID);
			}
			break;

		case hintRemoveView:
			// Remove a view from the current view arrangement.
			//, prevent if only view? should remove entire tab otherwise.
			{
				BObject* pobjView = DYNAMIC_DOWNCAST(BObject, pHint);
				ASSERT_VALID(pobjView);
				ULONG lngViewID = pobjView->GetObjectID();
				ASSERT(lngViewID);
				RemoveView(lngViewID);
			}
			break;

		case hintMoveViewUp:
			// Move the specified view up in the view arrangement.
			{
				BObject* pobjView = DYNAMIC_DOWNCAST(BObject, pHint);
				ASSERT_VALID(pobjView);
				ULONG lngViewID = pobjView->GetObjectID();
				ASSERT(lngViewID);
				MoveViewUp(lngViewID);
			}
			break;


		case hintPropertyChange:
			{
				// If current object's class changed, reload view arrangement in case it's different
				CHint* ph = DYNAMIC_DOWNCAST(CHint, pHint);
				if (ph->idProperty == propClassID)
				{
					BObject* pobjCurrent = m_pDoc->GetCurrentObject();
					if (ph->pobjObject == pobjCurrent)
					{
						// Load views associated with the object
						LoadViews(pobjCurrent);
					}
				}
			}
			break;
	}
	
}



// Update tab control to correspond to m_pdatViews data
void CViewTabs::UpdateTabs()
{
	ASSERT_VALID(m_pdatViews);

//	CNeoDoc* pDoc = (CNeoDoc*) GetDocument();
	//, lame!
	if (m_pDoc == NULL)
		m_pDoc = DYNAMIC_DOWNCAST(CNeoDoc, GetDocument());

	// Clear tabs
	m_tbc.DeleteAllItems();

	// Get number of tabs
	int nTabs = m_pdatViews->GetTabCount();

	// Walk through tabs
	for (int nTab = 0; nTab < nTabs; nTab++)
	{
		// Get the name for the tab (eg "Text, Contents")
		CString strTabName;
		m_pdatViews->GetTabName(nTab, strTabName, m_pDoc);

		// Create tab
		int nImage = 0; // might have icon in future
		m_tbc.InsertItem(TCIF_TEXT, nTab, strTabName, nImage, 0);
	}

	// Delete any leftover tabs that we don't need
	for (int nTab = nTabs; nTab < m_tbc.GetItemCount(); nTab++)
		VERIFY(m_tbc.DeleteItem(nTab));

	// Bug: Had to add this to fix a refresh problem - tabs weren't being updated when they changed.
	m_tbc.RedrawWindow();

}



void CViewTabs::OnSize(UINT nType, int cx, int cy) 
{
	// Call base class
	CViewEx::OnSize(nType, cx, cy);

	// Size the contained controls
	RecalcLayout(FALSE);
}



void CViewTabs::RecalcLayout(BOOL bRepaint)
{
	CRect r;
	GetClientRect(r);

	// Move tabstrip to new position
	if (::IsWindow(m_tbc))
		m_tbc.MoveWindow(0, 0, r.right, r.bottom, FALSE);

	// If there is just one current view, then resize it, otherwise, resize the splitter window
	int nViews = m_aCurrentViews.GetSize();
	m_tbc.GetClientRect(r);
	m_tbc.AdjustRect(FALSE, r);
	if (nViews == 0)
		return;
	if (nViews == 1)
	{
		CViewEx* pView = STATIC_DOWNCAST(CViewEx, m_aCurrentViews[0]);
		pView->MoveWindow(r, TRUE);
	}
	else
	{
		// Resize the splitter
		CSplitterWndEx* pSplitter = STATIC_DOWNCAST(CSplitterWndEx, m_Splitters[m_nCurrentTab]);
		if (pSplitter)
		{
			// get current height of panes as percent of avail height
//			int cyCur, cyMin;

			///// hmm - if we store the % height in the tab data, why not just use that?
			// so if user resizes splitter panes we'll have to write that to the tab data
			// otherwise this will get out of whack

//			UINT nAvailHeight = r.bottom;
//			pSplitter->GetClientRect(r);
//			UINT nAvailHeight = r.bottom;
//			int nRows = pSplitter->GetRowCount();
//			CUIntArray aHeights;
//			for (int nRow = 0; nRow < nRows - 1; nRow++) // skip last row
//			{
//				pSplitter->GetRowInfo(nRow, cyCur, cyMin);
//				UINT nHeightPercent = cyCur * 100 / nAvailHeight;
//				aHeights.Add(nHeightPercent);
//			}
			pSplitter->MoveWindow(r, TRUE);
			// now adjust the heights of the panes to keep them proportionate
//			for (nRow = 0; nRow < nRows - 1; nRow++) // skip last row
//			{
//				UINT nHeightPercent = aHeights.GetAt(nRow);
//				UINT nHeight = nHeightPercent * nAvailHeight / 100;
//				pSplitter->SetRowInfo(nRow, nHeight, nHeight);
//			}
		}
	}
}
	




// Methods
//--------------------------------------------------------------------------------------------------------------------


// Set mode of view to either modeNavigation or modeContents
// Affects how Update events are handled
void CViewTabs::SetMode(ULONG lngMode)
{
	xTRACE("CViewTabs SetMode %d\n", lngMode);
	m_lngMode = lngMode;
	// Note: Can't select tab here because tries to lookup the view object to get the viewtype,
	// and at this point the document has not been created yet.
	// For that reason, we can't use addtab here - better to put it in update handler
}





// Select a view by selecting a tab
// Will load the specified view if necessary
// Returns TRUE if view is available
BOOL CViewTabs::SelectView(ULONG lngViewID)
{
	// Find which tab the view is on, select it and display it
	int iTab = FindView(lngViewID);
	if (iTab != -1)
	{
		// Select the tab (will call ShowTab)
		SelectTab(iTab);

		// Now set focus to the specified view
		// Note: This is not good because if you use keyboard to navigate the treeview, you lose the focus.
//		CView* pView = GetView(lngViewID);
//		pView->SetFocus();

		// Remember which view is current so can try to keep it visible
		m_lngCurrentViewID = lngViewID;

		return TRUE;
	}

	// Return FALSE if view not available
	return FALSE;
}


// Select the specified tab as if the user had clicked on it (will call ShowTab)
void CViewTabs::SelectTab(int nTab)
{
	// This should trigger the tabclick event, which will load the view if necessary.
	// It doesn't seem to, so need to call it manually
//	m_tbc.SetCurFocus(nTab);
	// Make sure nTab is valid, otherwise select first tab
	if (nTab < m_tbc.GetItemCount())
		m_tbc.SetCurSel(nTab);
	else
		m_tbc.SetCurSel(0);

	// Handle as if user had clicked on the tab (will call ShowTab)
	OnTabClick(NULL, NULL);
}



// Find which tab the specified view is on, if any
// Returns 0 based tab index, or -1 if view not available
int CViewTabs::FindView(ULONG lngViewID)
{
	ASSERT_VALID(m_pdatViews);
	return m_pdatViews->GetViewTab(lngViewID);
}




// User clicked on a tab - switch the current view
void CViewTabs::OnTabClick(NMHDR* pNotify, LRESULT* result)
{
	// Determine which tab was clicked, exit if none selected
	// BUG:: All of a sudden, this starting returning nothing but 0 no matter which tab was clicked
	// it was because i had commented out some code in lbuttondown handler in ctabctrlex
	int nTab = m_tbc.GetCurSel();
	if (nTab == -1) return;

	// Show the tab (splitter window and any child views),
	// and hide current tab
	ShowTab(nTab);

	// Activate the first view in the list (but only if this is triggered by the user, not by code)
	// Note: pNotify will be non-null if this is triggered by the user.
	// If this is called from code it will be NULL
	if (pNotify)
		ActivateFirstView();
}




// Activate the first view in the list
// Note: This is called by framechild in F6'ing between views
void CViewTabs::ActivateFirstView()
{
	if (m_aCurrentViews.GetSize() > 0)
	{
		CViewEx* pView = STATIC_DOWNCAST(CViewEx, m_aCurrentViews.GetAt(0));
		if (pView)
		{
			ASSERT_VALID(pView);
			CFrameChild* pChild = DYNAMIC_DOWNCAST(CFrameChild, GetParentFrame());
			ASSERT_VALID(pChild);
			pChild->SetActiveView(pView, TRUE);
		}
	}
}



// Show the specified tab and whatever views it may contain
// Also hides whatever tab is currently displayed, if different
// Note: Not using Dynamic_downcast for speed
void CViewTabs::ShowTab(int nTab)
{
	xTRACE("CViewTabs ShowTab %d\n", nTab);

	ASSERT(nTab < m_tbc.GetItemCount());

	// We first need to reset views and splitter windows to a neutral state
	ResetViews();

	// Get the start index for the specified tab
	int nStart = m_pdatViews->GetTabIndex(nTab);
	int nViews = m_pdatViews->GetViewCount(nTab);

	// If there is just one view in this tab, the view takes up all the client space, and the splitter
	// window for this tab is hidden.
	if (nViews == 1)
	{
		ViewInfo& rvi = m_pdatViews->GetViewInfo(nStart);
		ULONG lngViewID = rvi.m_lngViewID;

		// Load view, but don't display it yet
		CViewEx* pView = LoadView(lngViewID);
		ASSERT_VALID(pView);

		// Make the view the child of the tab control
		pView->SetParent(&m_tbc);

		// Hide miniheader (title bar) since we just have one view
		// Bug: Had this AFTER movewindow below, so the view's resize event was messed up.
//		pView->m_bShowTitleBar = FALSE; 
		pView->ShowTitleBar(FALSE);

		// Now position the view
		CRect r;
		m_tbc.GetClientRect(r);
		m_tbc.AdjustRect(FALSE, r);
		pView->MoveWindow(r);

		// Set the window at the top of the z-order (so it repaints correctly) and show it
		pView->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS | SWP_SHOWWINDOW);

		// Add view to array of current views
		m_aCurrentViews.Add(pView);

		// Enable the view
		pView->EnableWindow(TRUE); //. test

		// Send the hintSelect current object to the view to update it
		//. test
//x		BObject* pobjCurrent = DYNAMIC_DOWNCAST(CNeoDoc, GetDocument())->GetCurrentObject();
		CNeoDoc* pdoc = DYNAMIC_DOWNCAST(CNeoDoc, GetDocument());
		ASSERT_VALID(pdoc);
		BObject* pobjCurrent = pdoc->GetCurrentObject();
		pView->UpdateView(this, hintSelect, pobjCurrent);

		// Save this as the current view
		m_lngCurrentViewID = lngViewID;

	}
	else
	{
		// We have more than one view to show - make sure the splitter window associated with
		// this tab has enough panes, and assign a view window to each pane.

		// Initialize the splitter for this tab to the required number of views.
		// Returns a pointer to the splitter, and sets m_bNewSplitter if had to
		// deleteing existing one and create a new one.
		//, maybe should pass ref to flag instead of hiding it in a m_ var
		CSplitterWndEx* pSplitter = InitializeSplitter(nTab, nViews);
		ASSERT_VALID(pSplitter);

		// Get available height - use splitter window for consistency
		CRect r;
		pSplitter->GetClientRect(r);
		int nAvailHeight = r.bottom;

		// Walk through views and add them to the splitter window
		int nIndex = nStart;
		for (int iView = 0; iView < nViews; iView++)
		{
			// Get the ViewID
			ViewInfo& rvi = m_pdatViews->GetArray()[nIndex];
			ULONG lngViewID = rvi.m_lngViewID;
			ASSERT(lngViewID);
			nIndex++;

			// Remember the first view
			if (iView == 0)
				m_lngCurrentViewID = lngViewID;

			// Load view, but don't display it yet
			CViewEx* pView = LoadView(lngViewID);
			ASSERT_VALID(pView);

			// Turn on the titlebar for each view below the first view
//			pView->m_bShowTitleBar = (iView != 0);
			pView->ShowTitleBar(iView != 0);
			if (iView != 0)
			{
				BObject* pobjView = m_pDoc->GetObject(lngViewID);
				ASSERT_VALID(pobjView);
//				pView->m_strTitleBar = pobjView->GetPropertyText(propName);
//				pView->SetTitleBarName(pobjView->GetPropertyText(propName));
				//,,, bad - this is the third place this code exists - encapsulate it!
				CString strView = pobjView->GetPropertyText(propCaption);
				if (strView.IsEmpty())
					strView = pobjView->GetPropertyText(propName); // use name if caption is blank
				pView->SetTitleBarName(strView);
			}

			// Add the view to the splitter control
			pSplitter->SetPane(iView, 0, pView);

			// Set pane height as percent of total, unless it's the last view
			//, For v1, get first pane's height from bobject
			if (iView == 0)
				pSplitter->SetRowHeightPct(iView, (int) m_bytViewHeight, FALSE);
//			if (iView < nViews - 1)
			else if (iView < nViews - 1)
			{
				int nHeightPercent = rvi.m_lngViewHeight;
				pSplitter->SetRowHeightPct(iView, nHeightPercent, FALSE);
			}

			// Show the view window
			// (the splitter window is still hidden at this point, so this won't become visible yet)
			pView->ShowWindow(SW_SHOW);

			// Add view to array of current views
			m_aCurrentViews.Add(pView);

			// enable the window
			pView->EnableWindow(TRUE);//. test

			// Send the hintSelect current object to the view
			//.test
//x			BObject* pobjCurrent = DYNAMIC_DOWNCAST(CNeoDoc, GetDocument())->GetCurrentObject();
			CNeoDoc* pdoc = DYNAMIC_DOWNCAST(CNeoDoc, GetDocument());
			ASSERT_VALID(pdoc);
			BObject* pobjCurrent = pdoc->GetCurrentObject();
			pView->UpdateView(this, hintSelect, pobjCurrent);

		}

		// Position the splitter
		// only need to do this if it's a new splitter - problem when opening a document.
		// had problem dealing with maximizing main frame, so now do this every time.
//		if (m_bNewSplitter)
//		{
			m_tbc.GetClientRect(r);
			m_tbc.AdjustRect(FALSE, r);
			pSplitter->MoveWindow(r, FALSE);
//		}

		// Show the splitter
		pSplitter->ShowWindow(SW_SHOW);

		// BUG: Views were not being positioned properly sometimes. The help said that you didn't
		// need to call this if you resized the panes with the splitter invisible, but I tried it anyway,
		// and it fixed the problem.
		pSplitter->RecalcLayout();

	}

	// Unfreeze the display
	//. not working right
//	SetRedraw(TRUE);
//	Invalidate(FALSE);

	// Save the current tab
	m_nCurrentTab = nTab;
}





// Reset views and splitters to a neutral state
void CViewTabs::ResetViews()
{
	// Freeze the display
	// Set all current views as children of the tab control
	// Hide them all
	// Hide the splitter

	// An application calls SetRedraw to allow changes to be redrawn or to prevent changes 
	// from being redrawn. This member function sets or clears the redraw flag. While the 
	// redraw flag is cleared, the contents will not be updated after each change and will 
	// not be repainted until the redraw flag is set. For example, an application that needs 
	// to add several items to a list box can clear the redraw flag, add the items, and then 
	// set the redraw flag. Finally, the application can call the Invalidate or InvalidateRect 
	// member function to cause the list box to be repainted.
	SetRedraw(FALSE);

	// Hide all current views and set them as children of the tab control
	int nViews = m_aCurrentViews.GetSize();
	for (int iView = 0; iView < nViews; iView++)
	{
		CViewEx* pView = STATIC_DOWNCAST(CViewEx, m_aCurrentViews[iView]);
		ASSERT_VALID(pView);
		pView->ShowWindow(SW_HIDE);
		pView->EnableWindow(FALSE);//. test
		pView->SetParent(&m_tbc);
		//, tell splitter also?
//		pSplitter->ClearPane(iView, 0);
	}

	// Make sure the splitter window is hidden also
	//. make sure array is dimensioned correctly
	CSplitterWndEx* pSplitter = STATIC_DOWNCAST(CSplitterWndEx, m_Splitters[m_nCurrentTab]);
	if (pSplitter) 
	{
		ASSERT_VALID(pSplitter);
		pSplitter->ShowWindow(SW_HIDE);
	}

	// Clear array of current views
	m_aCurrentViews.RemoveAll();

	// Unfreeze the display
	//. flickering a bit
	SetRedraw(TRUE);
//	Invalidate(FALSE);

}






// Initialize the splitter associated with the specified tab, setting it to have the required
// number of panes. May need to delete existing splitter if number of panes needs to change.
// Returns pointer to the splitter window.
CSplitterWndEx* CViewTabs::InitializeSplitter(int nTab, int nViews)
{
	// Make sure splitter storage array is big enough.
	// Note: Each tab is associated with a different splitter window, and
	// they are all children of the tab control. Only one can be visible at a time.
	// They are stored in the m_Splitters obarray.
	if (nTab + 1 > m_Splitters.GetSize())
		m_Splitters.SetSize(nTab + 1);

	// If the splitter for this tab does not exist yet, create it now
	CSplitterWndEx* pSplitter = DYNAMIC_DOWNCAST(CSplitterWndEx, m_Splitters[nTab]);
	int nRows;
	if (pSplitter)
		nRows = pSplitter->GetRowCount();
	else
		nRows = 0;

	// Set flag for creating a new splitter. This is to fix problem with opening a file - 
	// splitter does not get positioned correctly and so remains invisible
//	BOOL bNewSplitter = FALSE;
	m_bNewSplitter = FALSE;

	// If the splitter does not have the right number of panes, we need to delete it and create a new one.
	if (nRows != nViews)
	{
		// Delete existing splitter and create another with right number of panes
		if (pSplitter) 
			delete pSplitter;
		pSplitter = new CSplitterWndEx;
		ASSERT(pSplitter);
		pSplitter->CreateStatic(&m_tbc, nViews, 1);
		ASSERT_VALID(pSplitter);

		// Set mode of splitter window so that it has no borders and caption below splitter bars
//		pSplitter->SetMode(modeSplitterContents); 
		pSplitter->SetMode(CSplitterWndEx::modeSplitterContents); 

		// Add splitter to array
		m_Splitters.SetAt(nTab, pSplitter);

		// Need to trigger OnSize so that splitter window gets positioned correctly
		// or just size it here...
		// Note: Can't size it here because the splitter requires the child views to be assigned
//		CRect r;
//		m_tbc.GetClientRect(r);
//		m_tbc.AdjustRect(FALSE, r);
//		pSplitter->MoveWindow(r, TRUE);
//		bNewSplitter = TRUE;
		m_bNewSplitter = TRUE;
	}

	ASSERT_VALID(pSplitter);
	return pSplitter;
}



// Find the specified view in the ViewID to CViewEx object map.
// This is used so that view objects need to be created only once, then hidden
// when not needed.
// Returns NULL if not found.
// See also LoadView
CViewEx* CViewTabs::GetView(ULONG lngViewID)
{
	CViewEx* pView;
	if (m_Map.Lookup((WORD) lngViewID, (void*&) pView))
	{
		ASSERT_VALID(pView);
		ASSERT_KINDOF(CViewEx, pView);	
		return pView;
	}
	else
	{
		return NULL;
	}
}



// Load a cview but don't show it
// If already loaded, just return a pointer to it
//. Should this update the view as well? probably not
CViewEx* CViewTabs::LoadView(ULONG lngViewID)
{
	CWaitCursor cw;

	CViewEx* pView = GetView(lngViewID);
	if (pView)
	{
		xTRACE("CViewTabs LoadView %d found in map\n", lngViewID);

		// Note: Since the view is already loaded, it should already have been updated to
		// show the current object's data.
		//,, Might want to rethink this as that will slow things down - if all views are updated
		// when you click on an item, that's not very efficient. Better to do it only as view is shown.
	}

	// Otherwise load a new view object
	else
	{
		xTRACE("CViewTabs LoadView %d not in map - loading cview object\n", lngViewID);

		// Each view is associated with a different CView class 
		// In the future, two or more views might share a CView class also
		CRuntimeClass* pClass = 0;
		switch (lngViewID)
		{
			case viewText:
				pClass = RUNTIME_CLASS(CViewRtf);
				break;
			case viewContents:
				pClass = RUNTIME_CLASS(CViewContents);
				break;
			case viewData:
				pClass = RUNTIME_CLASS(CViewTree);
				break;
			case viewSearch:
				pClass = RUNTIME_CLASS(CViewSearch);
				break;
			case viewProperties:
				pClass = RUNTIME_CLASS(CViewProperties);
				break;
			case viewIndex:
				pClass = RUNTIME_CLASS(CViewIndex);
				break;
			case viewHome:
				pClass = RUNTIME_CLASS(CViewHome);
//				pClass = RUNTIME_CLASS(CViewHome2);
				break;
/*
			case viewBookmarks:
				pClass = RUNTIME_CLASS(CViewBookmarks);
				break;
			case viewPictures:
				pClass = RUNTIME_CLASS(CViewPictures);
				break;
			case viewGrid:
				pClass = RUNTIME_CLASS(CViewGrid);
				break;
			case viewGraphPaper:
				pClass = RUNTIME_CLASS(CViewGraphPaper);
				break;
			case viewHTML:
				pClass = RUNTIME_CLASS(CViewHTML);
				break;
			case viewMap:
				pClass = RUNTIME_CLASS(CViewMap);
				break;
*/			default:
				AfxMessageBox("Need switch handler for view in LoadView");
				pClass = 0;
				return 0;
		}

		// BUG: In release version, this doesn't assert, so if you open an old file or something,
		// pClass can be 0, which will bomb the program in CreateChildView.
		// Now it returns 0 from the default case handler.
		ASSERT(pClass);

		// Add a View as a child of the tabstrip
		// Bug: declared pView again in braces, which meant that it disappeared before the end.
		CRect r;
		r.SetRectEmpty();
		pView = STATIC_DOWNCAST(CViewEx, CreateChildView(pClass, GetDocument(), r, 0));
		ASSERT_VALID(pView);

		// Set the mode of the new view to match this mode
		//, don't need to worry about this till we use tree, search or bookmark on the right
//		pView->SetMode(

		// Add the view to the map
		//. catch memory exception
		m_Map.SetAt((WORD) lngViewID, (void*) pView);

		// Now update the view, as we just loaded it
		// BUG: This somehow wiped out the m_pDocument pointer in the release version only.
		//    Very weird bug - hard to trace too.
//		pView->SendMessage(WM_SELECT_CURRENT, 0, 0);
//		AfxCallWndProc(pView, pView->m_hWnd, WM_SELECT_CURRENT, 0, 0);

		// Now tell the view to load itself
		// Send the negative of the view we want to send the message to,
		// in order to have the message received by only one view
//		BObject* pobjCurrent = ((CNeoDoc*) GetDocument())->GetCurrentObject();
//		GetDocument()->UpdateAllViewsEx(-pView, hintLoad, pobjCurrent);

		// Store the ViewID in the view so it knows what it is
		pView->SetViewID(lngViewID);

		// Call ViewEx routine that calls view's OnUpdate routine
		if (theApp.m_bIgnoreLoadHint == FALSE)
		{
//			pView->SelectCurrent();
			BObject* pobjCurrent = m_pDoc->GetCurrentObject();
			pView->UpdateView(this, hintLoad, pobjCurrent);
		}
	}

	return pView;
}





// Returns pointer to next visible view, or NULL if current view is not in list, or
// -1 if already at end of list.
// Called by CFrameChild object to implement F6 navigation
CView* CViewTabs::GetNextView(CView *pviewCurrent)
//CViewEx* CViewTabs::GetNextView(CViewEx *pviewCurrent)
{
	// Check assumptions
	ASSERT_VALID(&m_aCurrentViews);
	ASSERT(m_aCurrentViews.GetSize() > 0);

	// If passed 0, they just want the first view
	if (pviewCurrent == NULL)
	{
		CView* pView = STATIC_DOWNCAST(CView, m_aCurrentViews[0]);
		ASSERT_VALID(pView);
		return pView;
	}

	int nViews = m_aCurrentViews.GetSize();
	for (int i = 0; i < nViews; i++)
	{
		CView* pView = STATIC_DOWNCAST(CView, m_aCurrentViews[i]);
		if (pviewCurrent == pView)
		{
			// If already at end of list, return -1
			if (i == nViews - 1)
				return (CView*) -1;
//				return (CViewEx*) -1;
			else
			{
				// Return pointer to next view in sequence
				CView* pNextView = STATIC_DOWNCAST(CView, m_aCurrentViews[i+1]);
				ASSERT_VALID(pNextView);
				return pNextView;
			}				
		}
	}
	// Current view was not found in list
	return NULL;
}



// Returns pointer to previous visible view, or NULL if current view is not in list, or
// -1 if already at end of list.
// Called by CFrameChild object to implement F6 navigation
//, merge with getnext?
CView* CViewTabs::GetPreviousView(CView *pviewCurrent)
//CViewEx* CViewTabs::GetPreviousView(CViewEx *pviewCurrent)
{
	// Check assumptions
	ASSERT_VALID(&m_aCurrentViews);
	ASSERT(m_aCurrentViews.GetSize() > 0);

	int nViews = m_aCurrentViews.GetSize();
	
	// If passed 0, they just want the last view
	if (pviewCurrent == NULL)
	{
		CView* pView = STATIC_DOWNCAST(CView, m_aCurrentViews[nViews-1]);
		ASSERT_VALID(pView);
		return pView;
	}

	// Otherwise walk through list of views
	for (int i = nViews - 1; i >= 0; i--)
	{
		CView* pView = STATIC_DOWNCAST(CView, m_aCurrentViews[i]);
		if (pviewCurrent == pView)
		{
			// If already at beginning of list, return -1
			if (i == 0)
				return (CView*) -1;
//				return (CViewEx*) -1;
			else
			{
				// Return pointer to previous view in sequence
				CView* pPrevView = STATIC_DOWNCAST(CView, m_aCurrentViews[i-1]);
				ASSERT_VALID(pPrevView);
				return pPrevView;
			}				
		}
	}
	// Current view was not found in list
	return NULL;
}






void CViewTabs::OnTabMerge()
{
	// we need to merge the selected tab with the current tab

	// if no splitter window exists,
	// create one
	// assign the viewid of the current tab to the first pane
	// assign the viewid of the rclicked tab to the second pane
	// show the view associated with the first pane
	// show the view associated with the second pane

	// if a splitter window already exists,
	// create a new one with one more pane
	// copy the viewid's and view windows to the new splitter
	// assign the viewid of the rclicked tab to the last pane
	// load the view associated with the second pane
/*
	if (m_pSplitter == 0)
	{

		// Create splitter window of 2 rows and 1 column
		//, actually might need more than 2 rows! need to check
		// array attached to new tab
		m_pSplitter = new CSplitterWndEx;
		if (!m_pSplitter->CreateStatic(this, 2, 1, WS_CHILD | WS_BORDER))
		{
			TRACE("Failed to create nested splitter\n");
			return;
		}
		
		// Add the current view as a child of the splitter
		m_pSplitter->SetPane(0, 0, pviewCurrent);

		// Add the new view(s) also as child of splitter
		// view may or may not be loaded already
		// may be more than one view in the tab also
		// need to get its array and walk through it
//		m_pSplitter->SetPane(1, 0, m_pNewView);
//		CView* pNewView = ShowView(lngNewViewID);

		// Position the new splitter window
		CRect r;
		CWnd::GetClientRect(r);
		m_pSplitter->MoveWindow(r, FALSE);

		// Show the splitter window and child windows
		m_pSplitter->ShowWindow(SW_SHOW);

		// Delete the tab containing the view
		// This redraws the tab control
//		ASSERT(m_nTabRClick >= 0);
//		m_tbc.DeleteItem(m_nTabRClick);

	}
	else
	{
*/
  /*
		// Add selected view to current splitter control
		int nRows = m_pSplitter->GetRowCount();
		nRows++;
		// Create new splitter window
		CSplitterWnd* pNewSplitter = new CSplitterWndEx;
		if (!pNewSplitter->CreateStatic(this, nRows, 1, WS_CHILD | WS_BORDER))
		{
			TRACE("Failed to create new splitter\n");
			return;
		}
		// Now move the views in the current splitter to the new splitter
		int i;
		for (i = 0; i < nRows; i++)
		{
			CWnd* pViewWnd = m_pSplitter->GetPane(i, 0);
			//, need this method in the SplitterEx class
			// otherwise would need to use CreateView and recreate the whole view window
//			pNewSplitter->SetPane(i, 0, pView);
		}
		// If the window is the parent of any windows, these child windows are automatically 
		// destroyed when the parent window is destroyed. The DestroyWindow member 
		// function destroys child windows first and then the window itself. 
		// If the CWnd being destroyed is a child window and does not have the 
		// WS_EX_NOPARENTNOTIFY style set, then theWM_PARENTNOTIFY message is sent 
		// to the parent.
		m_pSplitter->DestroyWindow();
	}
*/

/*
	CRuntimeClass* pViewClass = RUNTIME_CLASS(CViewContents);

	// Create a Context object which will associate the newly created View with the Document
	CCreateContext cc;
	cc.m_pCurrentDoc = pDoc;
	cc.m_pCurrentFrame = 0;
	cc.m_pNewViewClass = pViewClass;
	cc.m_pNewDocTemplate = pDoc->GetDocTemplate();

	// Now create the Views inside the splitter
	if (!m_pSplitter->CreateView(0, 0, pViewClass, CSize(100, 200), &cc))
	{
		TRACE("Failed to create view\n");
		return;
	}
*/

/*
	// CPaneContainerView is used to control the right pane that CMainFrame
	// sets up. In this case we create a second splitter window.
	m_wndSplitter.CreateStatic(this, 1,2);

	// The context information is passed on from the framework
	CCreateContext *pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;

	// Create two views
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CView1),CSize(150,0), pContext);
	m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CView2),CSize(0,0), pContext);
*/



}



// Splitter was resized so need to store new proportional height in array and set flag.
// Note: This is a method called by the splitter window, not a normal message handler.
void CViewTabs::OnSplitterResized()
{
	xTRACE("CViewTabs::OnSplitterResized - set heights of panes as pct of total height\n");

	// Get current tab and splitter window (if any)
	int nTab = m_tbc.GetCurSel();
	CSplitterWndEx* psw = DYNAMIC_DOWNCAST(CSplitterWndEx, m_Splitters[nTab]);
	if (psw)
	{
		ASSERT_VALID(psw);
		
		// For v1, just resize first pane
//		if (psw->GetRowCount() > 1)
//			psw->SetRowHeightPct(0, m_bytViewHeight, TRUE);

		// Need to walk through all panes of the current tab and resize them
		int nViews = psw->GetRowCount();
		if (nViews > 1)
		{
			for (int nView = 0; nView < nViews; nView++)
			{
				//, for v1, get first pane height from bobject
				if (nView == 0)
					psw->SetRowHeightPct(0, m_bytViewHeight, TRUE);
				else
				{
					int nHeightPct = m_pdatViews->GetViewHeight(nTab, nView);
					psw->SetRowHeightPct(nView, nHeightPct, FALSE);
				}
			}
		}
	}
}


// Pane in splitter was resized so need to store new proportional height in array and set flag.
// Note: This is a method called by the splitter window, not a normal message handler.
void CViewTabs::OnSplitterPaneResized()
{
	// Get current tab and splitter window
	int nTab = m_tbc.GetCurSel();
	CSplitterWndEx* psw = DYNAMIC_DOWNCAST(CSplitterWndEx, m_Splitters[nTab]);
	ASSERT_VALID(psw);

	//, Get height of first pane (that's all we need for v1)
	m_bytViewHeight = psw->GetRowHeightPct(0);

	// Create copy of viewdata so we can modify it, if haven't already done so
	PrepareToModifyViews();

	// Walk through panes and get current heights
	int nViews = psw->GetRowCount();
	for (int nView = 0; nView < nViews; nView++)
	{
		int nHeightPct = psw->GetRowHeightPct(nView);
		m_pdatViews->SetViewHeight(nTab, nView, nHeightPct);
	}

	// Set flag
	m_bViewsChanged = TRUE;
}










void CViewTabs::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	xTRACE("CViewTabs::OnActivateView bActivate %d\n", bActivate);
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	// If this view is being activated, set the focus to the child view (either splitter window or view)
	if (bActivate) 
	{
		xTRACE("  activate first view (splitter or view)\n");
		ActivateFirstView();
	}
}




void CViewTabs::OnSetFocus(CWnd* pOldWnd) 
{
	xTRACE("CViewTabs::OnSetFocus - activate first view\n");

	CViewEx::OnSetFocus(pOldWnd);

//	if (pviewCurrent)
//	{
//		ASSERT_VALID(pviewCurrent);
//		pviewCurrent->SetFocus();
//	}

	ActivateFirstView();
}



void CViewTabs::OnContextMenu(CWnd* pWnd, CPoint point) 
{
//	xTRACE("CViewTabs OnContextMenu %d  %d\n", ptScreen.x, ptScreen.y);
	CViewEx::OnContextMenu(pWnd, point);	
}



// User right clicked on a tab - bring up popup
void CViewTabs::OnTabRightClick(NMHDR* pNotify, LRESULT* result)
{
/*
	// Exit if we're on the Home Object
	//, allow view changes in v2?
	if (m_pDoc->GetCurrentObject()->GetObjectID() == rootUser)
	{
		AfxMessageBox("The Home Folder views cannot be modified.", MB_ICONINFORMATION);
		return;
	}

	CPoint ptScreen(::GetMessagePos()); // Get position of message source

	// Get current tab
	int nTab = m_tbc.GetCurSel();

	// Get tab that was right clicked on
	CPoint ptClient = ptScreen;
	CWnd::ScreenToClient(&ptClient); 
	TCHITTESTINFO tci;
	tci.pt = ptClient;
	int nTabRClick = m_tbc.HitTest(&tci);

	// Select the tab so popup menu will make sense
	SelectTab(nTabRClick);

	// Save this to module variable
	m_nTabRClick = nTabRClick;

	// Display pop up menu
	CMenu menu;
	if (menu.LoadMenu(IDR_POPUP_TAB))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		if (pPopup)
		{
			//, Get dialog working eventually, add main menu item
			pPopup->RemoveMenu(ID_VIEW_EDIT, MF_BYCOMMAND);

			//, Disable "Hide This Tab" if it's the only tab!
			//, also if it's the home object?
			//, just hide it for now
//			if (m_tbc.GetItemCount() < 2)
//				pPopup->EnableMenuItem(ID_TAB_HIDE, MF_BYCOMMAND | MF_GRAYED);
			pPopup->RemoveMenu(ID_TAB_HIDE, MF_BYCOMMAND);

			// "Hide Home View"
			// ----
			// "Show Contents View"
			// "Show Text View"
			// -----
			// Cancel

			// Add show menu items for all views available
			BObject* pobjViews = m_pDoc->GetObject(folderViews);
			ASSERT_VALID(pobjViews);
			BObjects* paChildren = pobjViews->m_paChildren->CreateCopy();
			ASSERT_VALID(paChildren);
			paChildren->Sort(propName);

			int nViews = paChildren->GetSize();
			int nAdded = 0;
			for (int i = 0; i < nViews; i++)
			{
				BObject* pobjView = STATIC_DOWNCAST(BObject, paChildren->GetAt(i));
				ASSERT_VALID(pobjView);
				if (!(pobjView->GetFlag(theApp.m_lngExcludeFlags)))
				{
					CString strName = CString("Show ") + pobjView->GetPropertyText(propName) + CString(" View");
					ULONG lngViewID = pobjView->GetObjectID();
					//, make macros to convert back and forth!
					UINT nCommandID = lngViewID - viewData + ID_VIEW_DATA;
					pPopup->InsertMenu(nAdded, MF_BYPOSITION | MF_STRING, nCommandID, (LPCTSTR) strName);

					// Set checkmark next to currently visible views
//					int nTab = m_pdatViews->GetViewTab(lngViewID);
//					BOOL bVisible = (nTab == m_nTabRClick);
//					if (bVisible)
//						pPopup->CheckMenuItem(nItem, MF_BYPOSITION | MF_CHECKED);

					nAdded++;
				}
			}
			delete paChildren;
			
			// Add separator if items were added
			if (nAdded > 0)
				pPopup->InsertMenu(nAdded, MF_BYPOSITION | MF_SEPARATOR);

			// Add "Hide" menu items for each view visible on this tab, and remove the associated
			// "Show" menu items.
			// Don't do this though if there's only one view available, as each tab must have one view at least.
			int nIndex = m_pdatViews->GetTabIndex(m_nTabRClick);
			nViews = m_pdatViews->GetViewCount(m_nTabRClick);
			if (nViews > 1)
			{
				nAdded = 0; // reset counter
				for (i = 0; i < nViews; i++)
				{
					ViewInfo& rvi = m_pdatViews->GetViewInfo(nIndex + i);
					ULONG lngViewID = rvi.m_lngViewID;
					// Don't let user hide Home View!
					//, kinda kludgy - could use bobject flags - make home view hidden?
					if (lngViewID != viewHome)
					{
						ASSERT(lngViewID);
						BObject* pobjView = m_pDoc->GetObject(lngViewID);
						ASSERT_VALID(pobjView);
						CString strName = CString("Hide ") + pobjView->GetPropertyText(propName) + CString(" View");
						//, make macros to convert back and forth!
						UINT nCommandID = lngViewID - viewData + ID_VIEW_DATA;
						// Remove the show version for this view
						pPopup->RemoveMenu(nCommandID, MF_BYCOMMAND);
						// Add the hide version for this view
						pPopup->InsertMenu(nAdded, MF_BYPOSITION | MF_STRING, nCommandID, (LPCTSTR) strName);
						nAdded++;
					}
				}

				// Now add a separator after the "Hide" if items were added and next item is not a separator
				if (nAdded > 0)
				{
					UINT nState = pPopup->GetMenuState(nAdded, MF_BYPOSITION);
					// Bug: If you don't wrap the & in parentheses, the == takes precedence!!!!!!!!!!!!!!!!!!!!
//					if (nState & MF_SEPARATOR == 0)
					if ((nState & MF_SEPARATOR) == 0)
						pPopup->InsertMenu(nAdded, MF_BYPOSITION | MF_SEPARATOR);
				}
			}


			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this);
		}
	}

	*result = 0;
*/


/*
	//, commented out for v1

	// Only want Merge Tab to be enabled if user clicked on tab other than the active one

	CPoint ptScreen(::GetMessagePos()); // Get position of message source

	// Get current tab
	int nTab = m_tbc.GetCurSel();

	// Get tab that was right clicked on
	CPoint ptClient = ptScreen;
	CWnd::ScreenToClient(&ptClient); 
	TCHITTESTINFO tci;
	tci.pt = ptClient;
	int nTabRClick = m_tbc.HitTest(&tci);

	// Save this to module variable
	m_nTabRClick = nTabRClick;

	// If tabs are the same (ie right clicked on current tab), disable the Merge Tab item
	UINT nFlags = MF_STRING;
	if (nTab == nTabRClick)
		nFlags |= MF_GRAYED;

	// Create and show popup menu
	//, get string from resource at least
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(nFlags, ID_TAB_MERGE, _T("&Merge Tab"));
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptScreen.x, ptScreen.y, this, NULL);
	*result = 0;
*/

}


void CViewTabs::OnViewEdit() 
{
	// Bring up dialog with available views for this tab, let user modify and reorder.
	
	// maybe the dialog should let them arrange the entire view arrangement, i.e.,
	// this would be the BDataViews->UIEditValue method?
	// dialog would be more complicated though and might be confusing.

	PrepareToModifyViews();

	CDialogViews dlg;
	dlg.m_pdatViews = m_pdatViews;
	dlg.m_nTab = m_nTabRClick; // tell dialog which tab to edit
	if (IDOK == dlg.DoModal())
	{
		// Note: If user hits okay, dialog will copy new settings to m_pdatViews!
		ViewsModified();
	}
}



// Prompt user and then remove this tab from the m_pdatViews and refresh display.
//. this is not implemented yet
void CViewTabs::OnTabHide() 
{
	BObject* pobj = m_pDoc->GetCurrentObject();
	ASSERT_VALID(pobj);
	CString strClassName = pobj->GetPropertyText(propClassName);

	CString strMsg;
	strMsg.Format("This will hide this tab and any views it contains for this object and all other objects of the same class (%s). Continue?",
							(LPCTSTR) strClassName);
	if (IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION + MB_YESNO))
	{
		//. modify m_pdatViews here - remove views in this tab
		PrepareToModifyViews();
		ViewsModified();
	}
	
}




// Get the currently active view
CViewEx* CViewTabs::GetCurrentView()
{
	return GetView(m_lngCurrentViewID);	
}




// Toggle display of the specified view
void CViewTabs::OnCmdViewID(UINT nCommandID) 
{
	ASSERT_VALID(m_pdatViews);
	ULONG lngViewID = nCommandID - ID_VIEW_DATA + viewData;
	
	// See if view is visible in the current tab, and remove or add accordingly
	int nTab = m_pdatViews->GetViewTab(lngViewID);
	int nCurrentTab = m_tbc.GetCurSel();
	BOOL bVisible = (nTab == nCurrentTab);
	if (bVisible)
	{
		//, Check if removal is valid (must have at least one text and props view)
		RemoveView(lngViewID);
	}
	else
	{
		AddView(lngViewID);
	}
}


//, don't need this yet as views aren't in main menu
void CViewTabs::OnUpdateViewID(CCmdUI* pCmdUI) 
{
}



// Remove a view from the current views, and set flag.
BOOL CViewTabs::RemoveView(ULONG lngViewID)
{
	ASSERT_VALID(m_pdatViews);

	// Search through bdata object and remove view
	PrepareToModifyViews();
	if (m_pdatViews->RemoveView(lngViewID))
	{
		AdjustViewHeights();
		ViewsModified();
		return TRUE;
	}
	return FALSE;	
}


// Add the specified view to the bottom of the list of views for the current tab
BOOL CViewTabs::AddView(ULONG lngViewID)
{
	ASSERT_VALID(m_pdatViews);

	int nTab = m_tbc.GetCurSel();
	int nView = m_pdatViews->GetViewCount(nTab); // add to end
	int nHeight = 100;
	
	// Add new view to end of list
	PrepareToModifyViews();
	if (m_pdatViews->InsertView(nTab, nView, lngViewID, nHeight))
	{
		AdjustViewHeights(); // equalize heights
		ViewsModified();
		return TRUE;
	}
	return FALSE;	
}



// Move the specified view up relative to siblings
BOOL CViewTabs::MoveViewUp(ULONG lngViewID)
{
	ASSERT_VALID(m_pdatViews);
	PrepareToModifyViews();
	if (m_pdatViews->MoveViewUp(lngViewID))
	{
		ViewsModified();
		return TRUE;
	}
	return FALSE;	
}



// Copy m_pdatViewss to new memory block so we can modify it
void CViewTabs::PrepareToModifyViews()
{
	//, invoke copy constructor to create copy of the array
	// ie given a pointer to another array, the copy constructor will initialize the new array to the same
	// length, and create copies of its members
	// ie it will walk through each item, calling its copy constructor to create new elements which
	// will be added to the new array
	if (!m_bViewsCopied)
	{
		m_pdatViews = STATIC_DOWNCAST(BDataViews, m_pdatViews->CreateCopy()); 
		// Set flag
		m_bViewsCopied = TRUE;
	}
}


// View arrangement was modified, so set flag and update display
void CViewTabs::ViewsModified()
{
	// Set flag so changes will be saved in OnUpdate hintSave handler
	m_bViewsChanged = TRUE;

	// Also set document modified flag
	m_pDoc->SetModifiedFlag();

	// Save current tab selected
	int nTab = m_tbc.GetCurSel();

	// Refresh this view
	UpdateTabs();
	
	// Reselect current tab
	SelectTab(nTab);
}


// Delete view info if it was copied but not saved to a bobject
void CViewTabs::DeleteCopiedViewInfo()
{
	// If m_pdatViews was copied but not saved, delete it now
	if (m_bViewsCopied && !m_bViewsSaved)
	{
		ASSERT_VALID(m_pdatViews);
		delete m_pdatViews;
	}
}



// Adjust the heights of all views on the current tab, setting them all equal.
// This is called after adding or removing a view.
BOOL CViewTabs::AdjustViewHeights()
{
	int nTab = m_tbc.GetCurSel();
	int nViews = m_pdatViews->GetViewCount(nTab);
	if (nViews > 0)
	{
		int nHeightPct = 100 / nViews;
		for (int nView = 0; nView < nViews; nView++)
		{
			m_pdatViews->SetViewHeight(nTab, nView, nHeightPct);
		}
		m_bytViewHeight = nHeightPct;
	}
	return TRUE;
}





// Load the views associated with the specified object
BOOL CViewTabs::LoadViews(BObject *pobj)
{
	// Delete copied view info if it was not saved
	DeleteCopiedViewInfo();

	// Get list of tabs and views available from object or its classdef
	//! bug: bombing on the assertion below. probably null.
	// so no view arrangement was assigned to the subfolder. 
	// where does that normally happen? 
	// really odd that it would bomb here - it should be looking up the class chain
	// for the default value. so it should never return null!
	// unless something bad happened with the folder object
	// or there's a bug in the code. ?
	m_pdatViews = STATIC_DOWNCAST(BDataViews, pobj->GetPropertyData(propViewArrangement));
	ASSERT_VALID(m_pdatViews);

	//, Get height of first pane (v1 only)
	m_bytViewHeight = pobj->GetViewHeight();

	// Clear flags
	m_bViewsCopied = FALSE;
	m_bViewsChanged = FALSE;
	m_bViewsSaved = FALSE;

	// Modify tab control to correspond with list of available views
	// Note: This just adds the tabs, not the actual views
	UpdateTabs();

	// Save heights of splitter panes to array
	// this could be in any tab
	// would need one for each tab
	// why not just copy the bdata, and compare that on hintSave?
	// yeah that's the ticket

	// Keep same view current, if possible, or use the default (viewText)
	// If view is not available, select the first tab instead
	if (m_lngCurrentViewID == 0) m_lngCurrentViewID = viewText;
	if (!SelectView(m_lngCurrentViewID))
		SelectTab(0);

	return TRUE;
}





// Check if the specified view is visible or not
BOOL CViewTabs::IsViewVisible(ULONG lngViewID)
{
	CView* pview = GetView(lngViewID);
	if (pview)
	{
		return pview->IsWindowVisible();
	}
	return FALSE;
}




