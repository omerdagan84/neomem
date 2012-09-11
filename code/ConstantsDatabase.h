
// Constants.h
// Constants required by database code
//-----------------------------------------------------------------------------------------------------------------

#pragma once

// versionFileStructure: Version of the physical layout of the file - BDoc and 
// BObject and BData member variables, etc.
// The program should be able to read all versions up to the current version.
// If there are any future changes to the file structure (BDoc, BObject, BObjects, 
// BDatas, etc) then this version number should be increased by one and all 
// serialization code equipped to handle it.
// See also: versionDataModel = in BDoc.h, which describes the version of the system 
// classes and properties, etc.
//ULONG const versionFileStructure = 1;
ULONG const versionFileStructure = 2; // added encryption, 



// BObject messages passed to SendMessage
//------------------------------------------------------------------------------------------------------------------
ULONG const msgResetData = 1;


// BObject Flags (can be OR'd together)
// can have 32 of these, so could be used for various object states
//----------------------------------------------------------------------------------------------------------
ULONG const flagSystem =     0x00000001L; // objects with this flag set will not show up in searches when in user mode
ULONG const flagExpanded = 0x00000002L; // if set then object is expanded in cviewtree window
ULONG const flagNoDelete =  0x00000004L; // objects with this flag set will not be able to be deleted
ULONG const flagAdminOnly =0x00000008L; // objects with this flag set will not show up in list of classes in addnew dialog, list of property types, properties, etc.
ULONG const flagNoModify =  0x00000010L; // use this to prevent user from modifying objects (eg some property defs)
ULONG const flagNoAutosort =   0x00000020L; // if true then children are not sorted
ULONG const flagTemp =        0x00000040L; // if on then will not show in either user or admin modes
ULONG const flagHighPriority = 0x00000080L; // gives object high sort priority
ULONG const flagNoModifyClass = 0x00000100L; // can't modify class (eg home object)
ULONG const flagFilter = 0x00000200L; // a temporary flag used for filtering objects in dialogs etc
ULONG const flagDisabled = 0x00000400L; // a temporary flag used for disabling object for listviewex control
//ULONG const flagObjectHasChildren = 0x00000800L; // a classdef flag indicating that objects of this class can have children (mainly used in class wizard for now)
ULONG const flagPropValueReadOnly = 0x00001000L; // use for read-only property defs (eg calculated values, size of object)
ULONG const flagPropValueNoSerialize = 0x00002000L; // use for property defs - if true then don't serialize property value (not used yet)
//ULONG const flagHidden = 0x00000004L;
//ULONG const flagShortcut = 0x00000008L;
//ULONG const flagSortChildren = 0x00000020L;
//ULONG const flagShowChildren = 0x00000040L;


// System ObjectID's
// These constants should match the objects in the template file
//----------------------------------------------------------------------------------------------------------

// We have four root objects:
// rootMain (GetRoot() returns this)
// rootUser 
// rootSystem
// rootClass 



// This reserves some id space for future system objects 
ULONG const lngUserIDStart = 500000;

// Classes 1-999
//ULONG const classRoot = 1;
ULONG const rootClass = 1;
ULONG const classFolder = 2;
ULONG const classClass = 3;
ULONG const classProperty = 4;
ULONG const classPaper = 5;
ULONG const classIcon = 6;
ULONG const classRecycleFolder = 7;
ULONG const classView = 8;
ULONG const classPropertyType = 9;
ULONG const classUnit = 10;
ULONG const classUnitSystem = 11; // english/metric
//ULONG const classPerson = 12;
ULONG const classHome = 20;
//ULONG const classPicture = 36; // this is a system/hidden class
//ULONG const classReport = 46; 
//ULONG const classFileFormat = 21;



