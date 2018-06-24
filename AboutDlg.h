/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The MIT License
//
//                                             Copyright (c) 2007-2015 hiyohiyo
/*---------------------------------------------------------------------------*/

#pragma once
#include "DialogCx.h"
#include "StaticCx.h"
#include "ButtonCx.h"

class CAboutDlg : public CDialogCx
{
	DECLARE_DYNCREATE(CAboutDlg)

#ifdef SUISHO_SHIZUKU_SUPPORT
	static const int SIZE_X = 640;
	static const int SIZE_Y = 660;
#else
	static const int SIZE_X = 480;
	static const int SIZE_Y = 160;
#endif

public:
	CAboutDlg(CWnd* pParent = NULL);
	virtual ~CAboutDlg();

	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
//	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);

	CString m_Version;
	CString m_Edition;
	CString m_Release;
	CString m_Copyright;;

	HRESULT OnCrystalDewWorld(IHTMLElement *pElement);
	HRESULT OnDiskSpd(IHTMLElement *pElement);
	HRESULT OnLicense(IHTMLElement *pElement);
#ifdef SUISHO_SHIZUKU_SUPPORT
	HRESULT OnProjectShizuku(IHTMLElement *pElement);
#endif
	DECLARE_MESSAGE_MAP()
};
