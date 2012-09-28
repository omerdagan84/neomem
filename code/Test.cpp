// Test.cpp: implementation of the CTest class.

#include "precompiled.h"
#include "Test.h"


#include "NeoMem.h"
#include "BDoc.h"
#include "ConstantsDatabase.h" //. should be part of neodoc

#include "BDataIcon.h"
#include "BFolder.h"
#include "BClass.h"



#include <iostream>
using namespace std;


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


		// if there's app-specific stuff do that separately. 
		//, mfc weird doctemplate stuff in filenew handler? override it and simplify? 
		// but also ties in with mru files etc. 

		//, BDoc& doc = BDoc::New(); //, not working because of access to onfilenew
		app.OnFileNew();
		BDoc* pdoc = BDoc::GetDoc(); // get rid of this fn, if possible? but i think mfc uses it?
		ASSERT_VALID(pdoc);
		BDoc& doc = *pdoc;

		// get pointer to UI, to pass to doc
//		CUI* pui = this;
//		CNeoMem* pui = this;
//		doc.pui = pui;


		//, should document really know about current object? no, that's a ui thing. 
		// there could be multiple ui's looking at the db. 
		// it's pretty entwined in BDoc though. 


		// add folder for fish
		// this is an object of class 'folder', not something in the 'class' folder. confusing. 
		BFolder& objFolder = BFolder::New(doc, "fish");

		// check class
		//, propClassID vs propClass??
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
		objPrice.SetPropertyLink(propPropertyType, proptypeCurrency);// reqd
		objPrice.SetPropertyString(propDescription, "how much it costs");
		// check
		BObject* pobj = doc.GetObject(objPrice.id);
		ASSERT(pobj->GetPropertyLink(propPropertyType) == proptypeCurrency);


		// add to fish class
		classFish.SetPropertyLinksAdd(propObjectProperties, objPrice.id);
		// check
		// get list of property values
		//,, should be bobjects, or some lightweight recordset
//		ObjIDArray a;
//		classFish.GetPropertyLinks(propObjectProperties, a);
		CObArray* pa = classFish.GetPropertyLinks(propObjectProperties);

		// should be ..& a = classFish.GetPropertyLinks(..);
		// with the reference being to the m_a
		// but... we want COPIES of data, not the original.
		// so have to return a pointer. right? 

		// check first value
		int x = pa->GetSize();
		ASSERT(pa->GetSize() == 1);
		ASSERT(pa->GetAt(0) == &objPrice);
		//, check (add FindItem)
//		ObjIDArray a;
//		classFish.GetPropertyLinks(propObjectProperties, a);
//		ASSERT(a.FindItem(objPrice.id));
//		delete pa;

		// add another property (will exercise different code)
		BObject& objSize = BObject::New(doc, classProperty, "size", folderProperties);
		objSize.SetPropertyLink(propPropertyType, proptypeNumber);//reqd
		objSize.SetPropertyString(propDescription, "size in inches");

		classFish.SetPropertyLinksAdd(propObjectProperties, objSize.id);

		pa->RemoveAll();
		delete pa;

		// check
		pa = classFish.GetPropertyLinks(propObjectProperties);
		ASSERT(pa->GetSize() == 2);
		ASSERT(pa->GetAt(0) == &objPrice);
		ASSERT(pa->GetAt(1) == &objSize);
		delete pa;


		// set plecy's price
		objPlecy.SetPropertyString(objPrice.id, "$1.34");
		// check
		ASSERT(objPlecy.GetPropertyString(objPrice.id) == "$1.34"); // works? why? must have good == method
		ASSERT(objPlecy.GetPropertyString(objPrice.id) == CString("$1.34")); 

		

		// select the fish folder
		doc.SetCurrentObject(&objFolder);
		// check
		ASSERT(doc.GetCurrentObject() == &objFolder);

		// add some text to it
		{
		CString str("what am i going to do with all these fish?");
		//, can't use propPlainText - not symmetric. fix it. 
		objFolder.SetPropertyString(propRtfText, str);
		ASSERT(objFolder.GetPropertyString(propRtfText) == str);
		}

		// add another fish
		BObject& objGlassfish = BObject::New(doc, classFish.id, "glassfish", objFolder.id);

		//, check count of fish in db
		//, no way to do a simple query like that

		objGlassfish.SetPropertyString(objPrice.id, "$2.54");

		//, convert the price prop to a number (instead of currency)