// System Objects and Folders 1000-1999
ULONG const rootMain = 1000; // absolute root node of everything
ULONG const rootUser = 1001; // root node of user objects
ULONG const rootSystem = 1002; // root node of system objects
ULONG const folderClasses = rootClass; // alias
ULONG const folderRecycle = 1003;
ULONG const folderViews = 1004;
ULONG const folderPropertyTypes = 1005;
ULONG const folderUnits = 1007;
ULONG const folderProperties = 1008; 
ULONG const folderIcons = 1009; 
ULONG const folderUnitSystems = 1010;
//ULONG const folderReference = 1011;
//ULONG const folderLookup = 1012;
//ULONG const folderFileFormats = 1013;


// Property Types 2000-2999
// These correspond with the different BData classes
ULONG const proptypeLong = 2000; // BDataLong
ULONG const proptypeString = 2001; // BDataString
ULONG const proptypeBinary = 2002; // BDataBinary
ULONG const proptypeDate = 2003; // BDataDate
ULONG const proptypePersonName = 2004; // BDataPersonName
ULONG const proptypeAddress = 2005; // BDataAddress
ULONG const proptypeNumber = 2006; // BDataNumber
//ULONG const proptypeColumnInfo = 2007; // BDataColumnInfo - obsolete
ULONG const proptypeIcon = 2008; // BDataIcon
ULONG const proptypeLink = 2009; // BDataLink
//ULONG const proptypeArray = 2010; // BDataArray - obsolete
//ULONG const proptypeLinks = 2011; // BDataLinks - obsolete - merged into BDataLink
ULONG const proptypeFlags = 2012; // BDataFlag
ULONG const proptypeCurrency = 2013; // uses BDataNumber with unitid=currency
ULONG const proptypeCalculated = 2014; // uses BDataNumber
ULONG const proptypeColumns = 2015; // BDataColumns
ULONG const proptypeViews = 2016; // BDataViews
ULONG const proptypeTimeInterval = 2017; // uses BDataNumber with unitid=seconds
ULONG const proptypeBoolean = 2018; // BDataBoolean
ULONG const proptypeEmail = 2019;
ULONG const proptypeHyperlink = 2020; // uses BDataString
ULONG const proptypeRtf = 2021; // goes by "String (Formatted)" - propRtfText is the only thing that uses it for now. 
ULONG const proptypeWebsite = 2022; 
ULONG const proptypeFile = 2023; 
ULONG const proptypeFolder = 2024;
// Note: Be sure to add handler for any new property types in BDoc::CreateBDataFromPropertyType!



// Properties 3000-4999

// pseudo properties (stored in BObject)
ULONG const propName = 3000; // (not a pseudo property)
ULONG const propObjectID = 3001;
ULONG const propClassID = 3002; 
ULONG const propIconID = 3003; 
ULONG const propFlags = 3004; 
ULONG const propClassName = 3005; 
ULONG const propParentName = 3006; // name of parent (location)
ULONG const propValue = 3007; // used in property view to obtain a property value of the current object
ULONG const propSize = 3008; 
ULONG const propParentID = 3009; // parent id (location) 
ULONG const propLocationID = propParentID;

// bobject versions of id properties for cmd interface
//! update template file
ULONG const propParent = 3020;
ULONG const propLocation = propParent;
//ULONG const propDateCreated = 3010; // see m_dateCreated
//ULONG const propDateModified = 3011; 

// ClassDef properties
ULONG const propObjectProperties = 3100; // properties inherited by objects of a class
ULONG const propObjectNamePropertyType = 3101; // type of bdata object to store object's name in (default is bdatastring)
ULONG const propObjectFlags = 3102; // flags to assign to new objects of this class

// PropertyDef properties
ULONG const propPropertyType = 3200; // specifies type of data the property stores
ULONG const propLinkSource = 3201; // source object/folder for link properties
ULONG const propDefaultUnits = 3202; // default units to use with number
ULONG const propLimitNumberOfLinks = 3203; // for now, 1 for single, or 0 for multiple
ULONG const propDisplayLinkHierarchy = 3204; // true/false - display hierarchy of linked object
ULONG const propAdditionalDisplayProperty = 3205; // link - additional property to display to make linked object name unique

