/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskMark.h"
#include "DiskMarkDlg.h"
#include "AboutDlg.h"

IMPLEMENT_DYNCREATE(CAboutDlg, CDialog)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialogFx(CAboutDlg::IDD, pParent)
{
	CMainDialog* p = (CMainDialog*)pParent;

	m_ZoomType = p->GetZoomType();
	m_FontScale = p->GetFontScale();
	m_FontRatio = p->GetFontRatio();
	m_FontFace = p->GetFontFace();
	m_CurrentLangPath = p->GetCurrentLangPath();
	m_DefaultLangPath = p->GetDefaultLangPath();
	m_ThemeDir = p->GetThemeDir();
	m_CurrentTheme = p->GetCurrentTheme();
	m_DefaultTheme = p->GetDefaultTheme();
	m_Ini = p->GetIniPath();

	m_BackgroundName = L"About";
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogFx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CRYSTAL_DEW_WORLD, m_CtrlCrystalDewWorld);

	DDX_Control(pDX, IDC_PROJECT_SITE_1, m_CtrlProjectSite1);
	DDX_Control(pDX, IDC_PROJECT_SITE_2, m_CtrlProjectSite2);
	DDX_Control(pDX, IDC_PROJECT_SITE_3, m_CtrlProjectSite3);
	DDX_Control(pDX, IDC_PROJECT_SITE_4, m_CtrlProjectSite4);
	DDX_Control(pDX, IDC_PROJECT_SITE_5, m_CtrlProjectSite5);
	DDX_Control(pDX, IDC_VERSION, m_CtrlVersion);
	DDX_Control(pDX, IDC_LICENSE, m_CtrlLicense);
	
	DDX_Control(pDX, IDC_RELEASE, m_CtrlRelease);
	DDX_Control(pDX, IDC_COPYRIGHT, m_CtrlCopyright);
	DDX_Control(pDX, IDC_EDITION, m_CtrlEdition);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogFx::OnInitDialog();

	SetWindowText(i18n(L"WindowTitle", L"ABOUT"));

	m_bShowWindow = TRUE;
	m_CtrlVersion.SetWindowTextW(PRODUCT_NAME L" " PRODUCT_VERSION);
	m_CtrlEdition.SetWindowTextW(PRODUCT_EDITION);
	m_CtrlRelease.SetWindowTextW(L"Release: " PRODUCT_RELEASE);
	m_CtrlCopyright.SetWindowTextW(PRODUCT_COPYRIGHT);
	m_CtrlLicense.SetWindowTextW(PRODUCT_LICENSE);

#ifndef SUISHO_SHIZUKU_SUPPORT
	m_CtrlProjectSite1.ShowWindow(SW_HIDE);
	m_CtrlProjectSite2.ShowWindow(SW_HIDE);
	m_CtrlProjectSite3.ShowWindow(SW_HIDE);
	m_CtrlProjectSite4.ShowWindow(SW_HIDE);
	m_CtrlProjectSite5.ShowWindow(SW_HIDE);
#endif

	UpdateDialogSize();

	CenterWindow();
	ShowWindow(SW_SHOW);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogFx)
	ON_BN_CLICKED(IDC_CRYSTAL_DEW_WORLD, &CAboutDlg::OnCrystalDewWorld)
	ON_BN_CLICKED(IDC_LICENSE, &CAboutDlg::OnLicense)
	ON_BN_CLICKED(IDC_VERSION, &CAboutDlg::OnVersion)
#ifdef SUISHO_SHIZUKU_SUPPORT
	ON_BN_CLICKED(IDC_PROJECT_SITE_1, &CAboutDlg::OnBnClickedProjectSite1)
	ON_BN_CLICKED(IDC_PROJECT_SITE_2, &CAboutDlg::OnBnClickedProjectSite2)
	ON_BN_CLICKED(IDC_PROJECT_SITE_3, &CAboutDlg::OnBnClickedProjectSite3)
	ON_BN_CLICKED(IDC_PROJECT_SITE_4, &CAboutDlg::OnBnClickedProjectSite4)
	ON_BN_CLICKED(IDC_PROJECT_SITE_5, &CAboutDlg::OnBnClickedProjectSite5)
#endif
END_MESSAGE_MAP()


