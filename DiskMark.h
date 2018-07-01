/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#include "resource.h"


#define THEME_DIR					_T("CdmResource\\themes\\")
#define LANGUAGE_DIR				_T("CdmResource\\language\\")
#define DIALOG_DIR					_T("CdmResource\\dialog\\")

#define MENU_THEME_INDEX			2
#define MENU_LANG_INDEX				4

#ifdef SUISHO_SHIZUKU_SUPPORT
#define MAIN_DIALOG					_T("MainS.html")
#define MAIN_DIALOG_IE8				_T("MainS8.html")
#define ABOUT_DIALOG				_T("AboutS.html")
#define DEFAULT_THEME				THEME_DIR _T("Shizuku\\MainS.css")
#define RECOMMENDED_THEME			_T("ShizukuLiteratureGirl")
#else
#define MAIN_DIALOG					_T("Main.html")
#define MAIN_DIALOG_IE8				_T("Main8.html")
#define ABOUT_DIALOG				_T("About.html")
#define DEFAULT_THEME				THEME_DIR _T("default\\Main.css")
#endif

#define SETTINGS_DIALOG				_T("Settings.html")
#define DEFAULT_LANGUAGE			LANGUAGE_DIR _T("English.lang")

class CDiskMarkApp : public CWinApp
{
public:
	CDiskMarkApp();

	CString m_MainDlgPath;
	CString m_MainDlgPathIE8;
	CString m_AboutDlgPath;
	CString m_SettingsDlgPath;
	CString m_ThemeDir;
	CString m_LangDir;
	DWORD m_ThemeIndex;
	DWORD m_LangIndex;

	CString m_ExeDir;
	CString m_Ini;

public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
};

extern CDiskMarkApp theApp;