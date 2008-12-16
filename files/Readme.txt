NeoMem Readme

--------------------------------------------------------------------
Description
--------------------------------------------------------------------
NeoMem is an open source information database, with an interface
similar to Windows Explorer. 

--------------------------------------------------------------------
What's New
--------------------------------------------------------------------

Version 1.2c Prerelease (2006-07-06)

- New export formats 
  - Tab Delimited (.txt)
  - Rtf Outline (.rtf)
  - Plain Text (.txt)
  - OPML (.opml) (Outline Processor Markup Language - see opml.org) 
- Starts on last selected item instead of home page.
- Can add "RegisterFileType=0" to neomem.ini file to prevent NeoMem 
  from registering the .neo file type. Useful for thumbdrive users. 
- Uses UPX to compress neomem.exe from 
- Removed rtf column from csv export. Just has plain text now. 

- Bugfix: version number not showing up in about box
- Bugfix: class list in index view not adding new classes
- Bugfix: bomb on adding new item due to no class selected
- Bugfix: currency export to csv failed due to commas in numbers
- Bugfix: pasting large amounts of text (~5000 lines) would crash the 
  program. hopefully fixed!? 



Version 1.2b (2006-03-01)

- Index view lets you filter by class
- Use Ctrl+D for strikethrough format
- Bugfix: Incompatibility of password protected files between Windows 
  2000 and XP.
  *Important* If you're on Windows XP and use password protection on 
  your files, please turn it off and resave the files before upgrading 
  to version 1.2b. You can turn it on again after the upgrade. 
  This had to do with the different platforms offering different 
  cryptography providers - NeoMem now defaults to using what is 
  effectively the Windows 2000 provider.

Version 1.2a (2006-02-20)

- Export entire file or selected object (recursively) to csv
- Uses an ini file instead of the registry for storing options
- Added File, Folder, Email, and Website property types, to replace 
   the Hyperlinks type (which is still available, though deprecated)
- Added Strikethrough format
- Added Format menu
- Insert date/time from Edit menu or F5 key
- Use Alt+, and Alt+. for back/forwards
- Use Alt+Backspace for undo, in addition to Ctrl+Z
- Various bug fixes


Version 1.1c (2005-02-26)

  Mainly a release to work out automation of build and release 
  procedures, with a minor diagnostic message added. 

- Adds diagnostic message if clicking on hyperlink in Text View 
  fails 

Version 1.1b (2004-12-15)

- Switched back to Ctrl+V as Paste, Ctrl+Alt+V as Paste Plain
- Fixed problem with Edit Link dialog when switching from single 
  to multiple links

Version 1.1a (2004-08-23)

- Password Protection and Encryption for files
- AutoRecover - Save recovery information at specified intervals
- AutoBackup - Keeps backup copy when saving file
- Text View remembers cursor position
- Ability to add more columns to Search and Index Views
- Automatic hyperlinks in Text View to websites, email, files, etc. 
  (eg www.somewhere.org, mailto:someone@somewhere.org, 
  file:c:\bootlog.txt)
- Multilevel Undo and Redo in Text View
- Set background color for text in Text View
- Ctrl+M - Move selected text to another object
- Ctrl+E - Add a new Folder object
- Ctrl+G - Goto another object
- Ctrl+Alt+M - Minimize NeoMem to System Tray
- Ctrl+Alt+T - Select and apply new text color
- Ctrl+Alt+K - Select and apply new background color
- Ctrl+T - Apply current text color
- Ctrl+K - Apply current background color
- Ctrl+Spacebar - Remove formatting from current selection
- Ctrl+Enter - Select item in Contents View
- Apply formatting to word without highlighting the whole word 
  (eg Ctrl+B)
- Improved menus
- Improved color dialogs
- The Startup File feature has been simplified - now just set the 
  AutoLoad feature and NeoMem will load the last opened file on 
  startup.

--------------------------------------------------------------------
License
--------------------------------------------------------------------
NeoMem is free software, released under the MIT X11 License. 
The source code (Visual C++/MFC) is available on the website. 


Copyright (c) 2001-2006 NeoMem.org. All Rights Reserved.

