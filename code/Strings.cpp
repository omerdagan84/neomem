
// This file defines string constants


#include "precompiled.h"
#include "strings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



//, just leaving these as is for now - need to find/write regkey mover...
const TCHAR szRegKey[] = _T("Intaglio Software");
const TCHAR szRegKeyDebug[] = _T("Intaglio Software Debug");
//const TCHAR szRegKeyOld[] = _T("Intaglio Software");
//const TCHAR szRegKeyOldDebug[] = _T("Intaglio Software Debug");
//const TCHAR szRegKey[] = _T("NeoMem");
//const TCHAR szRegKeyDebug[] = _T("NeoMem Debug");

const TCHAR szSettings[] = _T("Settings");
const TCHAR szWindowPosition[] = _T("WindowPosition");
const TCHAR szDisplayHeader[] = _T("DisplayHeader");
const TCHAR szHeaderFont[] = _T("HeaderFont");
const TCHAR szHeaderBackgroundColor[] = _T("HeaderBackgroundColor");
const TCHAR szHeaderForegroundColor[] = _T("HeaderForegroundColor");
const TCHAR szCustomColors[] = _T("CustomColors");
const TCHAR szDisplayGridlines[] = _T("DisplayGridlines");
const TCHAR szRtfDefaultFont[] = _T("DefaultRtfFont");
const TCHAR szShowTips[] = _T("ShowTips");
const TCHAR szLastTip[] = _T("LastTip");
// 1.1d not used anymore // const TCHAR szAdmin[] = _T("Admin");
const TCHAR szNameFormat[] = _T("NameFormat");
const TCHAR szDateFormat[] = _T("DateFormat");
const TCHAR szDocumentFolder[] = _T("DocumentFolder");
// 1.1 const TCHAR szRegistrationName[] = _T("RegistrationName");
// 1.1 const TCHAR szRegistrationKey[] = _T("RegistrationKey");
const TCHAR szShowWelcome[] = _T("ShowStartScreen");
const TCHAR szAutoLoad[] = _T("AutoLoad");
const TCHAR szAutoLoadFileName[] = _T("AutoLoadFileName");
//const TCHAR szAddAsSibling[] = _T("AddAsSibling");
//const TCHAR szSelectNewObject[] = _T("SelectNewObject");
const TCHAR szTemplateFileName[] = _T("TemplateFileName");
const TCHAR szAutoStart[] = _T("AutoStart");
const TCHAR szAutoRecover[] = _T("AutoRecover");
const TCHAR szAutoRecoverMinutes[] = _T("AutoRecoverMinutes");
const TCHAR szAutoBackup[] = _T("AutoBackup");
// 1.1 const TCHAR szInstallDate[] = _T("InstallDate");
const TCHAR szMarginLeft[] = _T("MarginLeft");
const TCHAR szMarginTop[] = _T("MarginTop");
const TCHAR szMarginRight[] = _T("MarginRight");
const TCHAR szMarginBottom[] = _T("MarginBottom");
const TCHAR szTextViewForecolor[] = _T("TextViewForecolor");
const TCHAR szTextViewBackcolor[] = _T("TextViewBackcolor");
const TCHAR szPrintHeaderFooter[] = _T("PrintHeaderFooter");
const TCHAR szExportFolder[] = _T("ExportFolder");
const TCHAR szExportFormat[] = _T("ExportFormat");
const TCHAR szRegisterFileType[] = _T("RegisterFileType");


//const TCHAR szExplorerKey[] = _T("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer");
//const TCHAR szExplorerKey[] = _T("Microsoft\\Windows\\CurrentVersion\\Explorer");
const TCHAR szShellFolders[] = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders");
const TCHAR szWindowsRun[] = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
const TCHAR szCentralProcessor[] = _T("Hardware\\Description\\System\\CentralProcessor\\0");

// Make sure that these strings match the order of SF_* 
//static TCHAR szFilterLoad[] = TEXT("Text and Rich Text\0*.TXT;*.RTF\0Text Files (*.TXT)\0*.TXT\0Rich Text Format (*.RTF)\0*.RTF\0All Files\0*.*\0\0"); 
//static TCHAR szFilterSave[] = TEXT("Text Files (*.TXT)\0*.TXT\0Rich Text Format (*.RTF)\0*.RTF\0RTF w/o Objects (*.RTF)\0*.RTF\0Textized\0*.TXT\0\0"); 
//const TCHAR szExportFilter[] = _T("Rich Text Format Files (*.rtf)|*.rtf|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
//const TCHAR szExportFilter2[] = _T("Rich Text Format Files (*.rtf)|*.rtf|Text Files (*.txt)|*.txt|NeoMem Files (*.neo)|*.neo|All Files (*.*)|*.*||");
//const TCHAR szExportFilter[] = _T("Rich Text Format Files (*.rtf)|*.rtf|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
//const TCHAR szImportFilter[] = _T("Rich Text Format Files (*.rtf)|*.rtf|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
const TCHAR szIconFilter[] = _T("Icon Files (*.ico)|*.ico|All Files (*.*)|*.*||");
const TCHAR szNeoMemFilter[] = _T("NeoMem Files (*.neo)|*.neo|All Files (*.*)|*.*||");



//. out of memory error strings should be here, since you don't want to have to load it from the resource file!
/*
static ASSENSZ rgassenszErrors[] = 
{ 
{EN_ERRSPACE, "Out of memory. Exit some other applications and try again."}, 
{EN_MAXTEXT, "The maximum text length has been reached."}, 
{0, NULL} 
}; 
*/

