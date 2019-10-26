/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskMark.h"
#include "DiskMarkDlg.h"
#include "DiskBench.h"
#include "AboutDlg.h"
#include "GetFileVersion.h"
#include "GetOsInfo.h"

#include <math.h>
#include <exdispid.h>

#include "DialogCx.h"
#include "MainDialog.h"

#include "ButtonCx.h"
#include "StaticCx.h"

#ifdef SUISHO_SHIZUKU_SUPPORT
#define SIZE_X		1000
#define SIZE_Y		500
#define SIZE_MIN_Y	500
#define OFFSET_X    200
#define MAX_METER_LENGTH	320
#else
#define SIZE_X		480
#define SIZE_X_MIX	680
#define SIZE_Y		300
#define SIZE_MIN_Y	300
#define OFFSET_X    0
#define MAX_METER_LENGTH	192
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern PROCESS_INFORMATION pi;

CDiskMarkDlg::CDiskMarkDlg(CWnd* pParent /*=NULL*/)
	: CMainDialog(CDiskMarkDlg::IDD,
	((CDiskMarkApp*)AfxGetApp())->m_ThemeDir,
	((CDiskMarkApp*)AfxGetApp())->m_ThemeIndex,
	((CDiskMarkApp*)AfxGetApp())->m_LangDir,
	((CDiskMarkApp*)AfxGetApp())->m_LangIndex,
	pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconMini = AfxGetApp()->LoadIcon(IDI_TRAY_ICON);

	m_AboutDlg = NULL;
	m_SettingsDlg = NULL;
	m_CommentDlg = NULL;

	m_ExeDir = ((CDiskMarkApp*)AfxGetApp())->m_ExeDir;
	_tcscpy_s(m_Ini, MAX_PATH, ((CDiskMarkApp*)AfxGetApp())->m_Ini);

	m_AdminMode = IsUserAnAdmin();
	m_EditBrush = new CBrush;
}

CDiskMarkDlg::~CDiskMarkDlg()
{
	if (m_EditBrush != NULL)
	{
		m_EditBrush->DeleteObject();
		delete m_EditBrush;
	}
}

void CDiskMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CMainDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_ALL, m_ButtonAll);
	DDX_Control(pDX, IDC_BUTTON_TEST_1, m_ButtonTest1);
	DDX_Control(pDX, IDC_BUTTON_TEST_2, m_ButtonTest2);
	DDX_Control(pDX, IDC_BUTTON_TEST_3, m_ButtonTest3);
	DDX_Control(pDX, IDC_BUTTON_TEST_4, m_ButtonTest4);

	DDX_Control(pDX, IDC_TEST_READ_1, m_TestRead1);
	DDX_Control(pDX, IDC_TEST_READ_2, m_TestRead2);
	DDX_Control(pDX, IDC_TEST_READ_3, m_TestRead3);
	DDX_Control(pDX, IDC_TEST_READ_4, m_TestRead4);

	DDX_Control(pDX, IDC_TEST_WRITE_1, m_TestWrite1);
	DDX_Control(pDX, IDC_TEST_WRITE_2, m_TestWrite2);
	DDX_Control(pDX, IDC_TEST_WRITE_3, m_TestWrite3);
	DDX_Control(pDX, IDC_TEST_WRITE_4, m_TestWrite4);

#ifdef MIX_MODE
	DDX_Control(pDX, IDC_TEST_MIX_1, m_TestMix1);
	DDX_Control(pDX, IDC_TEST_MIX_2, m_TestMix2);
	DDX_Control(pDX, IDC_TEST_MIX_3, m_TestMix3);
	DDX_Control(pDX, IDC_TEST_MIX_4, m_TestMix4);
	DDX_Control(pDX, IDC_COMBO_MIX, m_ComboMix);
#endif

	DDX_Control(pDX, IDC_COMMENT, m_Comment);
	DDX_Control(pDX, IDC_COMMENT_EX, m_CommentEx);

	DDX_Control(pDX, IDC_COMBO_COUNT, m_ComboCount);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_ComboSize);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_ComboDrive);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_ComboUnit);

	DDX_Control(pDX, IDC_READ_UNIT, m_ReadUnit);
	DDX_Control(pDX, IDC_WRITE_UNIT, m_WriteUnit);

#ifdef MIX_MODE
	DDX_Control(pDX, IDC_MIX_UNIT, m_MixUnit);
#endif

	DDX_Text(pDX, IDC_COMBO_UNIT, m_ValueTestUnit);
	DDX_Text(pDX, IDC_COMBO_COUNT, m_ValueTestCount);
	DDX_Text(pDX, IDC_COMBO_SIZE, m_ValueTestSize);
	DDX_Text(pDX, IDC_COMBO_DRIVE, m_ValueTestDrive);
	DDX_CBIndex(pDX, IDC_COMBO_UNIT, m_IndexTestUnit);
	DDX_CBIndex(pDX, IDC_COMBO_COUNT, m_IndexTestCount);
	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_IndexTestSize);
	DDX_CBIndex(pDX, IDC_COMBO_DRIVE, m_IndexTestDrive);
#ifdef MIX_MODE
	DDX_CBIndex(pDX, IDC_COMBO_MIX, m_IndexTestMix);
#endif
}

BEGIN_MESSAGE_MAP(CDiskMarkDlg, CMainDialog)
	//}}AFX_MSG_MAP
#ifdef SUISHO_SHIZUKU_SUPPORT
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
#endif
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_FILE_EXIT, OnExit)
	ON_COMMAND(ID_HELP_ABOUT, OnAbout)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_MESSAGE(WM_USER_UPDATE_SCORE, OnUpdateScore)
	ON_MESSAGE(WM_USER_UPDATE_MESSAGE, OnUpdateMessage)
	ON_MESSAGE(WM_USER_EXIT_BENCHMARK, OnExitBenchmark)
	ON_MESSAGE(WM_USER_UPDATE_COMMENT, OnUpdateComment)

	ON_COMMAND(ID_ZOOM_100, &CDiskMarkDlg::OnZoom100)
	ON_COMMAND(ID_ZOOM_125, &CDiskMarkDlg::OnZoom125)
	ON_COMMAND(ID_ZOOM_150, &CDiskMarkDlg::OnZoom150)
	ON_COMMAND(ID_ZOOM_200, &CDiskMarkDlg::OnZoom200)
	ON_COMMAND(ID_ZOOM_250, &CDiskMarkDlg::OnZoom250)
	ON_COMMAND(ID_ZOOM_300, &CDiskMarkDlg::OnZoom300)
//	ON_COMMAND(ID_ZOOM_400, &CDiskMarkDlg::OnZoom400)
	ON_COMMAND(ID_ZOOM_AUTO, &CDiskMarkDlg::OnZoomAuto)

	ON_COMMAND(ID_INTERVAL_TIME_0, &CDiskMarkDlg::OnIntervalTime0)
	ON_COMMAND(ID_INTERVAL_TIME_1, &CDiskMarkDlg::OnIntervalTime1)
	ON_COMMAND(ID_INTERVAL_TIME_3, &CDiskMarkDlg::OnIntervalTime3)
	ON_COMMAND(ID_INTERVAL_TIME_5, &CDiskMarkDlg::OnIntervalTime5)
	ON_COMMAND(ID_INTERVAL_TIME_10, &CDiskMarkDlg::OnIntervalTime10)
	ON_COMMAND(ID_INTERVAL_TIME_30, &CDiskMarkDlg::OnIntervalTime30)
	ON_COMMAND(ID_INTERVAL_TIME_60, &CDiskMarkDlg::OnIntervalTime60)
	ON_COMMAND(ID_INTERVAL_TIME_180, &CDiskMarkDlg::OnIntervalTime180)
	ON_COMMAND(ID_INTERVAL_TIME_300, &CDiskMarkDlg::OnIntervalTime300)
	ON_COMMAND(ID_INTERVAL_TIME_600, &CDiskMarkDlg::OnIntervalTime600)

	ON_COMMAND(ID_HELP, &CDiskMarkDlg::OnHelp)
	ON_COMMAND(ID_HELP_CRYSTALDEWWORLD, &CDiskMarkDlg::OnCrystalDewWorld)
	ON_COMMAND(ID_MODE_DEFAULT, &CDiskMarkDlg::OnModeDefault)
	ON_COMMAND(ID_MODE_ALL0X00, &CDiskMarkDlg::OnModeAll0x00)
	ON_COMMAND(ID_PROFILE_DEFAULT, &CDiskMarkDlg::OnProfileDefault)
	ON_COMMAND(ID_PROFILE_REAL, &CDiskMarkDlg::OnProfileReal)
	ON_COMMAND(ID_PROFILE_PEAK, &CDiskMarkDlg::OnProfilePeak)
#ifdef MIX_MODE
	ON_COMMAND(ID_PROFILE_DEFAULT_MIX, &CDiskMarkDlg::OnProfileDefaultMix)
	ON_COMMAND(ID_PROFILE_REAL_MIX, &CDiskMarkDlg::OnProfileRealMix)
	ON_COMMAND(ID_PROFILE_PEAK_MIX, &CDiskMarkDlg::OnProfilePeakMix)
#endif

	//}}AFX_MSG_MAP
	ON_COMMAND(ID_RESULT_SAVE, &CDiskMarkDlg::OnResultSave)
	ON_COMMAND(ID_SETTINGS_QUEUESTHREADS, &CDiskMarkDlg::OnSettingsQueuesThreads)
	ON_COMMAND(ID_FONT_SETTING, &CDiskMarkDlg::OnFontSetting)
	ON_WM_NCCREATE()
	ON_MESSAGE(WM_QUERYENDSESSION, &CDiskMarkDlg::OnQueryEndSession)

	ON_BN_CLICKED(IDC_BUTTON_ALL, &CDiskMarkDlg::OnAll)
	ON_BN_CLICKED(IDC_BUTTON_TEST_1, &CDiskMarkDlg::OnTest1)
	ON_BN_CLICKED(IDC_BUTTON_TEST_2, &CDiskMarkDlg::OnTest2)
	ON_BN_CLICKED(IDC_BUTTON_TEST_3, &CDiskMarkDlg::OnTest3)
	ON_BN_CLICKED(IDC_BUTTON_TEST_4, &CDiskMarkDlg::OnTest4)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVE, &CDiskMarkDlg::OnCbnSelchangeComboDrive)
	ON_CBN_SELCHANGE(IDC_COMBO_UNIT, &CDiskMarkDlg::OnCbnSelchangeComboUnit)

	ON_CBN_CLOSEUP(IDC_COMBO_UNIT, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_COUNT, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_SIZE, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_DRIVE, &CDiskMarkDlg::MoveForcus)
#ifdef MIX_MODE
	ON_CBN_SELCHANGE(IDC_COMBO_MIX, &CDiskMarkDlg::OnCbnSelchangeComboMix)
	ON_CBN_CLOSEUP(IDC_COMBO_MIX, &CDiskMarkDlg::MoveForcus)
#endif

	ON_EN_UPDATE(IDC_COMMENT, &CDiskMarkDlg::OnChangeComment)
	ON_BN_CLICKED(IDC_COMMENT_EX, &CDiskMarkDlg::OnCommentEx)
//	ON_EN_KILLFOCUS(IDC_COMMENT, &CDiskMarkDlg::OnKillFocusComment)
//	ON_BN_SETFOCUS(IDC_COMMENT_EX, &CDiskMarkDlg::OnSetFocusCommentEx)

END_MESSAGE_MAP()

