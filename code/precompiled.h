
// precompiled.h
// This is the include file for standard system include files, or project specific 
// include files that are used frequently but changed infrequently.
// Note: this file must be included as the first non-comment thing done in each file!
// precompiled.h was created by AppWizard to keep the list of header files to be precompiled. 
// Note: Precompiled header (PCH) files speed up build times because they don’t require recompiling.
// see also http://www.cygnus-software.com/papers/precompiledheaders.html
//-----------------------------------------------------------------------------------------------------

#pragma once

//#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
//#include <windows.h> // TrackMouseEvent



#ifdef _WIN32_WINNT
#if (_WIN32_WINNT >= 0x0400)
#pragma message("Functions Should Be Enabled")
#else
#pragma message("Operating System Version Too OLD")
#endif
#else
#pragma message("Computer is Not Win32 NT Based")
#endif


// Note: afx was the old name for mfc. 
#include <afxwin.h>     // MFC core and standard components
#include <afxext.h>     // MFC extensions
#include <afxcmn.h>		// MFC support for Windows Common Controls
#include <afxcview.h>	// MFC support for common control views
#include <afxdisp.h>    // MFC Automation classes
#include <afxdtctl.h>	// MFC support for Internet Explorer 4 Common Controls
#include <afxole.h>		// MFC Drag drop support
#include <afxadv.h>		// MFC CSharedFile support
#include <afxtempl.h>	// MFC Template support, for typedef CArray<...> etc
#include <afxpriv.h>    // MFC print preview support, includes CPreviewView
#include <afxrich.h>	// MFC support for richedit control - cricheditdoc etc
#include <afxodlgs.h>	// MFC support for olepropertydialog
#include <afxdao.h>		// MFC support for database access - for importing

#include <dde.h>	    // DDE suport (includes WM_DDE_EXECUTE definition - needed by CDialogWelcome)
#include <mapi.h>		// for email support
#include <richole.h>	// includes LPRICHEDITOLE etc
#include <shlwapi.h>	// Includes DLLGETVERSIONPROC
#include <stdlib.h>		// Standard something
#include <wincrypt.h>	// cryptography api functions


// If you copy source code from MFC routines some of them require this macro. 
// Defined in MFC's precompiled.h file:
#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)


#include "macros.h"
#include "library.h"




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

