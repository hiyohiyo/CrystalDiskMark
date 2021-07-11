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
	CMainDialogFx* p = (CMainDialogFx*)pParent;

	m_ZoomType = p->GetZoomType();
	m_FontScale = p->GetFontScale();
	m_FontRatio = p->GetFontRatio();
	m_FontFace = p->GetFontFace();
	m_FontRender = p->GetFontRender();
	m_CurrentLangPath = p->GetCurrentLangPath();
	m_DefaultLangPath = p->GetDefaultLangPath();
	m_ThemeDir = p->GetThemeDir();
	m_CurrentTheme = p->GetCurrentTheme();
	m_DefaultTheme = p->GetDefaultTheme();
	m_Ini = p->GetIniPath();

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_BackgroundName = L"About";
#else
	m_BackgroundName = L"";
#endif
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
	DDX_Control(pDX, IDC_COPYRIGHT1, m_CtrlCopyright1);
	DDX_Control(pDX, IDC_COPYRIGHT2, m_CtrlCopyright2);
	DDX_Control(pDX, IDC_EDITION, m_CtrlEdition);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogFx::OnInitDialog();

	SetWindowTitle(i18n(L"WindowTitle", L"ABOUT"));

	m_bShowWindow = TRUE;
	m_CtrlVersion.SetWindowTextW(PRODUCT_NAME L" " PRODUCT_VERSION);
	m_CtrlEdition.SetWindowTextW(PRODUCT_EDITION);
	m_CtrlRelease.SetWindowTextW(L"Release: " PRODUCT_RELEASE);
	m_CtrlCopyright1.SetWindowTextW(PRODUCT_COPYRIGHT_1);
	m_CtrlCopyright2.SetWindowTextW(PRODUCT_COPYRIGHT_2);
	m_CtrlLicense.SetWindowTextW(PRODUCT_LICENSE);

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
	ON_BN_CLICKED(IDC_PROJECT_SITE_1, &CAboutDlg::OnProjectSite1)
	ON_BN_CLICKED(IDC_PROJECT_SITE_2, &CAboutDlg::OnProjectSite2)
	ON_BN_CLICKED(IDC_PROJECT_SITE_3, &CAboutDlg::OnProjectSite3)
	ON_BN_CLICKED(IDC_PROJECT_SITE_4, &CAboutDlg::OnProjectSite4)
	ON_BN_CLICKED(IDC_PROJECT_SITE_5, &CAboutDlg::OnProjectSite5)
#endif
END_MESSAGE_MAP()


void CAboutDlg::UpdateDialogSize()
{
	CDialogFx::UpdateDialogSize();
	m_bHighContrast = FALSE;

	ChangeZoomType(m_ZoomType);
	SetClientSize(SIZE_X, SIZE_Y, m_ZoomRatio);
	UpdateBackground(TRUE, m_bDarkMode);

#ifdef TSUKUMO_TOKKA_SUPPORT
	m_CtrlCrystalDewWorld.InitControl(20, 20, 128, 128, m_ZoomRatio, &m_BkDC, IP(L"Logo"), 1, BS_CENTER, OwnerDrawImage, FALSE, FALSE, FALSE);
	m_CtrlProjectSite1.ShowWindow(SW_HIDE);
	m_CtrlProjectSite2.ShowWindow(SW_HIDE);
	m_CtrlProjectSite3.ShowWindow(SW_HIDE);
	m_CtrlProjectSite4.ShowWindow(SW_HIDE);
	m_CtrlProjectSite5.ShowWindow(SW_HIDE);
#elif SUISHO_SHIZUKU_SUPPORT
	m_CtrlProjectSite1.InitControl(64, 372, 140, 16, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlProjectSite2.InitControl(64, 416, 148, 16, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlProjectSite3.InitControl(64, 432, 184, 16, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlProjectSite4.InitControl(40, 460, 208, 16, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlProjectSite5.InitControl(92, 504, 432, 124, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlCrystalDewWorld.InitControl(80, 12, 128, 128, m_ZoomRatio, &m_BkDC, IP(L"Logo"), 1, BS_CENTER, OwnerDrawImage, FALSE, FALSE, FALSE);
	m_CtrlProjectSite1.SetHandCursor();
	m_CtrlProjectSite2.SetHandCursor();
	m_CtrlProjectSite3.SetHandCursor();
	m_CtrlProjectSite4.SetHandCursor();
	m_CtrlProjectSite5.SetHandCursor();
#else
	m_CtrlCrystalDewWorld.InitControl(12, 12, 128, 128, m_ZoomRatio, &m_BkDC, IP(L"Logo"), 1, BS_CENTER, OwnerDrawImage, FALSE, FALSE, FALSE);
	m_CtrlProjectSite1.ShowWindow(SW_HIDE);
	m_CtrlProjectSite2.ShowWindow(SW_HIDE);
	m_CtrlProjectSite3.ShowWindow(SW_HIDE);
	m_CtrlProjectSite4.ShowWindow(SW_HIDE);
	m_CtrlProjectSite5.ShowWindow(SW_HIDE); 
#endif

	m_CtrlCrystalDewWorld.SetHandCursor();

	m_CtrlVersion.SetFontEx(m_FontFace, 22, 22, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_BOLD, m_FontRender);
	m_CtrlEdition.SetFontEx(m_FontFace, 22, 22, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_BOLD, m_FontRender);
	m_CtrlRelease.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlCopyright1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlCopyright2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlLicense.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);

	m_CtrlVersion.SetHandCursor();
	m_CtrlLicense.SetHandCursor();

#ifdef TSUKUMO_TOKKA_SUPPORT
	m_CtrlVersion.InitControl(152, 12, 476, 28, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlEdition.InitControl(152, 40, 476, 28, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlRelease.InitControl(152, 72, 476, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlCopyright1.InitControl(152, 96, 476, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlCopyright2.InitControl(152, 116, 476, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlLicense.InitControl(152, 136, 476, 20, m_ZoomRatio, & m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
#elif SUISHO_SHIZUKU_SUPPORT
	m_CtrlVersion.InitControl(0, 152, 288, 28, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlEdition.InitControl(0, 180, 288, 28, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlRelease.InitControl(0, 216, 288, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlCopyright1.InitControl(0, 236, 288, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlCopyright2.InitControl(0, 256, 288, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
	m_CtrlLicense.InitControl(0, 276, 288, 20, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, FALSE, FALSE, FALSE);
#else
	m_CtrlVersion.InitControl(140, 12, 340, 28, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlEdition.InitControl(140, 40, 340, 28, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlRelease.InitControl(140, 76, 340, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlCopyright1.InitControl(140, 100, 340, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlLicense.InitControl(140, 120, 340, 20, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlCopyright2.ShowWindow(SW_HIDE);
#endif

	Invalidate();
}

void CAboutDlg::OnCrystalDewWorld()
{
	if (GetUserDefaultLCID() == 0x0411)// Japanese
	{
		OpenUrl(URL_MAIN_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_MAIN_EN);
	}
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
void CAboutDlg::OnProjectSite1()
{
	OpenUrl(URL_PROJECT_SITE_1);
}

void CAboutDlg::OnProjectSite2()
{
	OpenUrl(URL_PROJECT_SITE_2);
}

void CAboutDlg::OnProjectSite3()
{
	OpenUrl(URL_PROJECT_SITE_3);
}

void CAboutDlg::OnProjectSite4()
{
	OpenUrl(URL_PROJECT_SITE_4);
}

void CAboutDlg::OnProjectSite5()
{
	OpenUrl(URL_PROJECT_SITE_5);
}
#endif