; NeoMem.nsi
; This is an NSIS (Nullsoft Scriptable Install System) script.
;-------------------------------------------------------------

; decided it was safer to have this build NeoMemSetup.exe each time, 
; then let user rename it as needed. otherwise might accidentally 
; overwrite old setup versions.

; This just gets displayed in the dialogs. 
; Note: this is updated by the rakefile. so don't muck with it. 
; oh, you can just pass this by the cmd line to makensis, ie /DVERSION=1.2c
!define VERSION "1.2c"        

; The name of the installer
Name "NeoMem ${VERSION}" ; automatically includes " Setup" at end
XPStyle on
ShowInstDetails show
ShowUninstDetails show

; The name of the setup file to write
OutFile "NeoMemSetup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\NeoMem

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\NeoMem" "InstallDir"

;--------------------------------

; Pages

;Page components
;Page custom IntroPage
;PageEx license
;	Caption " - Readme"
;  LicenseText "NeoMem Readme" "Next"
;	; the file must be in DOS text format (\r\n)
;  LicenseData "..\setup\readme.txt" 
;PageExEnd
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles



;Function IntroPage
;	show "hello????"
;FunctionEnd



;--------------------------------
; The stuff to install
;Section "" ;No components page, name is not important
Section "-NeoMem (required)" ; empty string makes it hidden, so would starting with -

	; preliminary
	; if intaglio software key found, move it to neomem!
;  DetailPrint "Moving Intaglio Software registry key to NeoMem"
;	Var i ;Declare the variable
;	StrCpy $i "0" ;Now you can use the variable $i
;	EnumRegKey
;	DeleteRegKey HKLM "Software\Intaglio Software\NeoMem"
;	DeleteRegValue
;	EnumRegValue


  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put files there
  File "..\NeoMemSetup\NeoMem.exe"
  File "..\NeoMemSetup\NeoMem.hlp"
  File "..\NeoMemSetup\NeoMem.cnt"
  File "..\NeoMemSetup\Template.neo"
  File "..\NeoMemSetup\Example.neo"
  File "..\NeoMemSetup\Readme.txt"
  File "..\NeoMemSetup\License.txt"

	;Create desktop shortcut
  CreateShortCut "$DESKTOP\NeoMem.lnk" "$INSTDIR\NeoMem.exe" ""

  ; Write the installation path into the registry
  WriteRegStr HKLM "Software\NeoMem" "InstallDir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NeoMem" "DisplayName" "NeoMem"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NeoMem" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NeoMem" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NeoMem" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "-Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\NeoMem"
  CreateShortCut "$SMPROGRAMS\NeoMem\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\NeoMem\NeoMem.lnk" "$INSTDIR\neomem.exe" "" "$INSTDIR\neomem.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NeoMem"
  DeleteRegKey HKLM "Software\Intaglio Software\NeoMem"
  DeleteRegKey HKLM "Software\NeoMem"

  ; Remove files and uninstaller
  Delete $INSTDIR\neomem.exe
  Delete $INSTDIR\neomem.cnt
  Delete $INSTDIR\neomem.hlp
  Delete $INSTDIR\template.neo
  Delete $INSTDIR\example.neo
  Delete $INSTDIR\readme.txt
  Delete $INSTDIR\license.txt
  Delete $INSTDIR\uninstall.exe

  ; Remove start menu shortcuts, if any
  Delete "$SMPROGRAMS\NeoMem\*.*"

	; Remove desktop Shortcut
  Delete "$DESKTOP\NeoMem.lnk"

  ; Remove directories used
  RMDir "$SMPROGRAMS\NeoMem"
  RMDir "$INSTDIR"

SectionEnd

