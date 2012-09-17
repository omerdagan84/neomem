// Test.cpp: implementation of the CTest class.

#include "precompiled.h"
#include "Test.h"


#include "NeoMem.h"
#include "BDoc.h"
#include "ConstantsDatabase.h" //. should be part of neodoc

//#include "DSource.h"



CTest::CTest() {
}

CTest::~CTest() {
}


//void CTest::TestFolder() {
//}




// Run all Unit Tests
// this is called from InitInstance if in test mode (ie run with /t option).
//. and for now, by ctrl+alt+shift+T command. 
// static
void CTest::DoTests(CNeoMem& app) {

	// must wrap in try/catch because we're not inside a windows message handler!
	// see ProcessWndProcException
	try {
		// each module could have a test routine
		// the test command would know about these and call them explicitly
		// could also do various integrative tests
//		BDoc* pdoc = BDoc::GetDoc();
//		BDataBoolean::Test(pdoc);
//		BDataDate::Test(pdoc);
//		BDataHyperlink::Test(pdoc);
//		BDataLink::Test(pdoc);
//		BDataString::Test(pdoc);
//		BObject::Test(pdoc);
//		CRichEditCtrlEx::Test();


		// maybe simplest to create a new document, 
		// throw ALL commands at it (esp flaky/suspicious ones), 
		// then check that it's in proper state.

		//, this won't be necessary - just create new doc and use it
		app.CloseAllDocuments(FALSE);



		//, switch to this - 
//		BDoc& doc = BDoc::New();
//		BDoc& doc = BDoc::New(pui);

		// if there's app-specific stuff do that separately. 
		//, mfc weird doctemplate stuff in filenew handler? override it and simplify? 
		// but also ties in with mru files etc. 
		app.OnFileNew();
		BDoc* pdoc = BDoc::GetDoc(); // get rid of this fn, if possible? but i think mfc uses it?
		ASSERT_VALID(pdoc);
		BDoc& doc = *pdoc;

		// get pointer to UI, to pass to doc
//		CUI* pui = this;
//		CNeoMem* pui = this;
//		doc.pui = pui;


		//, should take and return handles, not pointers. 
		//, all props should be optional. have a default location, class, name. 

		//, should document really know about current object? no, that's a ui thing. 
		// there could be multiple ui's looking at the db. 
		// it's pretty entwined in BDoc though. 




		// add folder for fish
//		HOBJECT hobjParent = pdoc->GetCurrentObject();
//		HOBJECT hobjFishFolder = pdoc->CreateObject(classFolder, "Fish", hobjParent);
//		pdoc->AddObject(hobjFishFolder); // Add object to database (and tell views)

		// or
		// return id or hobj? 
		// can get hobj from id
		// or viceversa
		// but id is more lasting. mebbe.
		// hobject is opaque - we can make what we want of it. 
		// and not use it as a pointer. 

		// this is an object of class 'folder', not something in the 'class' folder. confusing. 
//		BObject* pobjFishFolder = doc.NewObject(classFolder, "fish");
//		HOBJECT hobjFishFolder = doc.NewObject(classFolder, "fish");
//		OBJID idFishFolder = doc.NewObject(classFolder, "fish");
//		BObject& objFishFolder = doc.NewObject(classFolder, "fish");
//		DObject& dobjFishFolder = ds.NewObject(classFolder, "fish");
//		BObject& objFolder = doc.NewObject(classFolder, "fish");
//		BObject& objFolder = BObject::New(doc);
//		BObject& objFolder = BObject::New(doc, classFolder);









		BObject& objFolder = BObject::New(doc, classFolder, "fish");

		// check class
		//, yuck - classID?
		OBJID idClass = objFolder.GetPropertyLink(propClassID);
		ASSERT(idClass == classFolder);

		// check location
		OBJID idParent = objFolder.GetPropertyLink(propParent);
		ASSERT(idParent == doc.GetCurrentObject()->id);


		// set description
		CString strDesc("fish i'm thinking of getting");
		objFolder.SetPropertyString(propDescription, strDesc);
		CString str = objFolder.GetPropertyString(propDescription);
		ASSERT(str==strDesc);


		// set name
		CString strName("my fish");
		objFolder.SetPropertyString(propName, strName);
		ASSERT(objFolder.GetPropertyString(propName) == strName);




		
		// add a fish class
		BObject& objClass = BObject::New(doc, classClass, "Fish", rootClass);
		OBJID classFish = objClass.id;

		// check location
		OBJID idLoc = objClass.GetPropertyLink(propParent);
		ASSERT(idLoc == rootClass);


		// set description
		{
		CString strDesc("a thing that swims in the water");
		objClass.SetPropertyString(propDescription, strDesc);
		CString str = objClass.GetPropertyString(propDescription);
		ASSERT(str==strDesc);
		}


		// id is a read-only variable!
//		OBJID idfoo = objClass.id;
//		objClass.id = 2; // fails



		// create a fish object, add it to fish folder
//		pdoc->UIAddNewObject(); //, adapt this so can pass params to it...
		BObject& objPlecy = BObject::New(doc, classFish, "plecy", objFolder.id);
		objPlecy.SetPropertyString(propDescription, "plecostomus");



		// set folder default to fish class
		// This will set document modified flag and update views?
//,		pdoc->UIChangeObjectContents(hobjFishClass);
		objFolder.SetPropertyLink(propDefaultClass, classFish);
		{
		OBJID id = objFolder.GetPropertyLink(propDefaultClass);
		ASSERT(id == classFish);
		}



		// add a new property 'price'
//		pdoc->UIAddNewPropertyDef(); //, adapt this
//,		BObject& objPrice = BPropDef::New(doc, "price", proptypeCurrency, "how much it costs");
		BObject& objPrice = BObject::New(doc, classProperty, "price", folderProperties);
		objPrice.SetPropertyLink(propPropertyType, proptypeCurrency);
		objPrice.SetPropertyString(propDescription, "how much it costs");


		// add to fish class
		objClass.SetPropertyLinksAdd(propObjectProperties, objPrice.id);


		// get list of property values
		ObjIDArray a;
		objClass.GetPropertyLinks(propObjectProperties, a);

		// check it
		int x = a.GetSize();
		ASSERT(a.GetSize() == 1);
		ASSERT(a.GetAt(0) == objPrice.id);
//		ASSERT(a.GetAt(1) == objSize.id);


		// add another property (will exercise different code)
		BObject& objSize = BObject::New(doc, classProperty, "size", folderProperties);
		objSize.SetPropertyLink(propPropertyType, proptypeNumber);
		objSize.SetPropertyString(propDescription, "size in inches");

		objClass.SetPropertyLinksAdd(propObjectProperties, objSize.id);

		// check it
		objClass.GetPropertyLinks(propObjectProperties, a);
		ASSERT(a.GetSize() == 2);
		ASSERT(a.GetAt(0) == objPrice.id);
		ASSERT(a.GetAt(1) == objSize.id);


		// set plecy's price
		objPlecy.SetPropertyString(objPrice.id, "$1.34");


		// select the fish folder
		doc.SetCurrentObject(&objFolder);


		// add another fish
		BObject& objGlassfish = BObject::New(doc, classFish, "glassfish", objFolder.id);

		// convert the price prop to a string
//		objPrice.SetPropertyData(propPropType, proptypeString);
//		objGlassfish.SetPropertyString(objPrice.id, "$2.54");



		// import a new icon

		// get Test folder with .ico files
		CStringEx strTestFolder = theApp.m_strApplicationFolder + "\\..\\test\\files\\";

		HOBJECT hobjIcon = NULL;
//		hobjIcon = pdoc->UIImportIcon(gpgui, "foo.ico"); // nonexistent file
//		hobjIcon = pdoc->UIImportIcon(gpgui, "neomem.cnt"); // bad file
//		hobjIcon = pdoc->UIImportIcon(gpgui, strTestFolder + "Fish.ico");
//		hobjIcon = doc.UIImportIcon(gpgui, strTestFolder + "Fish.ico", "Fish");
//		hobjIcon = pdoc->UIImportIcon(gpgui);

		// attach the icon to fish class
//		objFishClass.SetPropertyLink(propClassDefIcon
//		objFishClass.SetIconID(hobjIcon->id); // is this right? 
		// apparently. but maybe a command would be better. 
		// eg hobjFishClass->SetObjectIcon or something. 
		// ie something to mimic the user command. 

		// pass it the ui, which is like the callback object. 
		// it'll call the ui to get info from the user. 
		// the ui interface will be defined in the database module.
		// the ui will need to implement that interface. 
		// ie derive a class from it, and pass an object of that class to the db fns. 
		// like this one!
//,		pdoc->UIAddNewObject2(gpgui);




/*
		// add price to the contents view
//		nCol = GetColumnCount();
//		m_pdatColumns->InsertColumn(lngPropertyID, m_pDoc, 0, nCol);
		// remove a column
//		BObject* pobjProp = m_pDoc->GetObject(lngPropID);
//		m_pdatColumns->RemoveColumn(nCol);

		CFrameChild* pui = theApp.GetChildFrame();
		CViewContents* pview = DYNAMIC_DOWNCAST(CViewContents, pui->GetView(viewContents));
		ASSERT_VALID(pview);
		pview->m_lvw.InsertColumnAsk(objPrice, 2);

		objFolder.GetPropertyLinks(propColumns, a);
*/
		




		//, pass filename here
		//, pass pui for interactive / saveas
//		doc.Save();




	}
	catch (CException* e) {
		//?
		const MSG msg = {0};
		AfxGetThread()->ProcessWndProcException(e, &msg);
	}


//, beeps too loud		AfxMessageBox("All tests done.");

}




