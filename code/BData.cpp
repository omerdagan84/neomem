
// BData
 

#include "precompiled.h"

#include "BData.h"
#include "ConstantsDatabase.h"

//, bleh
//x#include "BDataColumns.h" 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




// Special macro for virtual base class
// got this from FAQ in msdn library help
#define IMPLEMENT_SERIAL_ABC(class_name, base_class_name, wSchema) \
_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, NULL) \
CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb) \
{ \
	pOb = (class_name*) ar.ReadObject(RUNTIME_CLASS(class_name)); \
	return ar; \
} \

// compare with normal version:
// removed the CreateObject and _init_ class functions
/*
#define IMPLEMENT_SERIAL(class_name, base_class_name, wSchema) \
CObject* PASCAL class_name::CreateObject() 	{ return new class_name; } \
_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, class_name::CreateObject) \
AFX_CLASSINIT _init_##class_name(RUNTIME_CLASS(class_name)); \
CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb) \
{ \
	pOb = (class_name*) ar.ReadObject(RUNTIME_CLASS(class_name)); \
	return ar; \
} \
*/


//. link - code\BData.cpp(48): warning C4003: not enough actual parameters for macro '_IMPLEMENT_RUNTIMECLASS'
IMPLEMENT_SERIAL_ABC(BData, CObject, VERSIONABLE_SCHEMA | versionFileStructure) // last parameter is version number
// get version with int nFileVersion = ar.GetObjectSchema();



// Construction/Destruction
//--------------------------------------------------------------------

BData::BData() {
}

// IMPORTANT: Delete any memory created on heap in destructor
BData::~BData() {
}



// Create a copy of this BData object.
// Caller will own the new BData object.
// (I prefer making this method explicit, instead of using C++'s
// copy constructor or assignment operator)
BData* BData::CreateCopy() {
	AfxMessageBox(_T("Need to implement CreateCopy for this BData class"));
	return NULL;
}



// Returns True if can find any references to the specified object in this bdata
BOOL BData::FindReferences(BObject* pobjFind) {
	AfxMessageBox(_T("Need FindReferences handler for this property type"));
	return FALSE;
}


ULONG BData::GetMemoryUsed(BOOL bRecursive) {
	return sizeof(BData);
}


CString BData::GetBDataText(BDoc* pDoc, ULONG lngPropertyID, BOOL bMachineVersion) {
	m_strText = "No GetBDataText for this BData class";
	return m_strText;
}


// Parse the given text and store a data representation of it
// Return True if was able to parse text
BOOL BData::SetBDataText(const CString& str, BObject* pobjPropertyDef /* = 0 */, BOOL bShowErrorMessage /* = TRUE */) {
	AfxMessageBox(_T("Need to implement SetBDataText for this BData class!"));
	return FALSE;
}



// Virtual function used to reset cache flag for this bdata object.
// Do nothing in base class.
// If a bdata has a cache flag, override this function and reset it here.
void BData::ResetData() {
}



BOOL BData::ReplaceReferences(BObject* pobjFind, BObject* pobjNew /* = 0 */) {
	return FALSE;
}



BOOL BData::IsValid(BDoc* pDoc) {
	// default: is valid
	return TRUE;
}


void BData::ConvertToSoftLinks() {
}

void BData::ConvertToHardLinks(BDoc* pDoc) {
}


//x
/*
BOOL BData::UIAddMenuItems(CMenu* pMenu, int nPos) {
	// default: do nothing
	return FALSE;
}
*/


// The UIEditValue routine should set the document modified flag
// and return TRUE if value was changed
//BOOL BData::UIEditValue(BObject* pobj, BObject* pobjPropertyDef) {
BOOL BData::UIEditValue(BObject* pobj, BObject* pobjPropertyDef, CUI& ui) {
	AfxMessageBox(_T("Need EditValue handler for this property type"));
	return FALSE;
}


BOOL BData::UIHandleCommand(UINT nCommandID) {
	// default: do nothing
	return FALSE;
}


void BData::UIOnClick() {
	// default: do nothing
}


//x
/*
void BData::UIOnMouseMove() {
	// default: do nothing
}
*/


// This lets the data type control what kind of cursor
// appears over a value in a grid control, for instance. 
// Return a zero for the default cursor (an arrow), 
// return one for a hand.
int BData::UICursorOnMouseover() {
	return 0; // default 
}


//x
/*
//xBDataColumns* BData::ToColumns() {
//xBDataColumns BData::ToColumns() {
BDataColumns& BData::ToColumns() {
	BDataColumns* pcols = DYNAMIC_DOWNCAST(BDataColumns, this);
	ASSERT_VALID(pcols);
//x	BDataColumns cols = *pcols;
//x	return pcols;
	return *pcols;
//x	return cols;
}
*/



// Return an array of popup menu item strings when user right clicks on a cell in a control. 
// Called from CViewContents::OnContextMenu etc
// considered:
// static char* ca[] = {"hi","bye","rye"}; // but would need to return # items also
//, might need to return by value to cross lib boundary? 
CStringArray& BData::UICommands() {
	static CStringArray cmds; // static so can return reference to it
	return cmds;
}