//		objPrice.SetPropertyLink(propPropType, proptypeNumber); // link or long?
//		ASSERT(objGlassFish.GetPropertyData();


		// import a new icon
		CStringEx strTestFolder = theApp.m_strApplicationFolder + "\\..\\test\\files\\";
		CStringEx strFilename = strTestFolder + "fish.ico";

		BObject& objIcon = BObject::New(doc, classIcon, "Fish", folderIcons);
		BDataIcon* pdatIcon = new BDataIcon();
		pdatIcon->LoadFile(strFilename);
		objIcon.SetPropertyData(propIconData, pdatIcon);
		delete pdatIcon;

		// set fish class icon
		classFish.SetIconID(objIcon.id);
		//, this is right, but maybe a command would be better. 
		// eg classFish.SetObjectIcon or something. 
		// ie something to mimic the user command. 

		//, next?
//		BIcon& objIcon = BIcon::New(doc, "Fish", strFilename);
//		classFish.SetIconID(objIcon.id);


		// test name functions
		//, note getname is slower though...
		BObject& objOctopus = BObject::New(doc, classFish.id);
		ASSERT(objOctopus.GetName() == CString("New Fish")); // default name
//		ASSERT(objOctopus.GetName() == "New Fish"); //, bombs - fix
		ASSERT(objOctopus.GetPropertyString(propName) == CString("New Fish"));
		ASSERT(objOctopus.GetPropertyString(propName) == "New Fish");
		ASSERT(objOctopus.GetPropertyString(propName) == objOctopus.GetName()); 
		objOctopus.SetName("octopus");
		ASSERT(objOctopus.GetName(true) == CString("fish \"octopus\"")); // extended name







		// add price column
		// Get column information from object or from object's classdef
		// this is a copy so we're responsible for it
		//, this code is gross - fix in add size column code below
		// what if gpd().ToColumns() <- does ddcast and assertvalid
		// BDataColumns* pdatColumns = objFolder.GetPropertyData(propColumnInfoArray).ToColumns();
		// much nicer
//		BDataColumns* pdatColumns = DYNAMIC_DOWNCAST(BDataColumns, objFolder.GetPropertyData(propColumnInfoArray));
//		ASSERT_VALID(pdatColumns);
//		objFolder.SetPropertyData(propColumnInfoArray, pdatColumns); // will send hint
//		delete pdatColumns;

		/*
		BDataColumns* pdatColumns = objFolder.GetPropertyData(propColumnInfoArray)->ToColumns();
//		BDataColumns datColumns(*pdatColumns); // bad and weird compile errors
		BDataColumns& datColumns(*pdatColumns);
		datColumns.InsertColumn(objPrice.id, &doc);
		objFolder.SetPropertyData(propColumnInfoArray, &datColumns); // will send hint
		*/

//x
/*
		// so tc could return a reference to the *pointer
		//, make GetPropertyColumns
		BDataColumns& datColumns = objFolder.GetPropertyData(propColumnInfoArray)->ToColumns();
		datColumns.InsertColumn(objPrice.id, &doc);
		objFolder.SetPropertyData(propColumnInfoArray, &datColumns); // will send hint
		//, but need to delete &datColumns? yep. bleh.
		delete &datColumns;
*/

		//, hmm, but will always be the same propertyid? hence GetColumns fn?
		{
		BDataColumns* pdatColumns = objFolder.GetPropertyColumns(propColumnInfoArray);
		pdatColumns->InsertColumn(objPrice.id, &doc);
		objFolder.SetPropertyData(propColumnInfoArray, pdatColumns); // will send hint
		delete pdatColumns;
		}

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

		// that's hideous. fix it below
		
		// objFolder.GetColumns().InsertColumn(objSize.id,&doc).PutColumns();
		// better but still ugh

//x
		// a little better api. still confusing w/ delete &cols hidden
