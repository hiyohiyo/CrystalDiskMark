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

#define MENU_THEME_INDEX			3
#define MENU_LANG_INDEX				5

#define DEFAULT_THEME				THEME_DIR _T("Default")
#define DEFAULT_LANGUAGE			LANGUAGE_DIR _T("English.lang")

class CDiskMarkApp : public CWinApp
{
public:
	CDiskMarkApp();

	CString m_MainDlgPath;
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