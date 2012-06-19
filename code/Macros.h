
// Macros
//----------------------------------------------------------------------------------------------------------

#pragma once


// In release mode, we want asserts to at least raise an exception if they are false.
// Note: The brackets are necessary because if you had an assert inside an if statement 
// that was followed by an else statement, the logic could get messed up.
// Note: The # is the stringizing operator, which returns the parameter in double quotes
//.#ifndef _DEBUG
#undef ASSERT
#undef ASSERT_VALID
#define ASSERT(f) if (!(f)) { ThrowAssertion(THIS_FILE, __LINE__, #f); }
#define ASSERT_VALID(p) if (!(p)) { ThrowAssertion(THIS_FILE, __LINE__, #p); }
//#endif

//. should be doing this somehow...
//#define ASSERT_VALID(pOb)  (::AfxAssertValidObject(pOb, THIS_FILE, __LINE__))


// Turn on debug logfile
//#define _DEBUG_LOG  



// aliases
#define VK_PAGE_DOWN VK_NEXT
#define VK_PAGE_UP VK_PRIOR


// This TRACEDUMP macro saves having to enclose .Dump calls in #ifdef _DEBUGs
// ex. just say TRACEDUMP(col);
#ifndef TRACEDUMP
#ifdef _DEBUG
#define TRACEDUMP(a)   a.Dump(afxDump)
#else
#define TRACEDUMP(a)   a;
#endif // _DEBUG
#endif // TRACEDUMP


// Note: TRACE calls AfxTrace which says afxDump << szBuffer;

// To turn off trace statement just put an x in front of it - makes it easier to track down trace's also.
// This is how MFC turns off TRACE for the release compilation - 
#define xTRACE     1 ? (void)0 : ::AfxTrace

// This makes it easier to track down which trace statements are turned on -
// ie if you use yTRACE instead of TRACE, then you can search on yTRACE to find them all.
#define yTRACE TRACE



