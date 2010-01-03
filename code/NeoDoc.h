
// CNeoDoc
// This class encapsulates all code required to store the actual 
// document information in memory. Handles reading and writing 
// to/from file, modifying memory structures based on user interaction, etc.
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------

// And if UI doesn't handle a command, MFC will pass it to the document, 
// right? ugh, that seems kind of weird. 



#pragma once


#include "BDataColumns.h"
#include "BDataLink.h"
#include "BDataFlags.h"
#include "BDataViews.h"
#include "BObject.h"


#include "FileName.h"
#include "FileFormats.h"
#include "History.h"

class CIconCache;
class CFrameChild;


// Map from ULONG to BObject*
typedef CMap<ULONG, ULONG, BObject*, BObject*> CMapObjects;


// versionDataModel: version of the classdefs, propertydefs, viewid's, etc. which make up the 
// system folder contents.
// This should correspond more or less with the release version of the program, though
// some releases might not make any changes to the data model.
// But for instance, if we add a new class (eg Graph Paper), and properties for it, and a view class for it,
// then we need to update this value.
// You can update the data model without affecting the file structure.
// When the data model changes, you want to update users' existing files also.
// To do this, we store the datamodel version in each document,
// and on loading, compare this with the latest version available (ie the version of the template file 
// that shipped with the release).
// If it needs to be updated, we'll either modify the file through code, or
// open the new template file as a separate pDoc, walk through it and the existing file, 
// adding, editing, or deleting where necessary.
// ie just synchronizing them
// use the subversion number if wind up needing to make more changes to template
// file before actual release. 
// yes, it's all a pain...
//   900 corresponds to 0.9.00 (ie 0.9)
// 1000 corresponds to 1.0.00 (ie 1.0)
// 1001 corresponds to 1.0.01 (ie 1.0a)
// 110102 corresponds to 1.1.01 (ie 1.1a) build 02 
// Use the build number to force updates to stuff.neo, example.neo etc when adding things to template file. 
//const ULONG versionDataModel = 0900; // version 0.9
//const ULONG versionDataModel = 1000; // version 1.0 (didn't assign a release letter to this, but should have)
//const ULONG versionDataModel = 1100; // version 1.1
//const ULONG versionDataModel = 1101; // version 1.1a
//const ULONG versionDataModel = 110101; // version 1.1a subversion 1
//const ULONG versionDataModel = 110102; // version 1.1a subversion 2
//const ULONG versionDataModel = 120101; // version 1.2a subversion 1
const ULONG versionDataModel = 120102; // version 1.2a subversion 2
// See also: versionFileStructure



class CNeoDoc : public CDocument
{
	DECLARE_DYNCREATE(CNeoDoc)

// Construction
public:
	CNeoDoc();
	virtual ~CNeoDoc();


// enums
public:
	enum EncryptionTypes {encryptNone = 0, encryptRC4 = 1};
	enum CompressionTypes {compressNone = 0, compressLZH = 1};

// static (class) methods
public:
	static CNeoDoc* GetDoc(); // class method used to get pointer to current document object


// Operations
public:
	BObject* AddObject(BObject* pobjParent, const ULONG lngClassID, const CString& strText, ULONG lngObjectID = 0, ULONG lngIconID = 0, ULONG lngFlags = 0);
	BOOL AddObjectToIndex(ULONG lngObjectID, BObject* pobj);
	BData* CreateBData(ULONG lngClassOrPropertyID);
	BData* CreateBDataFromPropertyType(ULONG lngPropertyTypeID);
	BOOL CreateTemplate();
	BOOL DoFileAutoRecoverSave();
	int FindReferences(BObject* pobjFind, CObArray& aRefs, BObject* pobjStart = 0, BOOL bRecurse = TRUE);
	BObject* GetCurrentObject();
	int GetEncryptionType() { return m_nEncryptionType; }; // inline
	CString GetFileSizeString();
	int GetIconIndex(ULONG lngIconID);
	CImageList* GetImageList();
	CFrameChild* GetMDIChildFrame();
	CString GetModifiedName(LPCTSTR szFileName, LPCTSTR szAppendText);
	ULONG GetNextObjectID(); // get next available objectid
	CString GetNumberOfObjectsString();
	BObject* GetObject(ULONG lngObjectID);
	int GetProperties(BDataLink& datProps, BObject *pobj = NULL);
//	BObject* GetRoot(); // get the root object of the document
	BObject* GetRoot() { return m_pobjRoot; }; // inline - get the root object of the document
	ULONG GetSplitterPos() { return m_lngSplitterPos; }; // inline
	BObject* GetTargetObject();
	BOOL IsBObjectValid(BObject* pobj);
	BOOL IsTargetSingle();
	BOOL SaveModifiedBackup();
	int SearchForText(BObject* pobjStart, ULONG lngPropertyID, CString strFindText, BObjects& aResults, 
					ULONG lngExcludeFlags = 0, BOOL bMatchCase = FALSE, BOOL bWholeWord = FALSE,
					BOOL bSearchStartObject = FALSE, BOOL bOriginalCall = TRUE);
	void SetCurrentObject(BObject* pobjCurrent, CView* pSender = 0, BOOL bNavigating = FALSE);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	void SetRoot(BObject* pobj);
	void SetTargetObject(BObject* pobj);
	void UpdateAllViewsEx(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL);
	void UpdateDocument(BObject* pobj);