//		{
//		BDataColumns& cols = objFolder.GetColumns();
//		cols.InsertColumn(objSize.id, &doc);
//		objFolder.SetColumns(cols); //, will delete cols - kindof creepy
//		}

		{
		BDataColumns* pdatColumns = objFolder.GetPropertyColumns(propColumnInfoArray);
		pdatColumns->InsertColumn(objSize.id, &doc, 100);
		objFolder.SetPropertyData(propColumnInfoArray, pdatColumns); // will send hint
		delete pdatColumns;
		}




		// narrow description column (old api)
		{
		BDataColumns* pdatColumns;
		pdatColumns = DYNAMIC_DOWNCAST(BDataColumns, objFolder.GetPropertyData(propColumnInfoArray));
		int nCol = pdatColumns->GetColumnIndex(propDescription);
		int nWidth = pdatColumns->GetColumnWidth(nCol);
		pdatColumns->SetColumnWidth(nCol, int(nWidth * 0.60));
		objFolder.SetPropertyData(propColumnInfoArray, pdatColumns); // sends hint
		delete pdatColumns;
		}

		// narrow price column (nicer api, but...)
		{
//		cols = objFolder.GetColumns(); //, bombs - redefining a ref? apparently so, but really weird message about =( operator in bdata
		BDataColumns& cols = objFolder.GetColumns(); 
		int nCol = cols.GetColumnIndex(objPrice.id);
		int nWidth = cols.GetColumnWidth(nCol);
		cols.SetColumnWidth(nCol, int(nWidth * 0.6));
		objFolder.SetColumns(cols); // sends hint, deletes cols? sets to 0. yeah  //, eh, creepy
		}

		// don't like using a ref like that - seems weird - better to do BDataColumns?
		// but then would need to write it back.



		// int nWidth = obj.GetColumns().GetColumnWidth(nCol);
		// obj.GetColumns().SetColumnWidth(nCol, nWidth*0.6);

		//, but search SetPropertyData(propObjectColumnInfoArray




		// make a category for fish
		BClass& classCategory = BClass::New(doc, "Category");
		BFolder& folderCategories = BFolder::New(doc, "Categories", rootUser);
		BObject& objSalt = BObject::New(doc, classCategory.id, "Saltwater", folderCategories.id);
		BObject& objFresh = BObject::New(doc, classCategory.id, "Freshwater", folderCategories.id);
		
		// make category property
		BObject& propCategory = BObject::New(doc, classProperty, "Category", folderProperties);
		propCategory.SetPropertyLink(propPropertyType, proptypeLink); //, note, this is REQUIRED, but not enforced here... should be
		propCategory.SetPropertyString(propDescription, "oh, fish category you know");
		propCategory.SetPropertyLink(propLinkSource, folderCategories.id); // where to find values

		// add property to the fish class
		classFish.SetPropertyLinksAdd(propObjectProperties, propCategory.id);
		// note, there is 
//		objTest.ClassDefAddProperty(objSize.id);
		// which does the same thing, because it's not finished. contains just:
//		SetPropertyLinksAdd(propObjectProperties, lngPropertyID);


		// add property to fish folder
		{
		BDataColumns* pdatColumns = objFolder.GetPropertyColumns(propColumnInfoArray);
		pdatColumns->InsertColumn(propCategory.id, &doc, 0, 2);
		objFolder.SetPropertyData(propColumnInfoArray, pdatColumns); // will send hint
		delete pdatColumns; //, bombs
		}

		// add squid
		BObject& objSquid = BObject::New(doc, classFish.id, "squid", objFolder.id);

		// set single and multiple values 
		{
		objPlecy.SetPropertyLink(propCategory.id, objFresh.id);
		objGlassfish.SetPropertyLink(propCategory.id, objFresh.id);

		CObArray a;
		a.Add(&objFresh);
		a.Add(&objSalt);
		objOctopus.SetPropertyLinks(propCategory.id, &a);

//,		objSquid.SetPropertyLinksAdd(propCategory.id, objFresh.id);
//,		objSquid.SetPropertyLinksAdd(propCategory.id, objSalt.id);
		}





		// check GetPropertyLinks and SetPropertyLinksAdd
		{
		BObject& objTest = BObject::New(doc, classClass, "test", objFolder.id);
		CObArray* pa = objTest.GetPropertyLinks(propObjectProperties); // could be null
		ASSERT(pa == NULL);
		delete pa;
		objTest.SetPropertyLinksAdd(propObjectProperties, objSize.id);
		pa = objTest.GetPropertyLinks(propObjectProperties);
		ASSERT(pa);
		ASSERT(pa->GetCount() == 1);
		ASSERT(pa->GetAt(0) == &objSize);
		delete pa;
		objTest.DeleteObject();
		}


		// check ClassDefAddProperty method
		{
		BObject& objTest = BObject::New(doc, classClass, "test", objFolder.id);
		CObArray* pa = objTest.GetPropertyLinks(propObjectProperties); // could be null
		ASSERT(pa == NULL);
		delete pa;
		objTest.ClassDefAddProperty(objSize.id);
		pa = objTest.GetPropertyLinks(propObjectProperties);
		ASSERT(pa->GetCount() == 1);
		ASSERT(pa->GetAt(0) == &objSize);
		delete pa;
		objTest.DeleteObject();
		}









		// test making copies of bobjects etc
//		BObject* pobj = doc.GetObject(objPlecy.id);
//		pobj->GetPropertyLong(









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




