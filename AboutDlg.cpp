/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The MIT License
//
//                                             Copyright (c) 2007-2015 hiyohiyo
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskMark.h"
#include "AboutDlg.h"

IMPLEMENT_DYNCREATE(CAboutDlg, CDHtmlDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialogEx(CAboutDlg::IDD, CAboutDlg::IDH, pParent)
{
	m_CurrentLangPath = ((CDHtmlMainDialog*)pParent)->m_CurrentLangPath;
	m_DefaultLangPath = ((CDHtmlMainDialog*)pParent)->m_DefaultLangPath;
	m_ZoomType = ((CDHtmlMainDialog*)pParent)->GetZoomType();
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialogEx::DoDataExchange(pDX);

	DDX_DHtml_ElementInnerText(pDX, _T("Version"), m_Version);
	DDX_DHtml_ElementInnerText(pDX, _T("Edition"), m_Edition);
	DDX_DHtml_ElementInnerText(pDX, _T("Release"), m_Release);
	DDX_DHtml_ElementInnerHtml(pDX, _T("Copyright"), m_Copyright);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDHtmlDialogEx::OnInitDialog();

	SetWindowText(i18n(_T("WindowTitle"), _T("ABOUT")));

	EnableDpiAware();
	InitDHtmlDialog(SIZE_X, SIZE_Y, ((CDiskMarkApp*)AfxGetApp())->m_AboutDlgPath);

	return TRUE;
}

void CAboutDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CString cstr;
	cstr = szUrl;
	if(cstr.Find(_T("html")) != -1 || cstr.Find(_T("dlg")) != -1)
	{
		m_FlagShowWindow = TRUE;
		m_Version = PRODUCT_VERSION;
		m_Edition = PRODUCT_EDITION;
		m_Release = PRODUCT_RELEASE;
		m_Copyright = PRODUCT_COPYRIGHT;

		ChangeZoomType(m_ZoomType);
		SetClientRect((DWORD)(SIZE_X * m_ZoomRatio), (DWORD)(SIZE_Y * m_ZoomRatio), 0);

		UpdateData(FALSE);
		CenterWindow();
		ShowWindow(SW_SHOW);
	}
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDHtmlDialogEx)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CAboutDlg)
	DHTML_EVENT_ONCLICK(_T("CrystalDewWorld"), OnCrystalDewWorld)
	DHTML_EVENT_ONCLICK(_T("License"), OnLicense)
	DHTML_EVENT_ONCLICK(_T("DiskSpd"), OnDiskSpd)
#ifdef SUISHO_SHIZUKU_SUPPORT
	DHTML_EVENT_ONCLICK(_T("ProjectShizuku"), OnProjectShizuku)
#endif
END_DHTML_EVENT_MAP()

HRESULT CAboutDlg::OnCrystalDewWorld(IHTMLElement* /*pElement*/)
{
	if(GetUserDefaultLCID() == 0x0411)// Japanese
	{
		OpenUrl(URL_CRYSTAL_DEW_WORLD_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_CRYSTAL_DEW_WORLD_EN);
	}

	return S_FALSE;
}

HRESULT CAboutDlg::OnLicense(IHTMLElement* /*pElement*/)
{
	if (GetUserDefaultLCID() == 0x0411)// Japanese
	{
		OpenUrl(URL_CDM_LICENSE_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_CDM_LICENSE_EN);
	}

	return S_FALSE;
}

HRESULT CAboutDlg::OnDiskSpd(IHTMLElement* /*pElement*/)
{
	OpenUrl(URL_DISKSPD);

	return S_FALSE;
}

#ifdef SUISHO_SHIZUKU_SUPPORT
HRESULT CAboutDlg::OnProjectShizuku(IHTMLElement* /*pElement*/)
{
	if(GetUserDefaultLCID() == 0x0411)// Japanese
	{
		OpenUrl(URL_PROJECT_SHIZUKU_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_PROJECT_SHIZUKU_EN);
	}

	return S_FALSE;
}
#endif