	// User-interface routines - generally bring up a dialog to get 
	// information from user.
	//, which is not good - ideally would have callbacks to ui layer. 
	BObject* UIAddNewClass();
	BObject* UIAddNewFolder(BObject* pobjParent = 0, BOOL bSelectNewObject = TRUE);
	BObject* UIAddNewObject(BObject* pobjParent = 0, BOOL bSelectNewObject = TRUE); 
	BObject* UIAddNewPropertyDef();
	BOOL UIChangeClassContents(BObject* pobj);
	BOOL UIChangeClassIcon(BObject* pobj);
	BOOL UIChangeObjectClass(BObject* pobj);
	BOOL UIChangeObjectContents(BObject* pobj);
	BOOL UIChangeObjectIcon(BObject* pobj);
	BOOL UIDeleteObject(BObject* pobj, BOOL bQuiet = FALSE);
	BOOL UIDeleteObjects(BObjects* paObjects, BOOL bOriginalCall = TRUE, BOOL bQuiet = FALSE);
	BOOL UIEditClass(BObject* pobjClass);
	BOOL UIEditObject(BObject* pobj);
	BOOL UIEditPropertyDef(BObject* pobjPropertyDef);
//	void Export(BDataLink& datLink);
//	void Export(BObject* pobj, BOOL bRecurse, BOOL bSystem, CString strFormat, CFilename strFilename);
	void Export(BObject* pobj, BOOL bRecurse, BOOL bSystem, eFileFormat nFormat, CFilename strFilename);
	void Import();
	BObject* UIImportIcon();
	BObject* UIMoveObjectTo();
	BOOL UIRenameObject(BObject* pobj);
	void UISetEncryption();
	void UITest();


// Attributes
private:
	// These get serialized
	BOOL m_bUnicode; // If true then document is/was serialized using unicode characters rather than ansi
	int m_nEncryptionType; // whether file is encrypted, how it's encrypted, etc.
	CString m_strSessionKeyHash; // hash of session key based on password (hexascii string) - used to verify password
	BObject* m_pobjRoot;			// Pointer to the root object, allocated on the heap
	BObject* m_pobjCurrent;		// Pointer to currently selected object
	ULONG m_lngSplitterPos; // Splitter position
	CStringList m_lstSearches; // List of recently performed searches
	ULONG m_lngNextObjectID; // Next available ObjectID
public: //, make private
	int m_nVersionFileStructure; // Version of physical file structure - where variables are expected to be, etc.
	int m_nVersionDataModel; // Version of system data model - which bobjects are expected to exist, etc.
private:
	ULONG m_lngDefaultFolderLocationID;			// Default location to add new folders
	ULONG m_lngStartObjectID; // Object to select on start

public: // leave these public?
	BDataViews m_datViewsLeft; // Tab/view arrangement for left side
	BDataColumns m_datColumnsSearchView; // Column arrangement for search view
	BDataColumns m_datColumnsIndexView; // Column arrangement for index view
	BDataColumns m_datColumnsPropertyView; // Column arrangement for property view
	BDataColumns m_datColumnsClassFolder; // Column arrangement for class folder
	BDataColumns m_datColumnsPropertyFolder; // Column arrangement for property folder
	BDataColumns m_datColumnsIconFolder; // Column arrangement for icon folder
	BDataColumns m_datColumnsViewFolder; // Column arrangement for views folder
	BDataColumns m_datColumnsDefault; // Column arrangement (Name, Class) //, not serialized yet

private:
	CHistory m_objHistory; // History of objects selected

public:

	// These don't get serialized
	CString m_strPassword; // password for encrypted file
	CMapObjects m_mapObjects;  // Map from ObjectID to BObject pointer
	CIconCache* m_pIconCache;	// Icon cache (encapsulates an image list)
	BData* m_pdatTemp; // Temporary bdata object used for F4ing pseudo properties
	CString m_strEmpty;		// An empty string used to return ""
	CString m_strTextCache; // Temporary string used by all BData objects in GetText method
	BDataFlags m_datFlagsTemp; // Temporary BDataFlags object used to get string version of m_lngFlags property
	BOOL m_bSettingCurrentObject; // Flag used by SetCurrentObject routine
	BOOL m_bAutoRecoverDirty; // Flag used to tell if file has changed since last AutoRecover save
	BOOL m_bUpgradeFileStructure; // True if file structure needs to be upgraded during Load
	BOOL m_bUpgradeDataModel; // True if data model needs to be upgraded during Load
	BOOL m_bLoadingFile; // Set to True during file load

	// Used to update progress bar during serialization
	ULONG m_nObject;
	ULONG m_nObjects; 


// Implementation
private:
	BOOL OnOpenDocumentEx(LPCTSTR lpszPathName);
	BOOL OnSaveDocumentEx(LPCTSTR lpszPathName);

//	BDataLink m_datTarget; // Object or objects which will be acted on by the object command handlers
	BObject* m_pobjTarget; // Object which will be acted on by the object command handlers

	void UpgradeFile(CArchive& ar);
	void Synchronize(CNeoDoc* pdocTemplate);
	void SynchronizeDelete(BObject* pobjThis, CNeoDoc* pdocTemplate);
	void SynchronizeRecurse(BObject* pobjTemplate);
	void SynchronizeRecurseProps(BObject* pobjTemplate);

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNeoDoc)
	public:
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void DeleteContents();
	virtual void OnCloseDocument();
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

	// Advanced Overridables
	virtual void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault);
	virtual BOOL DoFileSave();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	public:
	virtual CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError);

#ifdef _DEBUG
public:
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CNeoDoc)
	afx_msg void OnCmdFileProperties();
	afx_msg void OnCmdEditClasses();
	afx_msg void OnCmdPropertyWizard();
	afx_msg void OnCmdFileDeleteAll();
	afx_msg void OnNavigateBack();
	afx_msg void OnNavigateForward();
	afx_msg void OnUpdateNavigateBack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNavigateForward(CCmdUI* pCmdUI);
	afx_msg void OnObjEditInDialog();
	afx_msg void OnUpdateObjEditInDialog(CCmdUI* pCmdUI);
	afx_msg void OnObjAdd(); //.
//	afx_msg void OnObjMoveTo();
//	afx_msg void OnUpdateObjMoveTo(CCmdUI* pCmdUI);
	afx_msg void OnObjProperties();
	afx_msg void OnUpdateObjProperties(CCmdUI* pCmdUI);
	afx_msg void OnObjPriorityHigh();
	afx_msg void OnObjChangeClassContents();
	afx_msg void OnObjChangeClassIcon();
	afx_msg void OnObjChangeObjectClass();
	afx_msg void OnObjChangeObjectContents();
	afx_msg void OnObjChangeObjectIcon();
	afx_msg void OnObjDelete();
	afx_msg void OnObjMoveDown();
	afx_msg void OnObjMoveUp();
	afx_msg void OnObjOpen();
	afx_msg void OnObjAutosort();
	afx_msg void OnUpdateObjMoveDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjMoveUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateObjAutosort(CCmdUI* pCmdUI);
	afx_msg void OnObjSortChildren();
	afx_msg void OnUpdateObjSortChildren(CCmdUI* pCmdUI);
	afx_msg void OnViewToggleContents();
	afx_msg void OnUpdateViewToggleContents(CCmdUI* pCmdUI);
	afx_msg void OnObjAddFolder();
	afx_msg void OnNavigateGoto();
	afx_msg void OnFileSecurity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