void CAboutDlg::UpdateDialogSize()
{
	ChangeZoomType(m_ZoomType);
	if (m_bHighContrast)
	{
		SetClientSize((int)(SIZE_X * m_ZoomRatio), (int)(SIZE_HC_Y * m_ZoomRatio), 0);
	}
	else
	{
		SetClientSize((int)(SIZE_X * m_ZoomRatio), (int)(SIZE_Y * m_ZoomRatio), 0);
	}

	UpdateBackground(TRUE);

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_CtrlProjectSite1.InitControl(340, 324, 292, 20, m_ZoomRatio, &m_BgDC, NULL, 0, 0, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlProjectSite2.InitControl(340, 368, 292, 20, m_ZoomRatio, &m_BgDC, NULL, 0, 0, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlProjectSite3.InitControl(340, 392, 292, 20, m_ZoomRatio, &m_BgDC, NULL, 0, 0, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlProjectSite4.InitControl(340, 416, 292, 20, m_ZoomRatio, &m_BgDC, NULL, 0, 0, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlProjectSite5.InitControl(320, 192, 292, 92, m_ZoomRatio, &m_BgDC, NULL, 0, 0, OwnerDrawTransparent | m_bHighContrast);

	m_CtrlProjectSite1.SetHandCursor();
	m_CtrlProjectSite2.SetHandCursor();
	m_CtrlProjectSite3.SetHandCursor();
	m_CtrlProjectSite4.SetHandCursor();
	m_CtrlProjectSite5.SetHandCursor();
#endif

	m_CtrlCrystalDewWorld.InitControl(12, 20, 128, 128, m_ZoomRatio, &m_BgDC, IP(L"Logo"), 1, BS_CENTER, OwnerDrawImage | m_bHighContrast);
	m_CtrlCrystalDewWorld.SetHandCursor();

	m_CtrlVersion.SetFontEx(m_FontFace, 24, 24, m_ZoomRatio, m_FontRatio, 255, RGB(0, 0, 0), FW_BOLD);
	m_CtrlEdition.SetFontEx(m_FontFace, 24, 24, m_ZoomRatio, m_FontRatio, 255, RGB(0, 0, 0), FW_BOLD);
	m_CtrlRelease.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, 255, RGB(0, 0, 0), FW_NORMAL);
	m_CtrlCopyright.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, 255, RGB(0, 0, 0), FW_NORMAL);
	m_CtrlLicense.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, 255, RGB(0, 0, 0), FW_NORMAL);

	m_CtrlVersion.SetHandCursor();
	m_CtrlLicense.SetHandCursor();

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_CtrlVersion.InitControl(152, 12, 476, 36, m_ZoomRatio, &m_BgDC, NULL, 0, BS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlEdition.InitControl(152, 48, 476, 36, m_ZoomRatio, &m_BgDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlRelease.InitControl(152, 88, 476, 24, m_ZoomRatio, &m_BgDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlCopyright.InitControl(152, 112, 476, 24, m_ZoomRatio, &m_BgDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlLicense.InitControl(152, 136, 476, 24, m_ZoomRatio, &m_BgDC, NULL, 0, BS_CENTER, OwnerDrawTransparent | m_bHighContrast);
#else
	m_CtrlVersion.InitControl(152, 12, 316, 36, m_ZoomRatio, &m_BgDC, NULL, 0, BS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlEdition.InitControl(152, 48, 316, 36, m_ZoomRatio, &m_BgDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlRelease.InitControl(152, 88, 316, 24, m_ZoomRatio, &m_BgDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlCopyright.InitControl(152, 112, 316, 24, m_ZoomRatio, &m_BgDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_CtrlLicense.InitControl(152, 136, 316, 24, m_ZoomRatio, &m_BgDC, NULL, 0, BS_CENTER, OwnerDrawTransparent | m_bHighContrast);
#endif

	Invalidate();
}

void CAboutDlg::OnCrystalDewWorld()
{

#ifdef KUREI_KEI_SUPPORT
	OpenUrl(URL_PRONAMA);
#else if
	if (GetUserDefaultLCID() == 0x0411)// Japanese
	{
		OpenUrl(URL_CRYSTAL_DEW_WORLD_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_CRYSTAL_DEW_WORLD_EN);
	}
#endif
}

void CAboutDlg::OnVersion()
{
	if (GetUserDefaultLCID() == 0x0411)// Japanese
	{
		OpenUrl(URL_VERSION_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_VERSION_EN);
	}

}
void CAboutDlg::OnLicense()
{
	if (GetUserDefaultLCID() == 0x0411)// Japanese
	{
		OpenUrl(URL_LICENSE_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_LICENSE_EN);
	}
}

#ifdef SUISHO_SHIZUKU_SUPPORT
void CAboutDlg::OnBnClickedProjectSite1()
{
	OpenUrl(URL_PROJECT_SITE_1);
}

void CAboutDlg::OnBnClickedProjectSite2()
{
	OpenUrl(URL_PROJECT_SITE_2);
}

void CAboutDlg::OnBnClickedProjectSite3()
{
	OpenUrl(URL_PROJECT_SITE_3);
}

void CAboutDlg::OnBnClickedProjectSite4()
{
	OpenUrl(URL_PROJECT_SITE_4);
}

void CAboutDlg::OnBnClickedProjectSite5()
{
	OpenUrl(URL_PROJECT_SITE_5);
}
#endif