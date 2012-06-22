
// This file defines Help Context IDs for topics that are not associated with 
// user-interface items.

// This example header file is #include'd twice:
// (1) It is #include'd by the .CPP file that passes the DWORD Help
//     Context ID to CWinApp::WinHelp.
// (2) It is #include'd in the [MAP] section of the .HPJ file,
//     to associate the help context string "HID_MYTOPIC" with
//     the help context numeric ID, 101.
// The help context string "HID_MYTOPIC" is what identifies the
// help topic in the help .RTF source file, in the "#" footnote:
//     # HID_MYTOPIC
//
// Note, it is not necessary to manage help context id's this way
// for help topics associated with command id's and user interface
// id's defined in your RESOURCE.H file; you should use the MAKEHM
// tool via the custom build rule on your resource.h file to produce 
// a help map (.HM) file for these id's.  It is necessary to manage 
// help context id's as illustrated here only for help topics not 
// associated with command id's or user interface id's. 

#define HID_KEYBOARD_SHORTCUTS 101
#define HID_WHATS_NEW 103
#define HID_CLOCK 104





