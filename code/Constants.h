
// Constants.h
// Constants required by gui
//-----------------------------------------------------------------------------------------------------------------

#pragma once


// Application Messages
// The WM_APP constant is used by applications to help define private messages, 
// usually of the form WM_APP+X, where X is an integer value. 
// #define WM_APP       0x8000
// Message numbers in the third range (0x8000 through 0xBFFF) are available for 
// application to use as private messages. Message in this range do not conflict with 
// system messages. 
//#define AM_COLUMNS_CHANGED		WM_APP + 0
//#define AM_ITEM_SELECTED			 WM_APP + 1	// used by 


// UpdateView Hints
//------------------------------------------------------------------------------------------------------------------
ULONG const hintLoad = 1; // load data into the view
ULONG const hintSave = 2; // save the contents of the view
ULONG const hintAdd = 3; // object was added
ULONG const hintDelete = 4; // object was deleted; pHint is the BObject that was deleted
//ULONG const hintRename = 5; // use hintPropertyChange instead
ULONG const hintMoveObject = 6; // object was moved; pHint is the BObject that was moved
ULONG const hintLoadTree = 7; // load the entire file into the tree view
ULONG const hintSelect = 8; // Select is handled by the tab views - will add views dynamically
//ULONG const hintReloadChildren = 9; // obsolete
ULONG const hintResize = 10; // force recalculation of view window positions - used by header view
//ULONG const hintFindNext = 11; // 
ULONG const hintPropertyChange = 12; // object property was changed
ULONG const hintReposition = 13; // object was moved up or down relative to siblings
ULONG const hintRefresh = 14; // force redraw of all views - useful in cases like name display order changed (first last to last, first)
ULONG const hintDefaultFontChanged = 15; // 
ULONG const hintResortChildren = 16; // resort the children of the specified bobject
ULONG const hintRefreshAllIcons = 17; // causes all views to reobtain icons for each item - used when classdef changes an icon
ULONG const hintAddView = 18; // 
ULONG const hintRemoveView = 19; // 
ULONG const hintMoveViewUp = 20; // 
ULONG const hintResortAllChildren = 21; // resort all children in the file


// View modes
ULONG const modeNavigation = 1;
ULONG const modeContents = 2;


