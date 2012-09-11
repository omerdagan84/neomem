// Test.cpp: implementation of the CTest class.

#include "precompiled.h"
#include "Test.h"


#include "NeoMem.h"
#include "BDoc.h"
#include "ConstantsDatabase.h" //. should be part of neodoc

#include "DSource.h"



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

		// this won't be necessary - just create new doc and use it
		app.CloseAllDocuments(FALSE);

		// better - 
		// pdoc = new BDoc(); //?
		// pdoc = BDoc::Create(); //?
		// if there's app-specific stuff do that separately. 
		//, mfc weird doctemplate stuff in filenew handler? override it and simplify? 
		// but also ties in with mru files etc. 
		app.OnFileNew();
		BDoc* pdoc = BDoc::GetDoc();
		ASSERT_VALID(pdoc);

		BDoc& doc = *pdoc;
//		BDoc& doc = BDoc::GetDoc();

//		DSource& ds = DSource(pdoc);

//		CUI* pui = this;
//		CNeoMem* pui = this;

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
//		HOBJECT hobjFishFolder = doc.NewObject(classFolder, "fish");
		OBJID idFishFolder = doc.NewObject(classFolder, "fish");


//		DObject dobjFishFolder = ds.NewObject(classFolder, "fish");

//		HOBJECT hobj2 = doc.FindObject(classFolder, "fish");
//		HOBJECT hobj2 = doc.GetObject(classFolder, "fish");
//		assert(hobjFishFolder == hobj2);


		// set description

		CString strDesc("fish i'm thinking of getting");
		doc.SetPropertyString(idFishFolder, propDescription, strDesc);

		// check it
		CString str = doc.GetPropertyString(idFishFolder, propDescription);
		ASSERT(str==strDesc);

/*

		// higher level interface
//		doc.New("folder","fish");
//		doc.New("class","fish");
//		doc.New("fish","plecy");

		// higher
//		doc.Parse("add folder fish");
//		doc.Parse("add class fish");
//		doc.Parse("goto fish folder");
//		doc.Parse("add fish plecy");

		// cf mkdir fish


		// add a fish class
//		hobjParent = pdoc->GetObject(rootClass);
//		HOBJECT hobjFishClass = pdoc->CreateObject(classClass, "Fish", hobjParent);
//		pdoc->AddObject(hobjFishClass); // ugh, have to add it also!
//		OBJID classFish = hobjFishClass->GetObjectID();
		
//x		HOBJECT hobjFishClass = doc.NewObject(classClass, "Fish");
		OBJID idFishClass = doc.NewObject(classClass, "Fish");


		{
			// check location of new object
//x			HOBJECT hobj = doc.GetPropertyLink(hobjFishClass, propLocation); 
//x			HOBJECT hobj2 = doc.GetObject(folderClasses);
//x			ASSERT(hobj == hobj2);
			OBJID id = doc.GetPropertyLink(idFishClass, propLocation); 
			ASSERT(id == folderClasses);

			// double check id of location
//x			OBJID id = doc.GetObjectID(hobj);
//x			ASSERT(id == folderClasses);
		}


		{
			// set description
			CString strDesc("a thing that swims in the water");
			doc.SetPropertyString(idFishClass, propDescription, strDesc);

			// check it
			CString str = doc.GetPropertyString(idFishClass, propDescription);
			ASSERT(str==strDesc);
		}


		// add a fish to the fish folder
//		pdoc->UIAddNewObject(); //, adapt this so can pass params to it...

		OBJID idPlecy = doc.NewObject(idFishClass, "plecy", idFishFolder);
		doc.SetPropertyString(idPlecy, propDescription, "plecostomus");
*/
/*
		
		// import a new icon

		// get Test folder with .ico files
		CStringEx strTestFolder = m_strApplicationFolder + "\\..\\..\\..\\Test\\";

		HOBJECT hobjIcon = NULL;
//		hobjIcon = pdoc->UIImportIcon(gpgui, "foo.ico"); // nonexistent file
//		hobjIcon = pdoc->UIImportIcon(gpgui, "neomem.cnt"); // bad file
//		hobjIcon = pdoc->UIImportIcon(gpgui, strTestFolder + "Fish.ico");
		hobjIcon = pdoc->UIImportIcon(gpgui, strTestFolder + "Fish.ico", "Fish");
//		hobjIcon = pdoc->UIImportIcon(gpgui);

		// attach the icon to fish class
//		hobjFishClass->SetPropertyLink(propClassDefIcon
		hobjFishClass->SetIconID(hobjIcon->GetObjectID()); // is this right? 
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

*/



/*
		// set folder default to fish class
		// This will set document modified flag and update views
		doc.SetPropertyLink(idFishFolder, propDefaultClass, idFishClass);
//			pdoc->UIChangeObjectContents(hobjFishClass);

		{
			OBJID id = doc.GetPropertyLink(idFishFolder, propDefaultClass);
			ASSERT(id == idFishClass);
		}


		// add a new property 'price'

//		pdoc->UIAddNewPropertyDef(); //, adapt this

		OBJID idPrice = doc.NewProperty("Price", proptypeCurrency, "how much it costs");


		// add to fish class
//		doc.SetPropertyLink(
		doc.AddPropertyLink(idFishClass, propObjectProperties, idPrice);

*/

/*
		// set plecy's price
		hobjPlecy->SetPropertyText(propPrice, "$1.34");

		// select the fish folder
		pdoc->SetCurrentObject(hobjFishFolder);

		// add price to the contents view
//		nCol = GetColumnCount();
//		m_pdatColumns->InsertColumn(lngPropertyID, m_pDoc, 0, nCol);
		// remove a column
//		BObject* pobjProp = m_pDoc->GetObject(lngPropID);
//		m_pdatColumns->RemoveColumn(nCol);
		CFrameChild* pui = theApp.GetChildFrame();
		CViewContents* pview = DYNAMIC_DOWNCAST(CViewContents, pui->GetView(viewContents));
		ASSERT_VALID(pview);
		pview->m_lvw.InsertColumnAsk(propPrice, 2);

		// set plecy desc
		hobjPlecy->SetPropertyText(propDescription, "plecostomus catfish");
//		hobjPlecy->SetDescription("plecostomus catfish");

		// add another fish
		HOBJECT hobjGlassfish = pdoc->CreateObject(classFish, "glassfish", hobjFishFolder);
		pdoc->AddObject(hobjGlassfish);

		// convert the prop to a string
//		hobjPrice->SetPropertyData(propPropType, proptypeString);

*/

	}
	catch (CException* e) {
		//?
		const MSG msg = {0};
		AfxGetThread()->ProcessWndProcException(e, &msg);
	}

//, beeps too loud		AfxMessageBox("All tests done.");

}




