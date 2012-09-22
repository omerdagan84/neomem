// Test.cpp: implementation of the CTest class.

#include "precompiled.h"
#include "Test.h"


#include "NeoMem.h"
#include "BDoc.h"
#include "ConstantsDatabase.h" //. should be part of neodoc

#include "BDataIcon.h"
#include "BFolder.h"
#include "BClass.h"


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
		// this is an object of class 'folder', not something in the 'class' folder. confusing. 
		BFolder& objFolder = BFolder::New(doc, "fish");

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
		BClass& classFish = BClass::New(doc, "Fish");

		// check location
		OBJID idLoc = classFish.GetPropertyLink(propParent);
		ASSERT(idLoc == rootClass);


		// set description
		{
		CString strDesc("a thing that swims in the water");
		classFish.SetPropertyString(propDescription, strDesc);
		CString str = classFish.GetPropertyString(propDescription);
		ASSERT(str==strDesc);
		}


		// id is a read-only variable!
//		OBJID idfoo = classFish.id;
//		classFish.id = 2; // fails



		// create a fish, add it to fish folder
//		pdoc->UIAddNewObject(); //, adapt this so can pass params to it...
		BObject& objPlecy = BObject::New(doc, classFish.id, "plecy", objFolder.id);
		objPlecy.SetPropertyString(propDescription, "plecostomus");



		// set folder default to fish class
		// This will set document modified flag and update views?
//,		pdoc->UIChangeObjectContents(hobjFishClass);
		objFolder.SetPropertyLink(propDefaultClass, classFish.id);
		{
		OBJID id = objFolder.GetPropertyLink(propDefaultClass);
		ASSERT(id == classFish.id);
		}



		// add a new property 'price'
//		pdoc->UIAddNewPropertyDef(); //, adapt this
//,		BObject& objPrice = BPropDef::New(doc, "price", proptypeCurrency, "how much it costs");
		BObject& objPrice = BObject::New(doc, classProperty, "price", folderProperties);
		objPrice.SetPropertyLink(propPropertyType, proptypeCurrency);
		objPrice.SetPropertyString(propDescription, "how much it costs");


		// add to fish class
		classFish.SetPropertyLinksAdd(propObjectProperties, objPrice.id);


		// get list of property values
		ObjIDArray a;
		classFish.GetPropertyLinks(propObjectProperties, a);

		// check it
		int x = a.GetSize();
		ASSERT(a.GetSize() == 1);
		ASSERT(a.GetAt(0) == objPrice.id);


		// add another property (will exercise different code)
		BObject& objSize = BObject::New(doc, classProperty, "size", folderProperties);
		objSize.SetPropertyLink(propPropertyType, proptypeNumber);
		objSize.SetPropertyString(propDescription, "size in inches");

		classFish.SetPropertyLinksAdd(propObjectProperties, objSize.id);

		// check it
		classFish.GetPropertyLinks(propObjectProperties, a);
		ASSERT(a.GetSize() == 2);
		ASSERT(a.GetAt(0) == objPrice.id);
		ASSERT(a.GetAt(1) == objSize.id);


		// set plecy's price
		objPlecy.SetPropertyString(objPrice.id, "$1.34");


		// select the fish folder
		doc.SetCurrentObject(&objFolder);


		// add another fish
		BObject& objGlassfish = BObject::New(doc, classFish.id, "glassfish", objFolder.id);

		// convert the price prop to a string
//		objPrice.SetPropertyData(propPropType, proptypeString);
//		objGlassfish.SetPropertyString(objPrice.id, "$2.54");



		// import a new icon
		CStringEx strTestFolder = theApp.m_strApplicationFolder + "\\..\\test\\files\\";
		CStringEx strFilename = strTestFolder + "fish.ico";

		BObject& objIcon = BObject::New(doc, classIcon, "Fish", folderIcons);
		BDataIcon* pdatIcon = new BDataIcon();
		pdatIcon->LoadFile(strFilename);
		objIcon.SetPropertyData(propIconData, pdatIcon);

		// set fish class icon
		classFish.SetIconID(objIcon.id);
		//, this is right, but maybe a command would be better. 
		// eg classFish.SetObjectIcon or something. 
		// ie something to mimic the user command. 

		//, next
//		BIcon& objIcon = BIcon::New(doc, "Fish", strFilename);
//		classFish.SetIconID(objIcon.id);

		//, old ideas