// Icon properties
ULONG const propIconData = 3300; // icon binary data
ULONG const propIconResourceID = 3301; // if not storing raw icon data, this stores the resource id of the icon

// Paper properties
ULONG const propDescription = 3400;
ULONG const propRtfText = 3401; // rtf text
ULONG const propRtfInsertionPoint = 3402;
ULONG const propRtfTopLineChar = 3403;
ULONG const propPlainText = 3404; // plain version of rtf text
//#define propRtf propRtfText
//#define propText propRtfText

// View properties
ULONG const propCaption = 3500; // name of view with & for hotkey


/*
// Miscellaneous properties
ULONG const propAuthor = 3500; 
ULONG const propWeight = 3501; 
ULONG const propRating = 3502; 
ULONG const propDate = 3503; 
ULONG const propPrice = 3504; 
ULONG const propSalary = 3505; 
ULONG const propHours = 3506; 
ULONG const propAddress = 3507; 
ULONG const propPhoneNumber = 3508; 
ULONG const propEmail = 3509; 
ULONG const propWebsite = 3510; 
ULONG const propUserName = 3511; 
ULONG const propPassword = 3512; 
ULONG const propLength = 3513; 
ULONG const propCategory = 3514;
ULONG const propBirthday = 3515;
*/

// Inherited/class chain properties
// The classdef ("Object") version of these properties should be the object version + 500
ULONG const propDefaultClass = 4000; // default class to add as child of this object
ULONG const propColumnInfoArray = 4001; // array of bdatacolinfo objects describing contents column arrangement
ULONG const propViewArrangement = 4002; // array of arrays of bdataview objects describing view layout

ULONG const propObjectDefaultClass = 4500; // default class to add to objects of a class (in add new dialog)
ULONG const propObjectColumnInfoArray = 4501; // see propColumnInfoArray
ULONG const propObjectViewArrangement = 4502; // see propViewArrangement



// Icons 5000-5999
//ULONG const iconDefault = 5000;
ULONG const iconDefault = 5060; // default to use if no icon specified (colored triangles)
ULONG const iconNew = 5015; // new row icon (triangle) used in contents view


// Views 6000-6999 (correspond to CView classes)
// IMPORTANT:: The order of these must match the corresponding resource ID's, so that 
// they can be mapped to each other: 
//		lngObjectID = nCommandID - ID_VIEW_DATA + viewData;
//		nCommandID = lngObjectID - viewData + ID_VIEW_DATA;
// so viewSearch = ID_VIEW_SEARCH - ID_VIEW_DATA + viewData

ULONG const viewData = 6000; // navigation
ULONG const viewSearch = 6001; // navigation
ULONG const viewBookmarks = 6002; // navigation
ULONG const viewText = 6003;
ULONG const viewContents = 6004;
ULONG const viewProperties = 6005;
ULONG const viewPictures = 6006;
ULONG const viewGrid = 6007;
ULONG const viewHTML = 6008;
ULONG const viewMap = 6009;
ULONG const viewIndex = 6010;

ULONG const viewHome = 6011;
ULONG const viewGraphPaper = 6012;
ULONG const viewGraph = 6013;


// Units 7000-7999
//ULONG const unitMiles = 7000;
//ULONG const unitFeet = 7001;
//ULONG const unitInches = 7002;
ULONG const unitCurrency = 7003; // local currency as defined by windows
//ULONG const unitSeconds = 7004;
//ULONG const unitMinutes = 7005;
//ULONG const unitHours = 7006;


// File formats 8000-8999
//ULONG const formatCsv = 8000;
//ULONG const formatTab = 8001;
//ULONG const formatRtf = 8002;


// User ObjectID's
// (Start at 500,000)
//----------------------------------------------------------------------------------------------------------