LRESULT CDiskMarkDlg::OnQueryEndSession(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

int CALLBACK EnumFontFamExProcSegoeUI(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam)
{
	if (_tcscmp(lpelfe->elfLogFont.lfFaceName, _T("Segoe UI")) == 0)
	{
		BOOL *flag = (BOOL*)lParam;
		*flag = TRUE;
	}
	return TRUE;
}

BOOL CDiskMarkDlg::OnInitDialog()
{
	CMainDialog::OnInitDialog();

	m_hAccelerator = ::LoadAccelerators(AfxGetInstanceHandle(),
		                                MAKEINTRESOURCE(IDR_ACCELERATOR));

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIconMini, FALSE);

#ifdef SUISHO_SHIZUKU_SUPPORT
#ifdef KUREI_KEI_SUPPORT
	m_BackgroundName = L"KureiKeiBackground";
#else
	m_BackgroundName = L"ShizukuBackground";
#endif
#else
	m_BackgroundName = L"Background";
#endif

	TCHAR str[256];

	CClientDC dc(this);
	LOGFONT logfont;
	CString defaultFontFace;
	BOOL hasSegoeUI = FALSE;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = ANSI_CHARSET;
	::EnumFontFamiliesExW(dc.m_hDC, &logfont, (FONTENUMPROC)EnumFontFamExProcSegoeUI, (INT_PTR)(&hasSegoeUI), 0);

	if (hasSegoeUI)
	{
		defaultFontFace = _T("Segoe UI");
	}
	else
	{
		defaultFontFace = _T("Tahoma");
	}

	GetPrivateProfileString(_T("Setting"), _T("FontFace"), defaultFontFace, str, 256, m_Ini);
	m_FontFace = str;
	m_FontType = GetPrivateProfileInt(_T("Setting"), _T("FontType"), 0, m_Ini);
	if (m_FontType < FT_AUTO || FT_GDI_PLUS_3 < m_FontType)
	{
		m_FontType = FT_GDI;
	}

	m_TestData = GetPrivateProfileInt(_T("Setting"), _T("TestData"), TEST_DATA_RANDOM, m_Ini);
	if (m_TestData != TEST_DATA_ALL0X00)
	{
		m_TestData = TEST_DATA_RANDOM;
	}

	m_Profile = GetPrivateProfileInt(_T("Setting"), _T("Profile"), PROFILE_DEFAULT, m_Ini);
	if (m_Profile > PROFILE_REAL_MIX)
	{
		m_Profile = PROFILE_DEFAULT;
	}

	if (m_Profile == PROFILE_DEFAULT_MIX || m_Profile == PROFILE_PEAK_MIX || m_Profile == PROFILE_REAL_MIX)
	{
		m_MixMode = TRUE;
	}
	else
	{
		m_MixMode = FALSE;
	}

	m_FontScale = GetPrivateProfileInt(_T("Setting"), _T("FontScale"), 100, m_Ini);
	if (m_FontScale > 200 || m_FontScale < 50)
	{
		m_FontScale = 100;
	}

	m_Affinity = GetPrivateProfileInt(_T("Setting"), _T("Affinity"), AFFINITY_DISABLED, m_Ini);
	if (m_Affinity < 0 || m_Affinity > 1)
	{
		m_Affinity = AFFINITY_DISABLED;
	}

	// Unit
	m_ComboUnit.AddString(L"MB/s");
	m_ComboUnit.AddString(L"GB/s");
	m_ComboUnit.AddString(L"IOPS");
	m_ComboUnit.AddString(L"μs");

	// Count
	for (int i = 1; i < 10; i++)
	{
		CString cstr;
		cstr.Format(L"%d", i);
		m_ComboCount.AddString(cstr);
	}

#ifdef MIX_MODE
	// Mix
	for (int i = 1; i < 10; i++)
	{
		CString cstr;
		cstr.Format(L"R%d%%/W%d%%", i * 10, 100 - i*10);
		m_ComboMix.AddString(cstr);
	}
#endif
	

	UpdateQueuesThreads();

	m_WinThread = NULL;
	m_DiskBenchStatus = FALSE;
	m_FragmenteCounts = -1;

	InitThemeLang();
	InitMenu();
	ChangeTheme(m_CurrentTheme);
	ChangeLang(m_CurrentLang);

	m_IndexTestUnit = GetPrivateProfileInt(_T("Setting"), _T("TestUnit"), 0, m_Ini);
	if (m_IndexTestUnit < 0 || m_IndexTestUnit >= 3)
	{
		m_IndexTestUnit = 0;
	}
	m_ComboUnit.SetCurSel(m_IndexTestUnit);

	m_IndexTestCount = GetPrivateProfileInt(_T("Setting"), _T("TestCount"), 4, m_Ini);
	if (m_IndexTestCount < 0 || m_IndexTestCount >= 9)
	{
		m_IndexTestCount = 4;	// default value is 5.
	}
	m_ComboCount.SetCurSel(m_IndexTestCount);
	
	// Size
	TCHAR size[13][8] = { L"16MiB", L"32MiB", L"64MiB", L"128MiB", L"256MiB", L"512MiB", L"1GiB", L"2GiB", L"4GiB",  L"8GiB", L"16GiB", L"32GiB", L"64GiB" };

	for (int i = 0; i < 13; i++)
	{
		CString cstr;
		cstr.Format(L"%s", size[i]);
		m_ComboSize.AddString(cstr);
	}
	m_IndexTestSize = GetPrivateProfileInt(_T("Setting"), _T("TestSize"), 6, m_Ini);
	if (m_IndexTestSize < 0 || m_IndexTestSize > 13)
	{
		m_IndexTestSize = 6;	// default value is 1GiB;
	}
	m_ComboSize.SetCurSel(m_IndexTestSize);

	m_IndexTestMix = GetPrivateProfileInt(_T("Setting"), _T("TestMix"), 6, m_Ini);
	if (m_IndexTestMix < 0 || m_IndexTestMix > 10)
	{
		m_IndexTestMix = 6;	// default retio is R70W30;
	}
	m_MixRatio = (9 - m_IndexTestMix) * 10;

#ifdef MIX_MODE
	m_ComboMix.SetCurSel(m_IndexTestMix);
#endif

	UpdateData(FALSE);

	// Drive
	InitDrive();

	InitScore();
	UpdateUnitLabel();
	
	switch(GetPrivateProfileInt(_T("Setting"), _T("ZoomType"), 0, m_Ini))
	{
	case 100:  CheckRadioZoomType(ID_ZOOM_100, 100); break;
	case 125:  CheckRadioZoomType(ID_ZOOM_125, 125); break;
	case 150:  CheckRadioZoomType(ID_ZOOM_150, 150); break;
	case 200:  CheckRadioZoomType(ID_ZOOM_200, 200); break;
	case 250:  CheckRadioZoomType(ID_ZOOM_250, 250); break;
	case 300:  CheckRadioZoomType(ID_ZOOM_300, 300); break;
//	case 400:  CheckRadioZoomType(ID_ZOOM_400, 400); break;
	default:   CheckRadioZoomType(ID_ZOOM_AUTO, 0); break;
	}

	ChangeZoomType(m_ZoomType);
	
	m_SizeX = SIZE_X;
	m_SizeY = SIZE_Y;

#ifdef MIX_MODE
	if (m_MixMode)
	{
		m_SizeX = SIZE_X_MIX;
	}
#endif

	SetWindowTitle(L"");

	SetClientRect((DWORD)(m_SizeX * m_ZoomRatio), (DWORD)(m_SizeY * m_ZoomRatio));

	m_FlagShowWindow = TRUE;

//	ChangeButtonStatus(TRUE);
	CenterWindow();
	ChangeButtonStatus(TRUE);

	UpdateDialogSize();

	m_FlagInitializing = FALSE;

	SetForegroundWindow();

	return TRUE;
}

