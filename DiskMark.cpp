﻿/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskMark.h"
#include "DiskMarkDlg.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <afxole.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDiskMarkApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CDiskMarkApp::CDiskMarkApp()
{
}

CDiskMarkApp theApp;

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
static BOOL IsFileExistEx(const TCHAR* path, const TCHAR* fileName);
static BOOL RunAsRestart();

BOOL CDiskMarkApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

#ifndef UWP
	if (! IsUserAnAdmin())
	{
		if (RunAsRestart())
		{
			return FALSE;
		}
	}
#endif

	// Multimedia Timer Setting
	TIMECAPS tc;
	timeGetDevCaps(&tc,sizeof(TIMECAPS));
	timeBeginPeriod(tc.wPeriodMin);

	BOOL flagReExec = FALSE;

	CDiskMarkDlg dlg;
	m_pMainWnd = &dlg;

	if (dlg.DoModal() == RE_EXEC)
	{
		flagReExec = TRUE;
	}

	timeEndPeriod(tc.wPeriodMin);

	if(flagReExec)
	{
		TCHAR str[MAX_PATH];
		::GetModuleFileName(NULL, str, MAX_PATH);
		ShellExecute(NULL, NULL, str, NULL, NULL, SW_SHOWNORMAL);
	}

	return FALSE;
}

BOOL IsFileExistEx(const TCHAR* path, const TCHAR* fileName)
{
	if(! IsFileExist(path))
	{
		CString cstr;
		cstr.Format(L"Not Found \"%s\".", fileName); 
		AfxMessageBox(cstr);
		return FALSE;
	}
	return TRUE;
}

BOOL RunAsRestart()
{
	int count;

	TCHAR** cmd = ::CommandLineToArgvW(::GetCommandLine(), &count);

	if (count < 2 || _tcscmp(cmd[1], L"runas") != 0)
	{
		TCHAR path[MAX_PATH];
		::GetModuleFileName(NULL, path, MAX_PATH);
		if (::ShellExecute(NULL, L"runas", path, L"runas", NULL, SW_SHOWNORMAL) > (HINSTANCE)32)
		{
			return TRUE;
		}
	}
	return FALSE;
}