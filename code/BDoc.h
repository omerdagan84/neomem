
// BDoc
// This class encapsulates all code required to store the actual 
// document information in memory. Handles reading and writing 
// to/from file, modifying memory structures based on user interaction, etc.
// Date: November 1999
//-----------------------------------------------------------------------------------------------------------------

// If UI doesn't handle a command, MFC will pass it to the document.



#pragma once


#include "BDataColumns.h"
#include "BDataLink.h"
#include "BDataFlags.h"
#include "BDataViews.h"
#include "BObject.h"


#include "FileName.h"
#include "FileFormats.h"
#include "History.h"

#include "UI.h"

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



class BDoc : public CDocument {

	DECLARE_DYNCREATE(BDoc)

public:

	// Construction
	BDoc();
	virtual ~BDoc();
	static BDoc& New();
	
	// Enums
	enum EncryptionTypes {encryptNone = 0, encryptRC4 = 1};
	enum CompressionTypes {compressNone = 0, compressLZH = 1};

	// Static (class) methods
	static BDoc* GetDoc(); // class method used to get pointer to current document object


	// Operations
	void AddObject(HOBJECT hobj, CView* pviewIgnore = NULL);
	BOOL AddObjectToIndex(HOBJECT hobj);
	BOOL AddPropertyLink(OBJID id, OBJID idProperty, OBJID idValue);
	BData* CreateBData(OBJID idClassOrProperty);
	BData* CreateBDataFromPropertyType(OBJID idPropertyType);
	BOOL CreateTemplate();
	BOOL DoFileAutoRecoverSave();
	int FindReferences(BObject* pobjFind, CObArray& aRefs, BObject* pobjStart = 0, BOOL bRecurse = TRUE);
	BObject* GetCurrentObject();
	int GetEncryptionType() { return m_nEncryptionType; }; 
	CString GetFileSizeString();
	int GetIconIndex(OBJID idIcon);
	CImageList* GetImageList();
	CFrameChild* GetMDIChildFrame();
	CString GetModifiedName(LPCTSTR szFileName, LPCTSTR szAppendText);
	ULONG GetNextObjectID(); // get next available objectid
	CString GetNumberOfObjectsString();

	//, both these would return copies of objects, so would need to delete them
	BObject* GetObject(OBJID idObject);
	BObject* GetObjectNull(OBJID idObject);
	BObjects* GetObjects(); //, placeholder
	int GetObjects(BObject* pobjStart, OBJID idProperty, CString strFindText, BObjects& aResults, 
					ULONG lngExcludeFlags = 0, BOOL bMatchCase = FALSE, BOOL bWholeWord = FALSE,
					BOOL bSearchStartObject = FALSE, BOOL bOriginalCall = TRUE);

	BObject* GetObject2(OBJID idObject);


	int GetProperties(BDataLink& datProps, BObject *pobj = NULL);
	BObject* GetRoot() { return m_pobjRoot; }; // get the root object of the document
	ULONG GetSplitterPos() { return m_lngSplitterPos; };
	BObject* GetTargetObject();
	int GetVersionFileStructure() { return m_nVersionFileStructure; };
	int GetVersionDataModel() { return m_nVersionDataModel; };
	BOOL IsBObjectValid(BObject* pobj);
	BOOL IsSettingCurrentObject() { return m_bSettingCurrentObject; } ;
	BOOL IsTargetSingle();
	BObject& NewObject(const OBJID idClass, const CString& strName, OBJID idLocation = NULL);
	OBJID NewProperty(const CString& strName, OBJID idPropType, const CString& strDescription);
	void RemoveObjectFromIndex(OBJID idObject);
	void Save();
	BOOL SaveModifiedBackup();
	void SetCurrentObject(BObject* pobjCurrent, CView* pSender = 0, BOOL bNavigating = FALSE);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	void SetRoot(BObject* pobj);
	void SetTargetObject(BObject* pobj);
	void SetVersionDataModel(int n) { m_nVersionDataModel = n; };
	void UpdateAllViewsEx(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL);
	void UpdateDocument(BObject* pobj);


	// User-interface routines - generally bring up a dialog to get 
	// information from user.
	//. ideally should have callbacks to ui layer. 
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
	void Export(BObject* pobj, BOOL bRecurse, BOOL bSystem, eFileFormat nFormat, CFilename strFilename);
	void Import();
	BObject* UIMoveObjectTo();
//x	BOOL UIRenameObject(BObject* pobj);
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
	OBJID m_idNextObject; // Next available ObjectID
	int m_nVersionFileStructure; // Version of physical file structure - where variables are expected to be, etc.
	int m_nVersionDataModel; // Version of system data model - which bobjects are expected to exist, etc.
	OBJID m_idDefaultFolderLocation; // Default location to add new folders
	OBJID m_idStartObject; // Object to select on start

	// These get serialized also - public so View classes can access them
public: 
	BDataViews m_datViewsLeft; // Tab/view arrangement for left side
	BDataColumns m_datColumnsSearchView; // Column arrangement for search view
	BDataColumns m_datColumnsIndexView; // Column arrangement for index view
	BDataColumns m_datColumnsPropertyView; // Column arrangement for property view
	BDataColumns m_datColumnsClassFolder; // Column arrangement for class folder
	BDataColumns m_datColumnsPropertyFolder; // Column arrangement for property folder
	BDataColumns m_datColumnsIconFolder; // Column arrangement for icon folder
	BDataColumns m_datColumnsViewFolder; // Column arrangement for views folder
	BDataColumns m_datColumnsDefault; // Column arrangement (Name, Class) //, not serialized yet

		
// These don't get serialized
private:	
	CHistory m_objHistory; // History of objects selected
	CString m_strPassword; // password for encrypted file
	CIconCache* m_pIconCache;	// Icon cache (encapsulates an image list)
	BOOL m_bAutoRecoverDirty; // Flag used to tell if file has changed since last AutoRecover save
	BOOL m_bUpgradeFileStructure; // True if file structure needs to be upgraded during Load
	BOOL m_bUpgradeDataModel; // True if data model needs to be upgraded during Load
	BOOL m_bLoadingFile; // Set to True during file load
	CMapObjects m_mapObjects;  // Map from ObjectID to BObject pointer
	BOOL m_bSettingCurrentObject; // Flag used by SetCurrentObject routine

public:
	ULONG GetFlags(LPCTSTR strFlags); // convert bdataflags string to number
	LPCTSTR GetFlagText(ULONG lngFlags); // convert bdataflags number to string

	// Implementation
private:
	BObject* m_pobjTarget; // Object which will be acted on by the object command handlers

	BOOL OnOpenDocumentEx(LPCTSTR lpszPathName);
	BOOL OnSaveDocumentEx(LPCTSTR lpszPathName);
	void UpgradeFile(CArchive& ar);
	void Synchronize(BDoc* pdocTemplate);
	void SynchronizeDelete(BObject* pobjThis, BDoc* pdocTemplate);
	void SynchronizeRecurse(BObject* pobjTemplate);
	void SynchronizeRecurseProps(BObject* pobjTemplate);

	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BDoc)
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
protected:
	virtual BOOL DoFileSave();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	virtual void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault);
public:
	virtual CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException* pError);

#ifdef _DEBUG
public:
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	// Message map functions
protected:
	//{{AFX_MSG(BDoc)
	afx_msg void OnCmdFileProperties();
	afx_msg void OnCmdEditClasses();
	afx_msg void OnCmdPropertyWizard();
	afx_msg void OnCmdFileDeleteAll();
	afx_msg void OnFileSave();
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