//		hobjIcon = pdoc->UIImportIcon(gpgui, "foo.ico"); // nonexistent file
//		hobjIcon = pdoc->UIImportIcon(gpgui, "neomem.cnt"); // bad file
//		hobjIcon = pdoc->UIImportIcon(gpgui, strTestFolder + "Fish.ico");
//		hobjIcon = doc.UIImportIcon(gpgui, strTestFolder + "Fish.ico", "Fish");
//		hobjIcon = pdoc->UIImportIcon(gpgui);


		// test name functions
		BObject& objOctopus = BObject::New(doc, classFish.id);
		ASSERT(objOctopus.GetName() == CString("New Fish")); // default name
		ASSERT(objOctopus.GetPropertyString(propName) == CString("New Fish"));
//,bombs		ASSERT(objOctopus.GetPropertyString(propName) == objOctopus.GetName()); //, note getname is slower though...
		objOctopus.SetName("octopus");
		ASSERT(objOctopus.GetName(true) == CString("fish \"octopus\"")); // extended name







		// add price column
		// Get column information from object or from object's classdef
		// this is a copy so we're responsible for it
		BDataColumns* pdatColumns = DYNAMIC_DOWNCAST(BDataColumns, objFolder.GetPropertyData(propColumnInfoArray));
		pdatColumns->InsertColumn(objPrice.id, &doc);
		objFolder.SetPropertyData(propColumnInfoArray, pdatColumns); // will send hint
		delete pdatColumns;




		// add size column (new api)

		//, should be one line...
//		objFolder.GetColumns().AddColumn(objPrice.id);
		// want to say, at highest level
		// objFolder.AddColumn(objPrice.id);


		// cool, this works!
		// as long as you don't add more vars etc to the subclasses!
//		BFolder* folder = (BFolder*) &objFolder;
//		BFolder& f2 = (BFolder&) objFolder;

		//, not working
//		folder.GetColumns().AddColumn(objPrice.id);
//		BFolder& folder = (BFolder&) objFolder; //, move getcols to bobject
//		BDataColumns& cols = folder.GetColumns();
//		cols.InsertColumn(objSize.id,&doc);
//		folder.PutColumns(cols); //, ugh will delete cols

		// that's hideous.
		
		// objFolder.GetColumns().InsertColumn(objSize.id,&doc).PutColumns();
		// better but still ugh

		// a little better api. still confusing w/ delete &cols hidden

		BDataColumns& cols = objFolder.GetColumns();
		cols.InsertColumn(objSize.id, &doc);
		objFolder.SetColumns(cols); // will delete cols






		// narrow description column (old api)
		pdatColumns = DYNAMIC_DOWNCAST(BDataColumns, objFolder.GetPropertyData(propColumnInfoArray));
		int nCol = pdatColumns->GetColumnIndex(propDescription);
		int nWidth = pdatColumns->GetColumnWidth(nCol);
		pdatColumns->SetColumnWidth(nCol, nWidth * 0.60);
		objFolder.SetPropertyData(propColumnInfoArray, pdatColumns); // sends hint
		delete pdatColumns;

		// narrow price column (nicer api)
		{
//		cols = objFolder.GetColumns(); //, bombs - redefining a ref? apparently so, but really weird message about =( operator in bdata
		BDataColumns& cols = objFolder.GetColumns(); 
		nCol = cols.GetColumnIndex(objPrice.id);
		nWidth = cols.GetColumnWidth(nCol);
		cols.SetColumnWidth(nCol, nWidth * 0.6);
		objFolder.SetColumns(cols); // sends hint, deletes cols? sets to 0. yeah
		}


		// int nWidth = obj.GetColumns().GetColumnWidth(nCol);
		// obj.GetColumns().SetColumnWidth(nCol, nWidth*0.6);

		//, but search SetPropertyData(propObjectColumnInfoArray




		// make a category for fish
		BClass& classCategory = BClass::New(doc, "Category (Fish)");
		BFolder& folderCategories = BFolder::New(doc, "Categories", rootUser);
		BObject& objCatSaltwater = BObject::New(doc, classCategory.id, "Saltwater", folderCategories.id);
		BObject& objCatFreshwater = BObject::New(doc, classCategory.id, "Freshwater", folderCategories.id);
		

		BObject& propCategory = BObject::New(doc, classProperty, "Category", folderProperties);
		propCategory.SetPropertyLink(propPropertyType, proptypeLink); //, note, this is REQUIRED, but not enforced here... should be
		propCategory.SetPropertyString(propDescription, "oh, fish category you know");
		propCategory.SetPropertyLink(propLinkSource, folderCategories.id);

		//,
//		classFish.AddProperty(&propCategory);
		// vs
		classFish.SetPropertyLinksAdd(propObjectProperties, propCategory.id);

		// oh because addproperty is to add a property to the properties collection for that object, in this case a class.
		// but it's not what determines what properties an object has - that's the propObjectProperties. 
		// ok, so addproperty should be hidden?



		// point the category property to the categories folder
//		propCategory.SetProperty




		// save doc
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