void CDiskMarkDlg::UpdateDialogSize()
{
	int offsetX;
	ShowWindow(SW_HIDE);
	UpdateBackground(true);

	m_SizeX = SIZE_X;
	m_SizeY = SIZE_Y;

#ifdef MIX_MODE
	if (m_MixMode)
	{
		m_SizeX = SIZE_X_MIX;
	}
#endif

	SetControlFont();

	if (m_Profile != PROFILE_DEFAULT && m_Profile != PROFILE_DEFAULT_MIX)
	{
#ifdef SUISHO_SHIZUKU_SUPPORT
		offsetX = 120;
#else
		offsetX = 72;
#endif
	}
	else
	{
		offsetX = 0;
	}

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_ButtonAll.InitControl(12 + OFFSET_X, 8, 120, 80, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonTest1.InitControl(12 + OFFSET_X, 96, 120, 80, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonTest2.InitControl(12 + OFFSET_X, 184, 120, 80, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonTest3.InitControl(12 + OFFSET_X, 272, 120, 80, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonTest4.InitControl(12 + OFFSET_X, 360, 120, 80, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);

	m_ButtonAll.SetHandCursor(TRUE);
	m_ButtonTest1.SetHandCursor(TRUE);
	m_ButtonTest2.SetHandCursor(TRUE);
	m_ButtonTest3.SetHandCursor(TRUE);
	m_ButtonTest4.SetHandCursor(TRUE);

	m_TestRead1.InitControl(140 + OFFSET_X, 96, 320, 80, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestRead2.InitControl(140 + OFFSET_X, 184, 320, 80, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestRead3.InitControl(140 + OFFSET_X, 272, 320, 80, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestRead4.InitControl(140 + OFFSET_X, 360, 320, 80, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_TestWrite1.InitControl(468 + OFFSET_X, 96, 320, 80, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestWrite2.InitControl(468 + OFFSET_X, 184, 320, 80, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestWrite3.InitControl(468 + OFFSET_X, 272, 320, 80, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestWrite4.InitControl(468 + OFFSET_X, 360, 320, 80, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_Comment.MoveWindow((int)((12 + OFFSET_X) * m_ZoomRatio), (int)(452 * m_ZoomRatio), (int)(776 * m_ZoomRatio), (int)(40 * m_ZoomRatio));
	m_Comment.ShowWindow(SW_HIDE);
	m_CommentEx.InitControl(12 + OFFSET_X, 452, 776, 40, m_ZoomRatio, IP(L"Comment"), 1, BS_LEFT, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_CommentEx.SetIbeamCursor(TRUE);
//	m_CommentEx.SetMargin(0, 4, 0, 4, m_ZoomRatio);

	m_ReadUnit.InitControl(140 + OFFSET_X, 52, 320, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_WriteUnit.InitControl(468 + OFFSET_X, 52, 320, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_ComboCount.InitControl(140 + OFFSET_X, 8, 60, 500, m_ZoomRatio, m_IsHighContrast);
	m_ComboSize.InitControl(204 + OFFSET_X, 8, 140, 500, m_ZoomRatio, m_IsHighContrast);
	m_ComboDrive.InitControl(348 + OFFSET_X, 8, 320 + offsetX, 500, m_ZoomRatio, m_IsHighContrast);
	m_ComboUnit.InitControl(672 + OFFSET_X, 8, 116, 500, m_ZoomRatio, m_IsHighContrast);

#else

	m_ButtonAll.InitControl(8 + OFFSET_X, 8, 72, 48, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonTest1.InitControl(8 + OFFSET_X, 60, 72, 48, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonTest2.InitControl(8 + OFFSET_X, 112, 72, 48, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonTest3.InitControl(8 + OFFSET_X, 164, 72, 48, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonTest4.InitControl(8 + OFFSET_X, 216, 72, 48, m_ZoomRatio, IP(L"Button"), 3, BS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);

	m_ButtonAll.SetHandCursor(TRUE);
	m_ButtonTest1.SetHandCursor(TRUE);
	m_ButtonTest2.SetHandCursor(TRUE);
	m_ButtonTest3.SetHandCursor(TRUE);
	m_ButtonTest4.SetHandCursor(TRUE);

	m_TestRead1.InitControl(84 + OFFSET_X, 60, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestRead2.InitControl(84 + OFFSET_X, 112, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestRead3.InitControl(84 + OFFSET_X, 164, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestRead4.InitControl(84 + OFFSET_X, 216, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_TestWrite1.InitControl(280 + OFFSET_X, 60, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestWrite2.InitControl(280 + OFFSET_X, 112, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestWrite3.InitControl(280 + OFFSET_X, 164, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestWrite4.InitControl(280 + OFFSET_X, 216, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_Comment.MoveWindow((int)((8 + OFFSET_X) * m_ZoomRatio), (int)(268 * m_ZoomRatio), (int)(464 * m_ZoomRatio), (int)(24 * m_ZoomRatio));
	m_Comment.ShowWindow(SW_HIDE);
	m_CommentEx.InitControl(8 + OFFSET_X, 268, 464, 24, m_ZoomRatio, IP(L"Comment"), 1, BS_LEFT, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_CommentEx.SetIbeamCursor(TRUE);
//	m_CommentEx.SetMargin(0, 8, 0, 8, m_ZoomRatio);

	m_ReadUnit.InitControl(84 + OFFSET_X, 36, 192, 24, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_WriteUnit.InitControl(280 + OFFSET_X, 36, 192, 24, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_ComboCount.InitControl(84 + OFFSET_X, 8, 40, 300, m_ZoomRatio, CComboBoxCx::OwnerDrawTransparent | m_IsHighContrast);
	m_ComboSize.InitControl(128 + OFFSET_X, 8, 80, 300, m_ZoomRatio, CComboBoxCx::OwnerDrawTransparent | m_IsHighContrast);
	m_ComboDrive.InitControl(212 + OFFSET_X, 8, 188 + offsetX, 300, m_ZoomRatio, CComboBoxCx::OwnerDrawTransparent | m_IsHighContrast);
	m_ComboUnit.InitControl(404 + OFFSET_X, 8, 68, 300, m_ZoomRatio, CComboBoxCx::OwnerDrawTransparent | m_IsHighContrast);
#endif


	m_TestRead1.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestRead2.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestRead3.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestRead4.SetMargin(0, 0, 0, 4, m_ZoomRatio);

	m_TestWrite1.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestWrite2.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestWrite3.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestWrite4.SetMargin(0, 0, 0, 4, m_ZoomRatio);

	// Set CButtonCx's Margin equals CEditEx's margin
	DWORD margin;
	margin = m_Comment.GetMargins();
	m_CommentEx.SetMargin(0, LOWORD(margin) + 3, 0, HIWORD(margin) + 3, 1.0);

#ifdef MIX_MODE
	m_TestMix1.InitControl(480 + OFFSET_X, 60, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestMix2.InitControl(480 + OFFSET_X, 112, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestMix3.InitControl(480 + OFFSET_X, 164, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_TestMix4.InitControl(480 + OFFSET_X, 216, 192, 48, m_ZoomRatio, IP(L"Meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_ComboMix.InitControl(480 + OFFSET_X, 8, 192, 300, m_ZoomRatio, CComboBoxCx::OwnerDrawTransparent | m_IsHighContrast);
	m_MixUnit.InitControl(480 + OFFSET_X, 36, 192, 24, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	if (m_MixMode)
	{
//		m_Comment.MoveWindow((int)((8 + OFFSET_X)* m_ZoomRatio), (int)(268* m_ZoomRatio), (int)(664 * m_ZoomRatio), (int)(24 * m_ZoomRatio));
		m_CommentEx.InitControl(8 + OFFSET_X, 268, 664, 24, m_ZoomRatio, IP(L"CommentL"), 1, BS_LEFT, CButtonCx::OwnerDrawImage | m_IsHighContrast);

		m_TestMix1.ShowWindow(SW_SHOW);
		m_TestMix2.ShowWindow(SW_SHOW);
		m_TestMix3.ShowWindow(SW_SHOW);
		m_TestMix4.ShowWindow(SW_SHOW);
		m_ComboMix.ShowWindow(SW_SHOW);
		m_MixUnit.ShowWindow(SW_SHOW);
	}
	else
	{
		m_TestMix1.ShowWindow(SW_HIDE);
		m_TestMix2.ShowWindow(SW_HIDE);
		m_TestMix3.ShowWindow(SW_HIDE);
		m_TestMix4.ShowWindow(SW_HIDE);
		m_ComboMix.ShowWindow(SW_HIDE);
		m_MixUnit.ShowWindow(SW_HIDE);
	}
#endif

	if (m_EditBrush != NULL)
	{
		m_EditBrush->DeleteObject();
		if (m_IsHighContrast)
		{
			m_EditBrush->CreateSolidBrush(RGB(0, 0, 0));
		}
		else
		{
			m_EditBrush->CreateSolidBrush(m_EditBg);
		}
	}

	if (m_IsHighContrast)
	{
		m_TestRead1.SetDrawFrame(TRUE);
		m_TestRead2.SetDrawFrame(TRUE);
		m_TestRead3.SetDrawFrame(TRUE);
		m_TestRead4.SetDrawFrame(TRUE);
		m_TestWrite1.SetDrawFrame(TRUE);
		m_TestWrite2.SetDrawFrame(TRUE);
		m_TestWrite3.SetDrawFrame(TRUE);
		m_TestWrite4.SetDrawFrame(TRUE);

	#ifdef MIX_MODE
		m_TestMix1.SetDrawFrame(TRUE);
		m_TestMix2.SetDrawFrame(TRUE);
		m_TestMix3.SetDrawFrame(TRUE);
		m_TestMix4.SetDrawFrame(TRUE);
	#endif
	}
	else
	{
		m_TestRead1.SetDrawFrame(FALSE);
		m_TestRead2.SetDrawFrame(FALSE);
		m_TestRead3.SetDrawFrame(FALSE);
		m_TestRead4.SetDrawFrame(FALSE);
		m_TestWrite1.SetDrawFrame(FALSE);
		m_TestWrite2.SetDrawFrame(FALSE);
		m_TestWrite3.SetDrawFrame(FALSE);
		m_TestWrite4.SetDrawFrame(FALSE);

	#ifdef MIX_MODE
		m_TestMix1.SetDrawFrame(FALSE);
		m_TestMix2.SetDrawFrame(FALSE);
		m_TestMix3.SetDrawFrame(FALSE);
		m_TestMix4.SetDrawFrame(FALSE);
	#endif
	}


	UpdateScore();
	Invalidate();
	ShowWindow(SW_SHOW);

	m_ComboUnit.ShowWindow(SW_HIDE);
	m_ComboCount.ShowWindow(SW_HIDE);
	m_ComboSize.ShowWindow(SW_HIDE);
	m_ComboDrive.ShowWindow(SW_HIDE);

#ifdef SUISHO_SHIZUKU_SUPPORT
//	SetLayeredWindow(m_CommentEx.m_hWnd, m_EditAlpha);
//	SetLayeredWindow(m_Comment.m_hWnd, m_EditAlpha);
	SetLayeredWindow(m_ComboUnit.m_hWnd, m_ComboAlpha);
	SetLayeredWindow(m_ComboCount.m_hWnd, m_ComboAlpha);
	SetLayeredWindow(m_ComboSize.m_hWnd, m_ComboAlpha);
	SetLayeredWindow(m_ComboDrive.m_hWnd, m_ComboAlpha);
#else
//	SetLayeredWindow(m_CommentEx.m_hWnd, m_EditAlpha);
//	SetLayeredWindow(m_Comment.m_hWnd, m_EditAlpha);
	SetLayeredWindow(m_ComboUnit.m_hWnd, m_ComboAlpha);
	SetLayeredWindow(m_ComboCount.m_hWnd, m_ComboAlpha);
	SetLayeredWindow(m_ComboSize.m_hWnd, m_ComboAlpha);
	SetLayeredWindow(m_ComboDrive.m_hWnd, m_ComboAlpha);
#ifdef MIX_MODE
	if(m_MixMode)
	{
		SetLayeredWindow(m_ComboMix.m_hWnd, m_ComboAlpha);
	}
#endif
#endif

	COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
	m_ComboUnit.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);
	m_ComboCount.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);
	m_ComboSize.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);
	m_ComboDrive.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);
#ifdef MIX_MODE
	if (m_MixMode)
	{
		m_ComboMix.GetComboBoxInfo(&info);
		SetLayeredWindow(info.hwndList, m_ComboAlpha);
		m_ComboMix.ShowWindow(SW_SHOW);
	}
#endif

	m_ComboCount.ShowWindow(SW_SHOW);
	m_ComboSize.ShowWindow(SW_SHOW);
	m_ComboDrive.ShowWindow(SW_SHOW);

	if (m_Profile != PROFILE_DEFAULT && m_Profile != PROFILE_DEFAULT_MIX)
	{
		m_ComboUnit.ShowWindow(SW_HIDE);
	}
	else
	{
		m_ComboUnit.ShowWindow(SW_SHOW);
	}

	SetClientRect((DWORD)(m_SizeX * m_ZoomRatio), (DWORD)(m_SizeY * m_ZoomRatio), 1);

	TCHAR str[256];
	GetPrivateProfileString(L"Title", L"TEST_UNIT", L"Test Unit", str, 256, m_CurrentLangPath);
	m_ComboUnit.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_COUNT", L"Test Count", str, 256, m_CurrentLangPath);
	m_ComboCount.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_SIZE", L"Test Size", str, 256, m_CurrentLangPath);
	m_ComboSize.SetToolTipText(str);
#ifdef MIX_MODE
	if (m_MixMode)
	{
		GetPrivateProfileString(L"Title", L"TEST_MIX", L"Mix Ratio", str, 256, m_CurrentLangPath);
		m_ComboMix.SetToolTipText(str);
	}
#endif
	m_ComboDrive.SetToolTipText(m_ComboDrive.GetToolTipText().GetString());

	ShowWindow(SW_SHOW);
}

void CDiskMarkDlg::SetLayeredWindow(HWND hWnd, BYTE alpha)
{
	::SetWindowLong(hWnd, GWL_EXSTYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	::SetWindowLong(hWnd, GWL_EXSTYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	if (m_IsHighContrast)
	{
		::SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
	}
	else
	{
		::SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	}
}


void CDiskMarkDlg::SetControlFont()
{
	double scale = m_FontScale / 100.0;

#ifdef SUISHO_SHIZUKU_SUPPORT
	BYTE textAlpha = 255;
	COLORREF textColor = RGB(0, 0, 0);
#else
	BYTE textAlpha = 255;
	COLORREF textColor = RGB(0, 0, 0);
#endif

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_ButtonAll.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonTest1.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonTest2.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonTest3.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonTest4.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);

	m_TestRead1.SetFontEx(m_FontFace, (int)(56 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestRead2.SetFontEx(m_FontFace, (int)(56 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestRead3.SetFontEx(m_FontFace, (int)(56 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestRead4.SetFontEx(m_FontFace, (int)(56 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);

	m_TestWrite1.SetFontEx(m_FontFace, (int)(56 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestWrite2.SetFontEx(m_FontFace, (int)(56 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestWrite3.SetFontEx(m_FontFace, (int)(56 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestWrite4.SetFontEx(m_FontFace, (int)(56 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);

	m_Comment.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, FW_BOLD);
	m_CommentEx.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, textAlpha, m_EditText, FW_BOLD, m_FontType);
	m_ButtonAll.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, textAlpha, m_EditText, FW_BOLD, m_FontType);

	m_ReadUnit.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);
	m_WriteUnit.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);

	m_ComboUnit.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);
	m_ComboCount.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);
	m_ComboSize.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);
	m_ComboDrive.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);

	m_ButtonTest3.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonTest1.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonTest2.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonTest4.SetMargin(8, 0, 8, 0, m_ZoomRatio);

	m_ComboUnit.SetFontHeight((int)(24 * scale), m_ZoomRatio);
	m_ComboUnit.SetItemHeight(-1, (UINT)(36 * m_ZoomRatio * scale));
	m_ComboCount.SetFontHeight((int)(24 * scale), m_ZoomRatio);
	m_ComboCount.SetItemHeight(-1, (UINT)(36 * m_ZoomRatio * scale));
	m_ComboSize.SetFontHeight((int)(24 * scale), m_ZoomRatio);
	m_ComboSize.SetItemHeight(-1, (UINT)(36 * m_ZoomRatio * scale));
	m_ComboDrive.SetFontHeight((int)(24 * scale), m_ZoomRatio);
	m_ComboDrive.SetItemHeight(-1, (UINT)(36 * m_ZoomRatio * scale));

	for (int i = 0; i < m_ComboUnit.GetCount(); i++)
	{
		m_ComboUnit.SetItemHeight(i, (UINT)(32 * m_ZoomRatio * scale));
	}
	for (int i = 0; i < m_ComboCount.GetCount(); i++)
	{
		m_ComboCount.SetItemHeight(i, (UINT)(32 * m_ZoomRatio * scale));
	}
	for (int i = 0; i < m_ComboSize.GetCount(); i++)
	{
		m_ComboSize.SetItemHeight(i, (UINT)(32 * m_ZoomRatio * scale));
	}
	for (int i = 0; i < m_ComboDrive.GetCount(); i++)
	{
		m_ComboDrive.SetItemHeight(i, (UINT)(32 * m_ZoomRatio * scale));
	}

#else

	m_ButtonAll.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonTest1.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonTest2.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonTest3.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonTest4.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);

	m_TestRead1.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestRead2.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestRead3.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestRead4.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);

	m_TestWrite1.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestWrite2.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestWrite3.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_TestWrite4.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);

	m_Comment.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, FW_BOLD);
	m_CommentEx.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_EditText, FW_BOLD, m_FontType);

	m_ReadUnit.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);
	m_WriteUnit.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);

	m_ComboUnit.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);
	m_ComboCount.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);
	m_ComboSize.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);
	m_ComboDrive.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);

	m_ButtonTest3.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonTest1.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonTest2.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonTest4.SetMargin(4, 0, 4, 0, m_ZoomRatio);

	m_ComboUnit.SetFontHeight((int)(16 * scale), m_ZoomRatio);
	m_ComboUnit.SetItemHeight(-1, (UINT)(20 * m_ZoomRatio * scale));
	m_ComboCount.SetFontHeight((int)(16 * scale), m_ZoomRatio);
	m_ComboCount.SetItemHeight(-1, (UINT)(20 * m_ZoomRatio * scale));
	m_ComboSize.SetFontHeight((int)(16 * scale), m_ZoomRatio);
	m_ComboSize.SetItemHeight(-1, (UINT)(20 * m_ZoomRatio * scale));
	m_ComboDrive.SetFontHeight((int)(16 * scale), m_ZoomRatio);
	m_ComboDrive.SetItemHeight(-1, (UINT)(20 * m_ZoomRatio * scale));

	for (int i = 0; i < m_ComboUnit.GetCount(); i++)
	{
		m_ComboUnit.SetItemHeight(i, (UINT)(20 * m_ZoomRatio * scale));
	}
	for (int i = 0; i < m_ComboCount.GetCount(); i++)
	{
		m_ComboCount.SetItemHeight(i, (UINT)(20 * m_ZoomRatio * scale));
	}
	for (int i = 0; i < m_ComboSize.GetCount(); i++)
	{
		m_ComboSize.SetItemHeight(i, (UINT)(20 * m_ZoomRatio * scale));
	}
	for (int i = 0; i < m_ComboDrive.GetCount(); i++)
	{
		m_ComboDrive.SetItemHeight(i, (UINT)(20 * m_ZoomRatio * scale));
	}

#ifdef MIX_MODE
	if(m_MixMode)
	{
		m_ComboMix.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontType);
		m_ComboMix.SetFontHeight((int)(16 * scale), m_ZoomRatio);
		m_ComboMix.SetItemHeight(-1, (UINT)(20 * m_ZoomRatio * scale));
		for (int i = 0; i < m_ComboMix.GetCount(); i++)
		{
			m_ComboMix.SetItemHeight(i, (UINT)(20 * m_ZoomRatio * scale));
		}
		m_ComboMix.SetBgColor(m_ComboBg, m_ComboBgSelected);

		m_MixUnit.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);

		m_TestMix1.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
		m_TestMix2.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
		m_TestMix3.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
		m_TestMix4.SetFontEx(m_FontFace, (int)(35 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	}
#endif

#endif

	m_ComboUnit.SetBgColor(m_ComboBg, m_ComboBgSelected);
	m_ComboCount.SetBgColor(m_ComboBg, m_ComboBgSelected);
	m_ComboSize.SetBgColor(m_ComboBg, m_ComboBgSelected);
	m_ComboDrive.SetBgColor(m_ComboBg, m_ComboBgSelected);
}

CString CDiskMarkDlg::IP(CString imageName)
{
	CString imagePath;
	imagePath.Format(L"%s%s\\%s-%03d.png", m_ThemeDir.GetString(), m_CurrentTheme.GetString(), imageName.GetString(), (DWORD)(m_ZoomRatio * 100));
	if (IsFileExist(imagePath))
	{
		return imagePath;
	}
	imagePath.Format(L"%s%s\\%s-%03d.png", m_ThemeDir.GetString(), m_DefaultTheme.GetString(), imageName.GetString(), (DWORD)(m_ZoomRatio * 100));
	if (IsFileExist(imagePath))
	{
		return imagePath;
	}
	return L"";
}

void CDiskMarkDlg::UpdateQueuesThreads()
{
	CString cstr;

	m_SequentialSize1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialSize1"), 1, m_Ini);
	if (m_SequentialSize1 <= 0 || m_SequentialSize1 > 8)
	{
		m_SequentialSize1 = 1;
	}
	m_SequentialQueues1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialQueues1"), 8, m_Ini);
	if (m_SequentialQueues1 <= 0 || m_SequentialQueues1 > MAX_QUEUES)
	{
		m_SequentialQueues1 = 8;
	}
	m_SequentialThreads1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialThreads1"), 1, m_Ini);
	if (m_SequentialThreads1 <= 0 || m_SequentialThreads1 > MAX_THREADS)
	{
		m_SequentialThreads1 = 1;
	}

	m_SequentialSize2 = GetPrivateProfileInt(_T("Setting"), _T("SequentialSize2"), 1, m_Ini);
	if (m_SequentialSize2 <= 0 || m_SequentialSize2 > 8)
	{
		m_SequentialSize2 = 1;
	}
	m_SequentialQueues2 = GetPrivateProfileInt(_T("Setting"), _T("SequentialQueues2"), 1, m_Ini);
	if (m_SequentialQueues2 <= 0 || m_SequentialQueues2 > MAX_QUEUES)
	{
		m_SequentialQueues2 = 1;
	}
	m_SequentialThreads2 = GetPrivateProfileInt(_T("Setting"), _T("SequentialThreads2"), 1, m_Ini);
	if (m_SequentialThreads2 <= 0 || m_SequentialThreads2 > MAX_THREADS)
	{
		m_SequentialThreads2 = 1;
	}
	
	m_RandomSize1 = GetPrivateProfileInt(_T("Setting"), _T("RandomSize1"), 4, m_Ini);
	if (m_RandomSize1 < 4 || m_RandomSize1 > 512)
	{
		m_RandomSize1 = 4;
	}
	m_RandomQueues1 = GetPrivateProfileInt(_T("Setting"), _T("RandomQueues1"), 32, m_Ini);
	if (m_RandomQueues1 <= 0 || m_RandomQueues1 > MAX_QUEUES)
	{
		m_RandomQueues1 = 32;
	}
	m_RandomThreads1 = GetPrivateProfileInt(_T("Setting"), _T("RandomThreads1"), 16, m_Ini);
	if (m_RandomThreads1 <= 0 || m_RandomThreads1 > MAX_THREADS)
	{
		m_RandomThreads1 = 16;
	}

	m_RandomSize2 = GetPrivateProfileInt(_T("Setting"), _T("RandomSize2"), 4, m_Ini);
	if (m_RandomSize2 < 4 || m_RandomSize2 > 512)
	{
		m_RandomSize2 = 4;
	}
	m_RandomQueues2 = GetPrivateProfileInt(_T("Setting"), _T("RandomQueues2"), 1, m_Ini);
	if (m_RandomQueues2 <= 0 || m_RandomQueues2 > MAX_QUEUES)
	{
		m_RandomQueues2 = 1;
	}
	m_RandomThreads2 = GetPrivateProfileInt(_T("Setting"), _T("RandomThreads2"), 1, m_Ini);
	if (m_RandomThreads2 <= 0 || m_RandomThreads2 > MAX_THREADS)
	{
		m_RandomThreads2 = 1;
	}

	m_Affinity = GetPrivateProfileInt(_T("Setting"), _T("Affinity"), 0, m_Ini);
	if (m_Affinity < 0 || m_Affinity > 1)
	{
		m_Affinity = 0;
	}
}

BOOL CDiskMarkDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( 0 != ::TranslateAccelerator(m_hWnd, m_hAccelerator, pMsg) )
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

#define STRICT_TYPED_ITEMIDS    // Better type safety for IDLists
#include <shlobj.h>             // Typical Shell header file

INT_PTR CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		if (lpData != NULL)
		{
			::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
		}
		break;
	}
	return 0;
}

void CDiskMarkDlg::SelectDrive()
{
	CString cstr;
	if (m_DiskBenchStatus)
	{
		return ;
	}

	int previousComboDriveIndex = m_IndexTestDrive;

	UpdateData(TRUE);
	
	if (m_ComboDrive.GetCurSel() == m_MaxIndexTestDrive)
	{
		BROWSEINFO  bi;
		ZeroMemory(&bi, sizeof(BROWSEINFO));
		ITEMIDLIST __unaligned *idl;
		LPMALLOC    g_pMalloc;
		TCHAR       szTmp[MAX_PATH];

		HRESULT hResult = SHGetMalloc(&g_pMalloc);
		if (hResult == NOERROR)
		{
			bi.hwndOwner = this->m_hWnd;
			bi.lpfn = (BFFCALLBACK)BrowseCallbackProc;
			bi.lParam = (LPARAM)m_TestTargetPath.GetString();
			bi.pszDisplayName = szTmp;
			bi.lpszTitle = L"";
			bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_NONEWFOLDERBUTTON;
			idl = SHBrowseForFolderW(&bi);
			if (idl != NULL)
			{
				if (SHGetPathFromIDList(idl, szTmp) != FALSE)
				{
					WritePrivateProfileString(_T("Setting"), _T("TargetPath"), szTmp, m_Ini);
					m_TestTargetPath = szTmp;
				}
				g_pMalloc->Free(idl);
				m_ComboDrive.SetToolTipText(m_TestTargetPath);
			}
			else
			{
				m_IndexTestDrive = previousComboDriveIndex;
				m_ComboDrive.SetCurSel(m_IndexTestDrive);
			}
			g_pMalloc->Release();
		}
	}
	else
	{
		TCHAR str[256];
		GetPrivateProfileString(L"Title", L"TEST_DRIVE", L"Test Drive", str, 256, m_CurrentLangPath);
		m_ComboDrive.SetToolTipText(str);
	}
}


LRESULT CDiskMarkDlg::OnUpdateScore(WPARAM wParam, LPARAM lParam)
{
	UpdateScore();
	return 0;
}

LRESULT CDiskMarkDlg::OnExitBenchmark(WPARAM wParam, LPARAM lParam)
{
	ChangeButtonStatus(TRUE);
	EnableMenus();

	return 0;
}

void CDiskMarkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CMainDialog::OnPaint();
	}
}

HCURSOR CDiskMarkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDiskMarkDlg::OnOK()
{

}

void CDiskMarkDlg::OnExit()
{
	OnCancel();
}

void CDiskMarkDlg::OnAbout()
{
	m_AboutDlg = new CAboutDlg(this);
	m_AboutDlg->Create(CAboutDlg::IDD, m_AboutDlg, ID_HELP_ABOUT, this);
}

void CDiskMarkDlg::OnCancel()
{
	if (m_WinThread != NULL)
	{
		AfxMessageBox(m_MesStopBenchmark);
		return;
		//	Stop();
	}

	UpdateData(TRUE);
	CString cstr;
	cstr.Format(_T("%d"), m_IndexTestUnit);
	WritePrivateProfileString(_T("Setting"), _T("TestUnit"), cstr, m_Ini);
	cstr.Format(_T("%d"), m_IndexTestCount);
	WritePrivateProfileString(_T("Setting"), _T("TestCount"), cstr, m_Ini);
	cstr.Format(_T("%d"), m_IndexTestSize);
	WritePrivateProfileString(_T("Setting"), _T("TestSize"), cstr, m_Ini);
#ifdef MIX_MODE
	if (m_MixMode)
	{
		cstr.Format(_T("%d"), m_IndexTestMix);
		WritePrivateProfileString(_T("Setting"), _T("TestMix"), cstr, m_Ini);
	}
#endif

	if(m_IndexTestDrive != m_MaxIndexTestDrive)
	{ 
		cstr.Format(_T("%d"), m_ValueTestDrive.GetAt(0) - 'A');
	}
	else
	{
		cstr.Format(_T("%d"), 99);
	}
	WritePrivateProfileString(_T("Setting"), _T("DriveLetter"), cstr, m_Ini);

	CMainDialog::OnCancel();
}

void CDiskMarkDlg::InitScore()
{
	m_SequentialReadScore1 = 0.0;
	m_SequentialWriteScore1 = 0.0;
	m_SequentialReadScore2 = 0.0;
	m_SequentialWriteScore2 = 0.0;
	m_RandomReadScore1 = 0.0;
	m_RandomWriteScore1 = 0.0;
	m_RandomReadScore2 = 0.0;
	m_RandomWriteScore2 = 0.0;

#ifdef MIX_MODE
	m_SequentialMixScore1 = 0.0;
	m_SequentialMixScore2 = 0.0;
	m_RandomMixScore1 = 0.0;
	m_RandomMixScore2 = 0.0;
#endif

	m_SequentialReadLatency1 = 0.0;
	m_SequentialWriteLatency1 = 0.0;
	m_SequentialReadLatency2 = 0.0;
	m_SequentialWriteLatency2 = 0.0;
	m_RandomReadLatency1 = 0.0;
	m_RandomWriteLatency1 = 0.0;
	m_RandomReadLatency2 = 0.0;
	m_RandomWriteLatency2 = 0.0;

#ifdef MIX_MODE
	m_SequentialMixLatency1 = 0.0;
	m_SequentialMixLatency2 = 0.0;
	m_RandomMixLatency1 = 0.0;
	m_RandomMixLatency2 = 0.0;
#endif

	UpdateScore();
}

void CDiskMarkDlg::UpdateScore()
{
	UpdateData(TRUE);
	if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		SetMeter(&m_TestRead1, m_SequentialReadScore1, m_SequentialReadLatency1, m_SequentialSize1 * 1024 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead2, m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead3, m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1 * 1024, SCORE_IOPS);
		SetMeter(&m_TestRead4, m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1 * 1024, SCORE_US);
		SetMeter(&m_TestWrite1, m_SequentialWriteScore1, m_SequentialWriteLatency1, m_SequentialSize1 * 1024 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite2, m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite3, m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1 * 1024, SCORE_IOPS);
		SetMeter(&m_TestWrite4, m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1 * 1024, SCORE_US);

#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix1, m_SequentialMixScore1, m_SequentialMixLatency1, m_SequentialSize1 * 1024 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix2, m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix3, m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1 * 1024, SCORE_IOPS);
			SetMeter(&m_TestMix4, m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1 * 1024, SCORE_US);
		}
#endif
	}
	else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		SetMeter(&m_TestRead1, m_SequentialReadScore1, m_SequentialReadLatency1, 1 * 1024 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead2, m_RandomReadScore1, m_RandomReadLatency1, 4 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead3, m_RandomReadScore1, m_RandomReadLatency1, 4 * 1024, SCORE_IOPS);
		SetMeter(&m_TestRead4, m_RandomReadScore1, m_RandomReadLatency1, 4 * 1024, SCORE_US);
		SetMeter(&m_TestWrite1, m_SequentialWriteScore1, m_SequentialWriteLatency1, 1 * 1024 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite2, m_RandomWriteScore1, m_RandomWriteLatency1, 4 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite3, m_RandomWriteScore1, m_RandomWriteLatency1, 4 * 1024, SCORE_IOPS);
		SetMeter(&m_TestWrite4, m_RandomWriteScore1, m_RandomWriteLatency1, 4 * 1024, SCORE_US);

#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix1, m_SequentialMixScore1, m_SequentialMixLatency1, 1 * 1024 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix2, m_RandomMixScore1, m_RandomMixLatency1, 4 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix3, m_RandomMixScore1, m_RandomMixLatency1, 4 * 1024, SCORE_IOPS);
			SetMeter(&m_TestMix4, m_RandomMixScore1, m_RandomMixLatency1, 4 * 1024, SCORE_US);
		}
#endif
	}
	else
	{
		SetMeter(&m_TestRead1, m_SequentialReadScore1, m_SequentialReadLatency1, m_SequentialSize1 * 1024 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead2, m_SequentialReadScore2, m_SequentialReadLatency2, m_SequentialSize2 * 1024 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead3, m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead4, m_RandomReadScore2, m_RandomReadLatency2, m_RandomSize2 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite1, m_SequentialWriteScore1, m_SequentialWriteLatency1, m_SequentialSize1 * 1024 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite2, m_SequentialWriteScore2, m_SequentialWriteLatency2, m_SequentialSize2 * 1024 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite3, m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite4, m_RandomWriteScore2, m_RandomWriteLatency2, m_RandomSize2 * 1024, m_IndexTestUnit);

#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix1, m_SequentialMixScore1, m_SequentialMixLatency1, m_SequentialSize1 * 1024 * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix2, m_SequentialMixScore2, m_SequentialMixLatency2, m_SequentialSize2 * 1024 * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix3, m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1 * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix4, m_RandomMixScore2, m_RandomMixLatency2, m_RandomSize2 * 1024, m_IndexTestUnit);
		}
#endif
	}

}

void CDiskMarkDlg::SetScoreToolTip(CStaticCx *control, double score, double latency, int blockSize)
{
	CString cstr;
	if (blockSize == -1)
	{
		cstr.Format(L"%.3f MB/s\r\n%.3f GB/s", score, score / 1000);
	}
	else if (score <= 0.0)
	{
		cstr.Format(L"%.3f MB/s\r\n%.3f GB/s\r\n%.3f IOPS\r\n%.3f μs", 0.0, 0.0, 0.0, 0.0);
	}
	else
	{
		cstr.Format(L"%.3f MB/s\r\n%.3f GB/s\r\n%.3f IOPS\r\n%.3f μs", score, score / 1000, score * 1000 * 1000 / blockSize, latency);
	}
	control->SetToolTipText(cstr);
}

void CDiskMarkDlg::OnSequentialPeak()
{
	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_SequentialReadScore1 = 0.0;
		m_SequentialWriteScore1 = 0.0;
		m_SequentialReadLatency1 = 0.0;
		m_SequentialWriteLatency1 = 0.0;
#ifdef MIX_MODE
		m_SequentialMixScore1 = 0.0;
		m_SequentialMixLatency1 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchSequential1, (void*)this);
		if (m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::OnRandomPeak()
{
	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomReadScore1 = 0.0;
		m_RandomWriteScore1 = 0.0;
		m_RandomReadLatency1 = 0.0;
		m_RandomWriteLatency1 = 0.0;
#ifdef MIX_MODE
		m_RandomMixScore1 = 0.0;
		m_RandomMixLatency1 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchRandom1, (void*)this);
		if (m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::OnSequentialReal()
{
	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_SequentialReadScore1 = 0.0;
		m_SequentialWriteScore1 = 0.0;
		m_SequentialReadLatency1 = 0.0;
		m_SequentialWriteLatency1 = 0.0;
#ifdef MIX_MODE
		m_SequentialMixScore1 = 0.0;
		m_SequentialMixLatency1 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchSequentialReal, (void*)this);
		if (m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::OnRandomReal()
{
	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomReadScore1 = 0.0;
		m_RandomWriteScore1 = 0.0;
		m_RandomReadLatency1 = 0.0;
		m_RandomWriteLatency1 = 0.0;
#ifdef MIX_MODE
		m_RandomMixScore1 = 0.0;
		m_RandomMixLatency1 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchRandomReal, (void*)this);
		if (m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::OnTest1()
{
	if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		OnSequentialPeak();
		return;
	}
	else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		OnSequentialReal();
		return;
	}

	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_SequentialReadScore1 = 0.0;
		m_SequentialWriteScore1 = 0.0;
		m_SequentialReadLatency1 = 0.0;
		m_SequentialWriteLatency1 = 0.0;
#ifdef MIX_MODE
		m_SequentialMixScore1 = 0.0;
		m_SequentialMixLatency1 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchSequential1, (void*)this);
		if(m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::OnTest2()
{
	if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		OnRandomPeak();
		return;
	}
	else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		OnRandomReal();
		return;
	}

	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_SequentialReadScore2 = 0.0;
		m_SequentialWriteScore2 = 0.0;
		m_SequentialReadLatency2 = 0.0;
		m_SequentialWriteLatency2 = 0.0;
#ifdef MIX_MODE
		m_SequentialMixScore2 = 0.0;
		m_SequentialMixLatency2 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchSequential2, (void*)this);
		if (m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::OnTest3()
{
	if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		OnRandomPeak();
		return;
	}
	else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		OnRandomReal();
		return;
	}

	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomReadScore1 = 0.0;
		m_RandomWriteScore1 = 0.0;
		m_RandomReadLatency1 = 0.0;
		m_RandomWriteLatency1 = 0.0;
#ifdef MIX_MODE
		m_RandomMixScore1 = 0.0;
		m_RandomMixLatency1 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchRandom1, (void*)this);
		if(m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::OnTest4()
{
	if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		OnRandomPeak();
		return;
	}
	else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		OnRandomReal();
		return;
	}

	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomReadScore2 = 0.0;
		m_RandomWriteScore2 = 0.0;
		m_RandomReadLatency2 = 0.0;
		m_RandomWriteLatency2 = 0.0;
#ifdef MIX_MODE
		m_RandomMixScore2 = 0.0;
		m_RandomMixLatency2 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchRandom2, (void*)this);
		if(m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::OnAll()
{
	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		InitScore();
		m_DiskBenchStatus = TRUE;
		if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
		{
			m_WinThread = AfxBeginThread(ExecDiskBenchAllPeak, (void*)this);
		}
		else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
		{
			m_WinThread = AfxBeginThread(ExecDiskBenchAllReal, (void*)this);
		}
		else
		{
			m_WinThread = AfxBeginThread(ExecDiskBenchAll, (void*)this);
		}

		if(m_WinThread == NULL)
		{
			m_DiskBenchStatus = FALSE;
		}
		else
		{
			ChangeButtonStatus(FALSE);
		}
		DisableMenus();
	}
	else
	{
		Stop();
	}
}

void CDiskMarkDlg::Stop()
{
	if(m_DiskBenchStatus)
	{
		m_DiskBenchStatus = FALSE;

		if (pi.hProcess != NULL)
		{
			TerminateProcess(pi.hProcess, 0);
		}
	}
	EnableMenus();
}

void CDiskMarkDlg::EnableMenus()
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
	menu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
	menu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
	menu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
	menu->EnableMenuItem(4, MF_BYPOSITION | MF_ENABLED);
	menu->EnableMenuItem(5, MF_BYPOSITION | MF_ENABLED);
	SetMenu(menu);
}

void CDiskMarkDlg::DisableMenus()
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	menu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
	menu->EnableMenuItem(2, MF_BYPOSITION | MF_GRAYED);
	menu->EnableMenuItem(3, MF_BYPOSITION | MF_GRAYED);
	menu->EnableMenuItem(4, MF_BYPOSITION | MF_GRAYED);
	menu->EnableMenuItem(5, MF_BYPOSITION | MF_GRAYED);
	SetMenu(menu);
}

void CDiskMarkDlg::ChangeButtonStatus(BOOL status)
{
	if(status)
	{
		CString title;
		CString toolTip;

#ifdef MIX_MODE
		m_ComboMix.EnableWindow(TRUE);
#endif
		m_ComboUnit.EnableWindow(TRUE);
		m_ComboCount.EnableWindow(TRUE);
		m_ComboSize.EnableWindow(TRUE);
		m_ComboDrive.EnableWindow(TRUE);

		m_ButtonAll.SetWindowTextW(L"All");


		if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
		{
			title.Format(L"SEQ%dM\r\nQ%dT%d", m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1);
			m_ButtonTest1.SetWindowTextW(title);
			title.Format(L"RND%dK\r\nQ%dT%d", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			m_ButtonTest2.SetWindowTextW(title);
			title.Format(L"RND%dK\r\n(IOPS)", m_RandomSize1);
			m_ButtonTest3.SetWindowTextW(title);
			title.Format(L"RND%dK\r\n(μs)", m_RandomSize1);
			m_ButtonTest4.SetWindowTextW(title);

			toolTip.Format(L"Sequential %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1);
			m_ButtonTest1.SetToolTipText(toolTip);
			toolTip.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			m_ButtonTest2.SetToolTipText(toolTip);
			toolTip.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(IOPS)", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			m_ButtonTest3.SetToolTipText(toolTip);
			toolTip.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(μs)", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			m_ButtonTest4.SetToolTipText(toolTip);
		}
		else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
		{
			title.Format(L"SEQ%dM\r\nQ%dT%d", 1, 1, 1);
			m_ButtonTest1.SetWindowTextW(title);
			title.Format(L"RND4K\r\nQ%dT%d", 1, 1);
			m_ButtonTest2.SetWindowTextW(title);
			title.Format(L"RND4K\r\n(IOPS)");
			m_ButtonTest3.SetWindowTextW(title);
			title.Format(L"RND4K\r\n(μs)");
			m_ButtonTest4.SetWindowTextW(title);

			toolTip.Format(L"Sequential 1MiB\r\nQueues=1\r\nThreads=1\r\n(MB/s)");
			m_ButtonTest1.SetToolTipText(toolTip);
			toolTip.Format(L"Random 4KiB\r\nQueues=1\r\nThreads=1\r\n(MB/s)");
			m_ButtonTest2.SetToolTipText(toolTip);
			toolTip.Format(L"Random 4KiB\r\nQueues=1\r\nThreads=1\r\n(IOPS)");
			m_ButtonTest3.SetToolTipText(toolTip);
			toolTip.Format(L"Random 4KiB\r\nQueues=1\r\nThreads=1\r\n(μs)");
			m_ButtonTest4.SetToolTipText(toolTip);
		}
		else
		{
			title.Format(L"SEQ%dM\r\nQ%dT%d", m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1);
			m_ButtonTest1.SetWindowTextW(title);
			title.Format(L"SEQ%dM\r\nQ%dT%d", m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2);
			m_ButtonTest2.SetWindowTextW(title);
			title.Format(L"RND%dK\r\nQ%dT%d", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			m_ButtonTest3.SetWindowTextW(title);
			title.Format(L"RND%dK\r\nQ%dT%d", m_RandomSize2, m_RandomQueues2, m_RandomThreads2);
			m_ButtonTest4.SetWindowTextW(title);

			toolTip.Format(L"Sequential %dMiB\r\nQueues=%d\r\nThreads=%d", m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1);
			m_ButtonTest1.SetToolTipText(toolTip);
			toolTip.Format(L"Sequential %dMiB\r\nQueues=%d\r\nThreads=%d", m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2);
			m_ButtonTest2.SetToolTipText(toolTip);
			toolTip.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			m_ButtonTest3.SetToolTipText(toolTip);
			toolTip.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d", m_RandomSize2, m_RandomQueues2, m_RandomThreads2);
			m_ButtonTest4.SetToolTipText(toolTip);
		}
	}
	else
	{
#ifdef MIX_MODE
		m_ComboMix.EnableWindow(FALSE);
#endif
		m_ComboUnit.EnableWindow(FALSE);
		m_ComboCount.EnableWindow(FALSE);
		m_ComboSize.EnableWindow(FALSE);
		m_ComboDrive.EnableWindow(FALSE);

		m_ButtonAll.SetWindowTextW(L"Stop");
		m_ButtonTest1.SetWindowTextW(L"Stop");
		m_ButtonTest2.SetWindowTextW(L"Stop");
		m_ButtonTest3.SetWindowTextW(L"Stop");
		m_ButtonTest4.SetWindowTextW(L"Stop");
	}
}

LRESULT CDiskMarkDlg::OnUpdateMessage(WPARAM wParam, LPARAM lParam)
{
	CString wstr = _T("");
	CString lstr = _T("");

	if(wParam != NULL)
	{
		wstr = *((CString*)wParam);
	}

	if(lParam != NULL)
	{
		lstr = *((CString*)lParam);
	}

	SetWindowTitle(wstr);
	return 0;
}

LRESULT CDiskMarkDlg::OnUpdateComment(WPARAM wParam, LPARAM lParam)
{
	CString wstr = _T("");
	DWORD margin = 0;

	if (wParam != NULL)
	{
		wstr = *((CString*)wParam);
		m_CommentEx.SetWindowTextW(wstr);
	}

	/* SetMargin
	if (lParam != NULL)
	{
		margin = *(DWORD*)(lParam);
		m_CommentEx.SetMargin(0, LOWORD(margin) + 3, 0, HIWORD(margin) + 3, 1.0);
	}
	*/

	return 0;
}

void CDiskMarkDlg::SetMeter(CStaticCx* control, double score, double latency, int blockSize, int unit)
{
	CString cstr;

	double meterRatio = 0.0;

	if (unit == SCORE_UNIT::SCORE_US)
	{
		if(latency > 0.0000000001)
		{
			meterRatio = 1 - 0.16666666666666 * log10(latency);
		}
		else
		{
			meterRatio = 0;
		}
	}
	else
	{
		if (score > 0.1)
		{
			meterRatio = 0.16666666666666 * log10(score * 10);
		}
		else
		{
			meterRatio = 0;
		}
	}


	if (meterRatio > 1.0)
	{
		meterRatio = 1.0;
	}


	if (unit == SCORE_UNIT::SCORE_IOPS)
	{
		double iops = score * 1000 * 1000 / blockSize;
		if (iops >= 1000000.0)
		{
			cstr.Format(_T("%d"), (int)iops);
		}
		else
		{
			cstr.Format(_T("%.2f"), iops);
		}
	}
	else if (unit == SCORE_UNIT::SCORE_US)
	{
		if (score <= 0.0)
		{
			cstr.Format(_T("%.2f"), 0.0);
		}
		else if (latency >= 1000000.0)
		{
			cstr.Format(_T("%d"), (int)latency);
		}
		else
		{
			cstr.Format(_T("%.2f"), latency);
		}
	}
	else if(unit == SCORE_UNIT::SCORE_GBS)
	{
		cstr.Format(_T("%.3f"), score / 1000.0);
	}
	else
	{
		if(score >= 1000000.0)
		{
			cstr.Format(_T("%d"), (int)score);
		}
		else
		{
			cstr.Format(_T("%.2f"), score);
		}
	}

	UpdateData(FALSE);
	control->SetMeter(TRUE, meterRatio);
	control->SetWindowTextW(cstr);

	SetScoreToolTip(control, score, latency, blockSize);
}

void CDiskMarkDlg::InitDrive()
{
	while (m_ComboDrive.GetCount())
	{
		m_ComboDrive.DeleteString(0);
	}

	CString cstr;
	CString select;

	// list up drive
	TCHAR szDrives[256] = {0};
	LPTSTR pDrive = szDrives;
	TCHAR rootPath[4] = {0};
	TCHAR fileSystem[32] = {0};
	int count = 0;
	GetLogicalDriveStrings(255, szDrives);

	m_IndexTestDrive = 0;

	while( pDrive[0] != _T('\0') )
	{
		ULARGE_INTEGER freeBytesAvailableToCaller = {0};
		ULARGE_INTEGER totalNumberOfBytes = {0};
		ULARGE_INTEGER totalNumberOfFreeBytes = {0};

	//	_tcsupr_s(pDrive, sizeof(TCHAR) * 4);
		int result = GetDriveType(pDrive);
	
		int forward = (int)_tcslen( pDrive );

		if(result == DRIVE_FIXED || result == DRIVE_REMOTE || result == DRIVE_REMOVABLE || result == DRIVE_RAMDISK)
		{
			pDrive[1] = _T('\0');
			cstr.Format(_T("%C: "), pDrive[0]);
			if(GetDiskFreeSpaceEx(cstr, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes) != 0)
			{
				select += cstr;
				if(totalNumberOfBytes.QuadPart < ((ULONGLONG)8 * 1024 * 1024 * 1024)) // < 8 GB
				{
					cstr.Format(_T("%s: %.0f%% (%.0f/%.0fMiB)\r\n"), pDrive,
							(double)(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100,
							(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / 1024 / 1024.0,
							totalNumberOfBytes.QuadPart  / 1024 / 1024.0);
				}
				else
				{
					cstr.Format(_T("%s: %.0f%% (%.0f/%.0fGiB)\r\n"), pDrive,
							(double)(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100,
							(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / 1024 / 1024 / 1024.0,
							totalNumberOfBytes.QuadPart  / 1024 / 1024 / 1024.0);
				}
				select += cstr;

				if(m_TestDriveLetter == pDrive[0] - 'A')
				{
					m_IndexTestDrive = count;
				}
				count++;

				m_ComboDrive.AddString(cstr);
			}
		}
		pDrive += forward + 1;
	}

	m_ComboDrive.AddString(i18n(L"Menu", L"SELECT_FOLDER"));
	if (m_TestDriveLetter == 99)
	{
		m_IndexTestDrive = count;
	}

	m_MaxIndexTestDrive = count;

	m_ComboDrive.SetCurSel(m_IndexTestDrive);

	TCHAR str[256];
	GetPrivateProfileString(L"Title", L"TEST_DRIVE", L"Test Drive", str, 256, m_CurrentLangPath);
	m_ComboDrive.SetToolTipText(str);

	UpdateData(FALSE);
}

void CDiskMarkDlg::ChangeLang(CString LangName)
{
	m_CurrentLangPath.Format(_T("%s\\%s.lang"), (LPTSTR)m_LangDir.GetString(), (LPTSTR)LangName.GetString());

	CString cstr;
	CMenu *menu = GetMenu();
	CMenu subMenu;

	cstr = i18n(_T("Menu"), _T("FILE"));
	menu->ModifyMenu(0, MF_BYPOSITION | MF_STRING, 0, cstr);
	cstr = i18n(_T("Menu"), _T("SETTINGS"));
	menu->ModifyMenu(1, MF_BYPOSITION | MF_STRING, 1, cstr);
	cstr = i18n(_T("Menu"), _T("PROFILE"));
	menu->ModifyMenu(2, MF_BYPOSITION | MF_STRING, 2, cstr);
	cstr = i18n(_T("Menu"), _T("THEME"));
	menu->ModifyMenu(3, MF_BYPOSITION | MF_STRING, 3, cstr);
	cstr = i18n(_T("Menu"), _T("HELP"));
	menu->ModifyMenu(4, MF_BYPOSITION | MF_STRING, 4, cstr);
	cstr = i18n(_T("Menu"), _T("LANGUAGE"));
	if(cstr.Find(_T("Language")) >= 0)
	{
		cstr = _T("&Language");
		menu->ModifyMenu(5, MF_BYPOSITION | MF_STRING, 5, cstr);
	}
	else
	{
		menu->ModifyMenu(5, MF_BYPOSITION | MF_STRING, 5, cstr + _T("(&Language)"));
	}

	cstr = i18n(_T("Menu"), _T("FILE_EXIT")) + _T("\tAlt + F4");
	menu->ModifyMenu(ID_FILE_EXIT, MF_STRING, ID_FILE_EXIT, cstr);
	cstr = i18n(_T("Menu"), _T("SAVE")) + _T("\tCtrl + S");
	menu->ModifyMenu(ID_RESULT_SAVE, MF_STRING, ID_RESULT_SAVE, cstr);

	cstr = i18n(_T("Menu"), _T("EDIT_COPY")) + _T("\tCtrl + C");
	menu->ModifyMenu(ID_EDIT_COPY, MF_STRING, ID_EDIT_COPY, cstr);
	cstr = i18n(_T("Menu"), _T("QUEUES_THREADS")) + _T("\tCtrl + Q");
	menu->ModifyMenu(ID_SETTINGS_QUEUESTHREADS, MF_STRING, ID_SETTINGS_QUEUESTHREADS, cstr);

	cstr = i18n(_T("Menu"), _T("PROFILE_DEFAULT"));
	menu->ModifyMenu(ID_PROFILE_DEFAULT, MF_STRING, ID_PROFILE_DEFAULT, cstr);
	cstr = i18n(_T("Menu"), _T("PROFILE_PEAK"));
	menu->ModifyMenu(ID_PROFILE_PEAK, MF_STRING, ID_PROFILE_PEAK, cstr);
	cstr = i18n(_T("Menu"), _T("PROFILE_REAL"));
	menu->ModifyMenu(ID_PROFILE_REAL, MF_STRING, ID_PROFILE_REAL, cstr);

#ifdef MIX_MODE
	cstr = i18n(_T("Menu"), _T("PROFILE_DEFAULT")) + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_DEFAULT_MIX, MF_STRING, ID_PROFILE_DEFAULT_MIX, cstr);
	cstr = i18n(_T("Menu"), _T("PROFILE_PEAK")) + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_PEAK_MIX, MF_STRING, ID_PROFILE_PEAK_MIX, cstr);
	cstr = i18n(_T("Menu"), _T("PROFILE_REAL")) + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_REAL_MIX, MF_STRING, ID_PROFILE_REAL_MIX, cstr);
#endif

	cstr = i18n(_T("Menu"), _T("HELP")) + _T("\tF1");
	menu->ModifyMenu(ID_HELP, MF_STRING, ID_HELP, cstr);
	cstr = i18n(_T("Menu"), _T("HELP_ABOUT"));
	menu->ModifyMenu(ID_HELP_ABOUT, MF_STRING, ID_HELP_ABOUT, cstr);

	subMenu.Attach(menu->GetSubMenu(1)->GetSafeHmenu());
	cstr = i18n(_T("Menu"), _T("TEST_DATA"));
	subMenu.ModifyMenu(0, MF_BYPOSITION, 0, cstr);
	cstr = i18n(_T("Menu"), _T("INTERVAL_TIME"));
	subMenu.ModifyMenu(1, MF_BYPOSITION, 1, cstr);
	subMenu.Detach();

	CString second, minute;

	second = i18n(_T("Menu"), _T("SECOND"));
	minute = i18n(_T("Menu"), _T("MINUTE"));
	menu->ModifyMenu(ID_INTERVAL_TIME_0, MF_STRING, ID_INTERVAL_TIME_0, _T("0 ") + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_1, MF_STRING, ID_INTERVAL_TIME_1, _T("1 ") + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_3, MF_STRING, ID_INTERVAL_TIME_3, _T("3 ") + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_5, MF_STRING, ID_INTERVAL_TIME_5, _T("5 ") + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_10, MF_STRING, ID_INTERVAL_TIME_10, _T("10 ") + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_30, MF_STRING, ID_INTERVAL_TIME_30, _T("30 ") + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_60, MF_STRING, ID_INTERVAL_TIME_60, _T("1 ") + minute);
	menu->ModifyMenu(ID_INTERVAL_TIME_180, MF_STRING, ID_INTERVAL_TIME_180, _T("3 ") + minute);
	menu->ModifyMenu(ID_INTERVAL_TIME_300, MF_STRING, ID_INTERVAL_TIME_300, _T("5 ") + minute);
	menu->ModifyMenu(ID_INTERVAL_TIME_600, MF_STRING, ID_INTERVAL_TIME_600, _T("10 ") + minute);

	cstr = i18n(_T("Menu"), _T("DEFAULT_RANDOM"));
	menu->ModifyMenu(ID_MODE_DEFAULT, MF_STRING, ID_MODE_DEFAULT, cstr);
	cstr = i18n(_T("Menu"), _T("ALL_ZERO"));
	menu->ModifyMenu(ID_MODE_ALL0X00, MF_STRING, ID_MODE_ALL0X00, cstr);

	// Theme
	subMenu.Attach(menu->GetSubMenu(3)->GetSafeHmenu());
	cstr = i18n(_T("Menu"), _T("ZOOM"));
	subMenu.ModifyMenu(0, MF_BYPOSITION, 0, cstr);
	subMenu.Detach();

	cstr = i18n(_T("Menu"), _T("AUTO"));
	menu->ModifyMenu(ID_ZOOM_AUTO, MF_STRING, ID_ZOOM_AUTO, cstr);

	cstr = i18n(_T("Menu"), _T("FONT_SETTING")) + _T("\tCtrl + F");
	menu->ModifyMenu(ID_FONT_SETTING, MF_STRING, ID_FONT_SETTING, cstr);

	CheckRadioZoomType();

	if(m_TestData == TEST_DATA_ALL0X00)
	{
		OnModeAll0x00();
	}
	else
	{
		OnModeDefault();
	}

	switch (m_Profile)
	{
	case PROFILE_DEFAULT:
		ProfileDefault();
		break;
	case PROFILE_PEAK:
		ProfilePeak();
		break;
	case PROFILE_REAL:
		ProfileReal();
		break;
#ifdef MIX_MODE
	case PROFILE_DEFAULT_MIX:
		ProfileDefaultMix();
		break;
	case PROFILE_PEAK_MIX:
		ProfilePeakMix();
		break;
	case PROFILE_REAL_MIX:
		ProfileRealMix();
		break;
#endif
	default:
		OnProfileDefault();
		break;
	}

	CheckIntervalTime();

	SetMenu(menu);

	m_MesStopBenchmark = i18n(_T("Message"), _T("STOP_BENCHMARK"));
	m_MesDiskCapacityError = i18n(_T("Message"), _T("DISK_CAPACITY_ERROR"));
	m_MesDiskCreateFileError = i18n(_T("Message"), _T("DISK_CREATE_FILE_ERROR"));
	m_MesDiskWriteError = i18n(_T("Message"), _T("DISK_WRITE_ERROR"));
	m_MesDiskReadError = i18n(_T("Message"), _T("DISK_READ_ERROR"));
	m_MesDiskSpdNotFound = i18n(_T("Message"), _T("DISK_SPD_NOT_FOUND"));

	InitDrive();

	WritePrivateProfileString(_T("Setting"), _T("Language"), LangName, m_Ini);
}

BOOL CDiskMarkDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Select Theme
	if (WM_THEME_ID <= wParam && wParam < WM_THEME_ID + (UINT) m_MenuArrayTheme.GetSize())
	{
		CMainDialog::OnCommand(wParam, lParam);
		return TRUE;
	}

	// Select Language
	if(WM_LANGUAGE_ID <= wParam && wParam < WM_LANGUAGE_ID + (UINT)m_MenuArrayLang.GetSize())
	{
#ifdef _UNICODE
		CMenu menu;
		CMenu subMenu;
		CMenu subMenuAN;
		CMenu subMenuOZ;
		menu.Attach(GetMenu()->GetSafeHmenu());
		subMenu.Attach(menu.GetSubMenu(5)->GetSafeHmenu()); // 6th is "Language".
		subMenuAN.Attach(subMenu.GetSubMenu(0)->GetSafeHmenu());
		subMenuOZ.Attach(subMenu.GetSubMenu(1)->GetSafeHmenu());

		m_CurrentLang = m_MenuArrayLang.GetAt(wParam - WM_LANGUAGE_ID);
		ChangeLang(m_MenuArrayLang.GetAt(wParam - WM_LANGUAGE_ID));
		subMenuAN.CheckMenuRadioItem(WM_LANGUAGE_ID, WM_LANGUAGE_ID + (UINT)m_MenuArrayLang.GetSize(),
									(UINT)wParam, MF_BYCOMMAND);
		subMenuOZ.CheckMenuRadioItem(WM_LANGUAGE_ID, WM_LANGUAGE_ID + (UINT)m_MenuArrayLang.GetSize(),
									(UINT)wParam, MF_BYCOMMAND);

		subMenuOZ.Detach();
		subMenuAN.Detach();
		subMenu.Detach();
		menu.Detach();
#else
		CMenu menu;
		CMenu subMenu;
		menu.Attach(GetMenu()->GetSafeHmenu());
		subMenu.Attach(menu.GetSubMenu(4)->GetSafeHmenu()); // 5th is "Language".

		m_CurrentLang = m_MenuArrayLang.GetAt(wParam - WM_LANGUAGE_ID);
		ChangeLang(m_MenuArrayLang.GetAt(wParam - WM_LANGUAGE_ID));
		subMenu.CheckMenuRadioItem(WM_LANGUAGE_ID, WM_LANGUAGE_ID + (UINT)m_MenuArrayLang.GetSize(),
									(UINT)wParam, MF_BYCOMMAND);
		subMenu.Detach();
		menu.Detach();
#endif
	}

	return CMainDialog::OnCommand(wParam, lParam);
}

void CDiskMarkDlg::OnEditCopy()
{
	ResultText(RESULT_TEXT_CLIPBOARD);
}

void CDiskMarkDlg::OnResultSave()
{
	ResultText(RESULT_TEXT_FILE);
}

CString CDiskMarkDlg::GetRandomResultString(double score, double latency, int size, int queues, int threads)
{
	CString result;
	double iops = 0.0;

	iops = score * 1000 * 1000 / ((double)size * 1024);

	result.Format(L"Random %dKiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size, queues, threads, score, iops, latency);

	if (size > 100)
	{
		result = L"  " + result;
	}
	else if(size > 10)
	{
		result = L"   " + result;
	}
	else
	{
		result = L"    " + result;
	}

	return result;
}

CString CDiskMarkDlg::GetSequentialResultString(double score, double latency, int size, int queues, int threads)
{
	CString result;
	double iops = 0.0;

	iops = score * 1000 * 1000 / ((double)size * 1024 * 1024);

	result.Format(L"Sequential %dMiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size, queues, threads, score, iops, latency);

	return result;
}

void CDiskMarkDlg::ResultText(RESULT_TEXT type)
{
	CString cstr, clip;

	UpdateData(TRUE);

	if (m_Profile == PROFILE_DEFAULT || m_Profile == PROFILE_DEFAULT_MIX)
	{
		clip = L"\
------------------------------------------------------------------------------\r\n\
%PRODUCT% %VERSION%%EDITION% (C) %COPY_YEAR% hiyohiyo\r\n\
                                  Crystal Dew World: https://crystalmark.info/\r\n\
------------------------------------------------------------------------------\r\n\
* MB/s = 1,000,000 bytes/s [SATA/600 = 600,000,000 bytes/s]\r\n\
* KB = 1000 bytes, KiB = 1024 bytes\r\n\
\r\n\
[Read]\r\n\
%SequentialRead1%\r\n\
%SequentialRead2%\r\n\
%RandomRead1%\r\n\
%RandomRead2%\r\n\
\r\n\
[Write]\r\n\
%SequentialWrite1%\r\n\
%SequentialWrite2%\r\n\
%RandomWrite1%\r\n\
%RandomWrite2%\r\n\
\r\n\
";

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip += L"\
[Mix] %MixRatio%\r\n\
%SequentialMix1%\r\n\
%SequentialMix2%\r\n\
%RandomMix1%\r\n\
%RandomMix2%\r\n\
\r\n\
";
		}
#endif

		clip += L"\
Profile: Default\r\n\
   Test: %TestSize% (x%TestCount%)%TestMode% [%IntervalTime%] %Affinity%\r\n\
   Date: %Date%\r\n\
     OS: %OS%\r\n\
%Comment%\r\n\
";
	}
	else
	{
		clip = L"\
------------------------------------------------------------------------------\r\n\
%PRODUCT% %VERSION%%EDITION% (C) %COPY_YEAR% hiyohiyo\r\n\
                                  Crystal Dew World: https://crystalmark.info/\r\n\
------------------------------------------------------------------------------\r\n\
* MB/s = 1,000,000 bytes/s [SATA/600 = 600,000,000 bytes/s]\r\n\
* KB = 1000 bytes, KiB = 1024 bytes\r\n\
\r\n\
[Read]\r\n\
%SequentialRead1%\r\n\
%RandomRead1%\r\n\
\r\n\
[Write]\r\n\
%SequentialWrite1%\r\n\
%RandomWrite1%\r\n\
\r\n\
";

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip += L"\
[Mix] %MixRatio%\r\n\
%SequentialMix1%\r\n\
%RandomMix1%\r\n\
\r\n\
";
		}
#endif

		if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
		{
			clip += L"\
Profile: Peak\r\n\
";
		}
		else
		{
			clip += L"\
Profile: Real\r\n\
";
		}

		clip += L"\
   Test: %TestSize% (x%TestCount%)%TestMode% [%IntervalTime%] %Affinity%\r\n\
   Date: %Date%\r\n\
     OS: %OS%\r\n\
%Comment%\
";
	}

	clip.Replace(_T("%PRODUCT%"), PRODUCT_NAME);
	clip.Replace(_T("%VERSION%"), PRODUCT_VERSION);
	
	cstr = PRODUCT_EDITION;
	if(! cstr.IsEmpty())
	{
		clip.Replace(_T("%EDITION%"), _T(" ") PRODUCT_EDITION);
	}
	else
	{
		clip.Replace(_T("%EDITION%"), PRODUCT_EDITION);
	}	
	clip.Replace(_T("%COPY_YEAR%"), PRODUCT_COPY_YEAR);

	double iops = 0.0;
	double latency = 0.0;

	if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		clip.Replace(_T("%SequentialRead1%"), GetSequentialResultString(m_SequentialReadScore1, m_SequentialReadLatency1, 1, 1, 1));
		clip.Replace(_T("%SequentialWrite1%"), GetSequentialResultString(m_SequentialWriteScore1, m_SequentialWriteLatency1, 1, 1, 1));
		clip.Replace(_T("%RandomRead1%"), GetRandomResultString(m_RandomReadScore1, m_RandomReadLatency1, 4, 1, 1));
		clip.Replace(_T("%RandomWrite1%"), GetRandomResultString(m_RandomWriteScore1, m_RandomWriteLatency1, 4, 1, 1));

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip.Replace(_T("%SequentialMix1%"), GetSequentialResultString(m_SequentialMixScore1, m_SequentialMixLatency1, 1, 1, 1));
			clip.Replace(_T("%RandomMix1%"), GetRandomResultString(m_RandomMixScore1, m_RandomMixLatency1, 4, 1, 1));
			cstr.Format(_T("Read %d%%/Write %d%%"), 100 - m_MixRatio, m_MixRatio);
			clip.Replace(_T("%MixRatio%"), cstr);
		}
#endif
	}
	else 
	{
		clip.Replace(_T("%SequentialRead1%"), GetSequentialResultString(m_SequentialReadScore1, m_SequentialReadLatency1, m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1));
		clip.Replace(_T("%SequentialRead2%"), GetSequentialResultString(m_SequentialReadScore2, m_SequentialReadLatency2, m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2));
		clip.Replace(_T("%SequentialWrite1%"), GetSequentialResultString(m_SequentialWriteScore1, m_SequentialWriteLatency1, m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1));
		clip.Replace(_T("%SequentialWrite2%"), GetSequentialResultString(m_SequentialWriteScore2, m_SequentialWriteLatency2, m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2));

		clip.Replace(_T("%RandomRead1%"), GetRandomResultString(m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1, m_RandomQueues1, m_RandomThreads1));
		clip.Replace(_T("%RandomRead2%"), GetRandomResultString(m_RandomReadScore2, m_RandomReadLatency2, m_RandomSize2, m_RandomQueues2, m_RandomThreads2));
		clip.Replace(_T("%RandomWrite1%"), GetRandomResultString(m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1, m_RandomQueues1, m_RandomThreads1));
		clip.Replace(_T("%RandomWrite2%"), GetRandomResultString(m_RandomWriteScore2, m_RandomWriteLatency2, m_RandomSize2, m_RandomQueues2, m_RandomThreads2));

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip.Replace(_T("%SequentialMix1%"), GetSequentialResultString(m_SequentialMixScore1, m_SequentialMixLatency1, m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1));
			clip.Replace(_T("%SequentialMix2%"), GetSequentialResultString(m_SequentialMixScore2, m_SequentialMixLatency2, m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2));
			clip.Replace(_T("%RandomMix1%"), GetRandomResultString(m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1, m_RandomQueues1, m_RandomThreads1));
			clip.Replace(_T("%RandomMix2%"), GetRandomResultString(m_RandomMixScore2, m_RandomMixLatency2, m_RandomSize2, m_RandomQueues2, m_RandomThreads2));

			cstr.Format(_T("Read %d%%/Write %d%%"), 100 - m_MixRatio, m_MixRatio);
			clip.Replace(_T("%MixRatio%"), cstr);
		}
#endif
	}

	if (m_ValueTestSize.Find(L"MiB") == -1)
	{
		cstr.Format(_T("%d GiB"), _tstoi(m_ValueTestSize));
	}
	else
	{
		cstr.Format(_T("%d MiB"), _tstoi(m_ValueTestSize));
	}

	clip.Replace(_T("%TestSize%"), cstr);
	cstr.Format(_T("%d"), _tstoi(m_ValueTestCount));
	clip.Replace(_T("%TestCount%"), cstr);

	if (m_Affinity == AFFINITY_DISABLED)
	{
		cstr = L"<DefaultAffinity=DISABLED>";
	}
	else
	{
		cstr = L"<DefaultAffinity=ENABLED>";
	}
	clip.Replace(_T("%Affinity%"), cstr);

	m_CommentEx.GetWindowText(cstr);

	if (cstr.IsEmpty())
	{
		clip.Replace(_T("%Comment%"), _T(""));
	}else
	{
		clip.Replace(_T("%Comment%"), L"Comment: " + cstr + _T("\r\n"));
	}

	if(m_TestData == TEST_DATA_ALL0X00)
	{
		clip.Replace(_T("%TestMode%"), _T(" ") ALL_0X00_0FILL);
	}
	else
	{
		clip.Replace(_T("%TestMode%"), _T(""));
	}
	cstr.Format(L"Interval: %d sec", m_IntervalTime);
	clip.Replace(_T("%IntervalTime%"), cstr);

	/*
	if (m_FragmenteCounts > 0)
	{
		cstr.Format(L"[%d]", m_FragmenteCounts);
		clip.Replace(_T("%FragmenteCounts%"), cstr);
	}
	else
	{
		clip.Replace(_T("%FragmenteCounts%"), _T(""));
	}
	*/

	GetOsName(cstr);
	clip.Replace(_T("%OS%"), cstr);

	SYSTEMTIME st;
	GetLocalTime(&st);
	cstr.Format(_T("%04d/%02d/%02d %d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	clip.Replace(_T("%Date%"), cstr);

	if (type == RESULT_TEXT_CLIPBOARD)
	{
		if(OpenClipboard())
		{
			HGLOBAL clipbuffer;
			TCHAR* buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, sizeof(TCHAR) * (clip.GetLength() + 1));
			if(clipbuffer != NULL)
			{
				buffer = (TCHAR*)GlobalLock(clipbuffer);
				if (buffer != NULL)
				{
					_tcscpy_s(buffer, clip.GetLength() + 1, LPCTSTR(clip));
				}
				GlobalUnlock(clipbuffer);
			}			
			
	#ifdef _UNICODE
			SetClipboardData(CF_UNICODETEXT, clipbuffer);
	#else
			SetClipboardData(CF_OEMTEXT, clipbuffer);
	#endif
			CloseClipboard();
		}
	}
	else
	{
		CString cstr1, cstr2, cstr3;
		TCHAR path[_MAX_PATH];
		UCHAR bom[2];bom[0] = 0xFF;bom[1] = 0xFE; // UTF-16LE BOM
		SYSTEMTIME st;
		GetLocalTime(&st);
		cstr2.Format(L"CDM_%04d%02d%02d%0d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		cstr1 = "txt";
		cstr3 = "*.txt|*.txt|";
		CFileDialog save(FALSE, cstr1, cstr2, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, cstr3);
		SHGetSpecialFolderPath(NULL, path, CSIDL_DESKTOP, TRUE);
		save.m_ofn.lpstrInitialDir = path;
		if (save.DoModal() == IDOK)
		{
			FILE*	pFile;
			_wfopen_s(&pFile, save.GetPathName(), L"wb,ccs=UNICODE");
			if (pFile != NULL)
			{
				fwrite(bom, sizeof(UCHAR), sizeof(bom), pFile);
				fwprintf(pFile, L"%s", (LPCTSTR)clip);
				fclose(pFile);
			}
		}
	}
}

void CDiskMarkDlg::OnZoom100()
{
	if (CheckRadioZoomType(ID_ZOOM_100, 100))
	{
		UpdateDialogSize();
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom125()
{
	if (CheckRadioZoomType(ID_ZOOM_125, 125))
	{
		UpdateDialogSize();
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom150()
{
	if (CheckRadioZoomType(ID_ZOOM_150, 150))
	{
		UpdateDialogSize();
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom200()
{
	if (CheckRadioZoomType(ID_ZOOM_200, 200))
	{
		UpdateDialogSize();
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom250()
{
	if (CheckRadioZoomType(ID_ZOOM_250, 250))
	{
		UpdateDialogSize();
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom300()
{
	if (CheckRadioZoomType(ID_ZOOM_300, 300))
	{
		UpdateDialogSize();
		UpdateDialogSize();
	}
}

/*
void CDiskMarkDlg::OnZoom400()
{
	if (CheckRadioZoomType(ID_ZOOM_400, 400))
	{
		UpdateDialogSize();
	}
}
*/

void CDiskMarkDlg::OnZoomAuto()
{
	if (CheckRadioZoomType(ID_ZOOM_AUTO, 0))
	{
		UpdateDialogSize();
		UpdateDialogSize();
	}
}

BOOL CDiskMarkDlg::CheckRadioZoomType(int id, int value)
{
	if(m_ZoomType == value)
	{
		return FALSE;
	}

	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_ZOOM_100, ID_ZOOM_AUTO, id, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_ZoomType = value;

	CString cstr;
	cstr.Format(_T("%d"), value);
	WritePrivateProfileString(_T("Setting"), _T("ZoomType"), cstr, m_Ini);

	ChangeZoomType(m_ZoomType);

	return TRUE;
}

void CDiskMarkDlg::CheckRadioZoomType()
{
	int id = ID_ZOOM_AUTO;

	switch(m_ZoomType)
	{
	case 100: id = ID_ZOOM_100;	break;
	case 125: id = ID_ZOOM_125;	break;
	case 150: id = ID_ZOOM_150;	break;
	case 200: id = ID_ZOOM_200;	break;
	case 250: id = ID_ZOOM_250;	break;
	case 300: id = ID_ZOOM_300;	break;
//	case 400: id = ID_ZOOM_400;	break;
	default:  id = ID_ZOOM_AUTO;	break;
	}

	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_ZOOM_100, ID_ZOOM_AUTO, id, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();
}

void CDiskMarkDlg::OnIntervalTime0(){   CheckRadioIntervalTime(ID_INTERVAL_TIME_0, 0);}
void CDiskMarkDlg::OnIntervalTime1(){   CheckRadioIntervalTime(ID_INTERVAL_TIME_1, 1); }
void CDiskMarkDlg::OnIntervalTime3(){   CheckRadioIntervalTime(ID_INTERVAL_TIME_3, 3); }
void CDiskMarkDlg::OnIntervalTime5(){   CheckRadioIntervalTime(ID_INTERVAL_TIME_5, 5); }
void CDiskMarkDlg::OnIntervalTime10(){  CheckRadioIntervalTime(ID_INTERVAL_TIME_10, 10); }
void CDiskMarkDlg::OnIntervalTime30(){  CheckRadioIntervalTime(ID_INTERVAL_TIME_30, 30); }
void CDiskMarkDlg::OnIntervalTime60(){  CheckRadioIntervalTime(ID_INTERVAL_TIME_60, 60); }
void CDiskMarkDlg::OnIntervalTime180(){ CheckRadioIntervalTime(ID_INTERVAL_TIME_180, 180); }
void CDiskMarkDlg::OnIntervalTime300(){ CheckRadioIntervalTime(ID_INTERVAL_TIME_300, 300); }
void CDiskMarkDlg::OnIntervalTime600(){ CheckRadioIntervalTime(ID_INTERVAL_TIME_600, 600); }

BOOL CDiskMarkDlg::CheckRadioIntervalTime(int id, int value)
{
	if (m_WinThread != NULL)
	{
		AfxMessageBox(m_MesStopBenchmark);
		return FALSE;
	}

	if (m_IntervalTime == value)
	{
		return FALSE;
	}

	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_INTERVAL_TIME_0, ID_INTERVAL_TIME_600, id, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_IntervalTime = value;

	CString cstr;
	cstr.Format(_T("%d"), value);
	WritePrivateProfileString(_T("Setting"), _T("IntervalTime"), cstr, m_Ini);

	return TRUE;
}

void CDiskMarkDlg::CheckIntervalTime()
{
	int id;
	m_IntervalTime = GetPrivateProfileInt(_T("Setting"), _T("IntervalTime"), 5, m_Ini);

	switch (m_IntervalTime)
	{
	case   0: id = ID_INTERVAL_TIME_0;	break;
	case   1: id = ID_INTERVAL_TIME_1;	break;
	case   3: id = ID_INTERVAL_TIME_3;	break;
	case   5: id = ID_INTERVAL_TIME_5;	break;
	case  10: id = ID_INTERVAL_TIME_10;	break;
	case  30: id = ID_INTERVAL_TIME_30;	break;
	case  60: id = ID_INTERVAL_TIME_60;	break;
	case 180: id = ID_INTERVAL_TIME_180;break;
	case 300: id = ID_INTERVAL_TIME_300;break;
	case 600: id = ID_INTERVAL_TIME_600;break;
	default:
		id = ID_INTERVAL_TIME_5;
		WritePrivateProfileString(_T("Setting"), _T("IntervalTime"), L"5", m_Ini); 
		break;
	}

	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_INTERVAL_TIME_0, ID_INTERVAL_TIME_600, id, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();
}

void CDiskMarkDlg::OnHelp()
{
	if (GetUserDefaultLCID() == 0x0411) // Japanese
	{
		OpenUrl(URL_HTML_HELP_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_HTML_HELP_EN);
	}
}

void CDiskMarkDlg::OnCrystalDewWorld()
{
	if (GetUserDefaultLCID() == 0x0411) // Japanese
	{
		OpenUrl(URL_CRYSTAL_DEW_WORLD_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_CRYSTAL_DEW_WORLD_EN);
	}
}

void CDiskMarkDlg::OnModeDefault()
{
	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_MODE_DEFAULT, ID_MODE_ALL0X00, ID_MODE_DEFAULT, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_TestData = TEST_DATA_RANDOM;
	WritePrivateProfileString(_T("Setting"), _T("TestData"), _T("0"), m_Ini);
	SetWindowTitle(L"");
}

void CDiskMarkDlg::OnModeAll0x00()
{
	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_MODE_DEFAULT, ID_MODE_ALL0X00, ID_MODE_ALL0X00, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_TestData = TEST_DATA_ALL0X00;
	WritePrivateProfileString(_T("Setting"), _T("TestData"), _T("1"), m_Ini);
	SetWindowTitle(L"");
}

void CDiskMarkDlg::OnProfileDefault()
{
	ProfileDefault();
	ChangeButtonStatus(TRUE);
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfileDefault()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_REAL_MIX, ID_PROFILE_DEFAULT, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_DEFAULT;
	m_MixMode = FALSE;
	WritePrivateProfileString(_T("Setting"), _T("Profile"), _T("0"), m_Ini);
}

void CDiskMarkDlg::OnProfilePeak()
{
	ProfilePeak();
	ChangeButtonStatus(TRUE);
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfilePeak()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_REAL_MIX, ID_PROFILE_PEAK, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_PEAK;
	m_MixMode = FALSE;
	WritePrivateProfileString(_T("Setting"), _T("Profile"), _T("1"), m_Ini);
}

void CDiskMarkDlg::OnProfileReal()
{
	ProfileReal();
	ChangeButtonStatus(TRUE);
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfileReal()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_REAL_MIX, ID_PROFILE_REAL, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_REAL;
	m_MixMode = FALSE;
	WritePrivateProfileString(_T("Setting"), _T("Profile"), _T("2"), m_Ini);
}

#ifdef MIX_MODE
void CDiskMarkDlg::OnProfileDefaultMix()
{
	ProfileDefaultMix();
	ChangeButtonStatus(TRUE);
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfileDefaultMix()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_REAL_MIX, ID_PROFILE_DEFAULT_MIX, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_DEFAULT_MIX;
	m_MixMode = TRUE;
	WritePrivateProfileString(_T("Setting"), _T("Profile"), _T("3"), m_Ini);
}

void CDiskMarkDlg::OnProfilePeakMix()
{
	ProfilePeakMix();
	ChangeButtonStatus(TRUE);
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfilePeakMix()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_REAL_MIX, ID_PROFILE_PEAK_MIX, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_PEAK_MIX;
	m_MixMode = TRUE;
	WritePrivateProfileString(_T("Setting"), _T("Profile"), _T("4"), m_Ini);
}

void CDiskMarkDlg::OnProfileRealMix()
{
	ProfileRealMix();
	ChangeButtonStatus(TRUE);
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfileRealMix()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_REAL_MIX, ID_PROFILE_REAL_MIX, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_REAL_MIX;
	m_MixMode = TRUE;
	WritePrivateProfileString(_T("Setting"), _T("Profile"), _T("5"), m_Ini);
}
#endif

void CDiskMarkDlg::OnSettingsQueuesThreads()
{
	static BOOL once = TRUE;

	if (! m_DiskBenchStatus && once)
	{
		once = FALSE;
		CSettingsDlg dlg(this);
		dlg.DoModal();
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);
		once = TRUE;
	}
}

typedef BOOL(WINAPI *FuncEnableNonClientDpiScaling) (HWND hwnd);

BOOL CDiskMarkDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CMainDialog::OnNcCreate(lpCreateStruct))
		return FALSE;

	HMODULE hModule = GetModuleHandle(_T("User32.dll"));
	if (hModule != NULL)
	{
		FuncEnableNonClientDpiScaling pEnableNonClientDpiScaling = (FuncEnableNonClientDpiScaling)GetProcAddress(hModule, "EnableNonClientDpiScaling");
		if (pEnableNonClientDpiScaling != NULL)
		{
			pEnableNonClientDpiScaling(m_hWnd);
		}
	}

	return TRUE;
}

void CDiskMarkDlg::OnFontSetting()
{
	CFontSelection fontSelection(this);
	if (fontSelection.DoModal() == IDOK)
	{
		m_FontFace = fontSelection.GetFontFace();
		m_FontType = fontSelection.GetFontType();
		m_FontScale = fontSelection.GetFontScale();
	//	SetControlFont();
	//	Invalidate();
		CString cstr;
		WritePrivateProfileString(_T("Setting"), _T("FontFace"), _T("\"") + m_FontFace + _T("\""), m_Ini);
		cstr.Format(L"%d", m_FontType);
		WritePrivateProfileString(_T("Setting"), _T("FontType"), cstr, m_Ini);
		cstr.Format(L"%d", m_FontScale);
		WritePrivateProfileString(_T("Setting"), _T("FontScale"), cstr, m_Ini);

		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnCbnSelchangeComboDrive()
{
	SelectDrive();
}

void CDiskMarkDlg::OnCbnSelchangeComboUnit()
{
	UpdateScore();
	UpdateUnitLabel();
}

#ifdef MIX_MODE
void CDiskMarkDlg::OnCbnSelchangeComboMix()
{
	UpdateData(TRUE);

	m_MixRatio = (9 - m_IndexTestMix) * 10;

	CString cstr;
	cstr.Format(L"%d", m_MixRatio);
	WritePrivateProfileString(L"Setting", L"TestMix", cstr, m_Ini);
}
#endif

void CDiskMarkDlg::MoveForcus()
{
	GotoDlgCtrl(GetDlgItem(IDOK));
}

void CDiskMarkDlg::UpdateUnitLabel()
{
	if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		m_ReadUnit.SetWindowTextW(L"Read [MB/s]");
		m_WriteUnit.SetWindowTextW(L"Write [MB/s]");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix [MB/s]");
#endif
		return;
	}
	else if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		m_ReadUnit.SetWindowTextW(L"Read [MB/s]");
		m_WriteUnit.SetWindowTextW(L"Write [MB/s]");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix [MB/s]");
#endif
		return;
	}

	if (m_IndexTestUnit == SCORE_UNIT::SCORE_IOPS)
	{
		m_ReadUnit.SetWindowTextW(L"Read [IOPS]");
		m_WriteUnit.SetWindowTextW(L"Write [IOPS]");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix [IOPS]");
#endif
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_US)
	{
		m_ReadUnit.SetWindowTextW(L"Read [μs]");
		m_WriteUnit.SetWindowTextW(L"Write [μs]");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix [μs]");
#endif
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_GBS)
	{
		m_ReadUnit.SetWindowTextW(L"Read [GB/s]");
		m_WriteUnit.SetWindowTextW(L"Write [GB/s]");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix [GB/s]");
#endif
	}
	else
	{
		m_ReadUnit.SetWindowTextW(L"Read [MB/s]");
		m_WriteUnit.SetWindowTextW(L"Write [MB/s]");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix [MB/s]");
#endif
	}
}

void CDiskMarkDlg::SetWindowTitle(CString message)
{
	CString title;

	if (!message.IsEmpty())
	{
		title.Format(_T("%s - %s"), PRODUCT_SHORT_NAME, message.GetString());
	}
	else
	{
		title.Format(_T("%s %s %s"), PRODUCT_NAME, PRODUCT_VERSION, PRODUCT_EDITION);
	}

	if (m_AdminMode)
	{
		title += L" [ADMIN]";
	}

	switch (m_Profile)
	{
	case PROFILE_PEAK:
	case PROFILE_PEAK_MIX:
		title += L" <PEAK>";
		break;
	case PROFILE_REAL:
	case PROFILE_REAL_MIX:
		title += L" <REAL>";
		break;
	}

	if (m_TestData == TEST_DATA_ALL0X00)
	{
		title += L" <0Fill>";
	}

	SetWindowText(title);
}

HBRUSH CDiskMarkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogCx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID()) {
	case IDC_COMMENT:
		if(m_IsHighContrast)
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->SetBkColor(RGB(0, 0, 0));
		}
		else
		{
			pDC->SetTextColor(m_EditText);
			pDC->SetBkColor(m_EditBg);
		}
		hbr = (HBRUSH)m_EditBrush->GetSafeHandle();
		break;
	default:
		break;
	}
	return hbr;
}

void CDiskMarkDlg::OnChangeComment()
{
//	CString cstr;
//	m_Comment.GetWindowTextW(cstr);
//	m_CommentEx.SetWindowTextW(cstr);
}

void CDiskMarkDlg::OnCommentEx()
{
	m_CommentEx.GetWindowTextW(m_CommentExchange);

	CCommentDlg dlg(this);
	dlg.DoModal();

//	m_CommentEx.ShowWindow(SW_HIDE);
//	m_Comment.ShowWindow(SW_SHOW);
//	GotoDlgCtrl(GetDlgItem(IDC_COMMENT));
//	m_Comment.SetSel(-1);
}

/*
void CDiskMarkDlg::OnKillFocusComment()
{
	m_CommentEx.ShowWindow(SW_SHOW);
	m_Comment.ShowWindow(SW_HIDE);
}

void CDiskMarkDlg::OnSetFocusCommentEx()
{
	m_CommentEx.ShowWindow(SW_HIDE);
	GotoDlgCtrl(GetDlgItem(IDC_COMMENT));
	m_Comment.SetSel(-1);
	m_Comment.ShowWindow(SW_SHOW);
}
*/