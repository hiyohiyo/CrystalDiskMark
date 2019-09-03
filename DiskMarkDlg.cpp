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
#define SIZE_Y		600
#define SIZE_MIN_Y	600
#define OFFSET_X    200
#define MAX_METER_LENGTH	320
#else
#define SIZE_X		480
#define SIZE_Y		360
#define SIZE_MIN_Y	360
#define OFFSET_X    0
#define MAX_METER_LENGTH	192
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	DDX_Control(pDX, IDC_BUTTON_SEQUENTIAL_1, m_ButtonSequential1);
	DDX_Control(pDX, IDC_BUTTON_SEQUENTIAL_2, m_ButtonSequential2);
	DDX_Control(pDX, IDC_BUTTON_RANDOM_1, m_ButtonRandom1);
	DDX_Control(pDX, IDC_BUTTON_RANDOM_2, m_ButtonRandom2);
	DDX_Control(pDX, IDC_BUTTON_RANDOM_3, m_ButtonRandom3);

	DDX_Control(pDX, IDC_SEQUENTIAL_READ_1, m_SequentialRead1);
	DDX_Control(pDX, IDC_SEQUENTIAL_READ_2, m_SequentialRead2);
	DDX_Control(pDX, IDC_RANDOM_READ_1, m_RandomRead1);
	DDX_Control(pDX, IDC_RANDOM_READ_2, m_RandomRead2);
	DDX_Control(pDX, IDC_RANDOM_READ_3, m_RandomRead3);

	DDX_Control(pDX, IDC_SEQUENTIAL_WRITE_1, m_SequentialWrite1);
	DDX_Control(pDX, IDC_SEQUENTIAL_WRITE_2, m_SequentialWrite2);
	DDX_Control(pDX, IDC_RANDOM_WRITE_1, m_RandomWrite1);
	DDX_Control(pDX, IDC_RANDOM_WRITE_2, m_RandomWrite2);
	DDX_Control(pDX, IDC_RANDOM_WRITE_3, m_RandomWrite3);

#ifdef PRO_MODE
	DDX_Control(pDX, IDC_SEQUENTIAL_MIX_1, m_SequentialMix1);
	DDX_Control(pDX, IDC_SEQUENTIAL_MIX_2, m_SequentialMix2);
	DDX_Control(pDX, IDC_RANDOM_MIX_1, m_RandomMix1);
	DDX_Control(pDX, IDC_RANDOM_MIX_2, m_RandomMix2);
	DDX_Control(pDX, IDC_RANDOM_MIX_3, m_RandomMix3);
#endif

	DDX_Control(pDX, IDC_COMMENT, m_Comment);

	DDX_Control(pDX, IDC_COMBO_UNIT, m_ComboUnit);
	DDX_Control(pDX, IDC_COMBO_COUNT, m_ComboCount);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_ComboSize);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_ComboDrive);

	DDX_Control(pDX, IDC_READ_UNIT, m_ReadUnit);
	DDX_Control(pDX, IDC_WRITE_UNIT, m_WriteUnit);

#ifdef PRO_MODE
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
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_RESULT_SAVE, &CDiskMarkDlg::OnResultSave)
	ON_COMMAND(ID_SETTINGS_QUEUESTHREADS, &CDiskMarkDlg::OnSettingsQueuesThreads)
	ON_COMMAND(ID_FONT_SETTING, &CDiskMarkDlg::OnFontSetting)
	ON_WM_NCCREATE()
	ON_MESSAGE(WM_QUERYENDSESSION, &CDiskMarkDlg::OnQueryEndSession)

	ON_BN_CLICKED(IDC_BUTTON_ALL, &CDiskMarkDlg::OnAll)
	ON_BN_CLICKED(IDC_BUTTON_SEQUENTIAL_1, &CDiskMarkDlg::OnSequential1)
	ON_BN_CLICKED(IDC_BUTTON_SEQUENTIAL_2, &CDiskMarkDlg::OnSequential2)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM_1, &CDiskMarkDlg::OnRandom4KB1)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM_2, &CDiskMarkDlg::OnRandom4KB2)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM_3, &CDiskMarkDlg::OnRandom4KB3)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVE, &CDiskMarkDlg::OnCbnSelchangeComboDrive)
	ON_CBN_SELCHANGE(IDC_COMBO_UNIT, &CDiskMarkDlg::OnCbnSelchangeComboUnit)

	ON_CBN_CLOSEUP(IDC_COMBO_UNIT, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_COUNT, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_SIZE, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_DRIVE, &CDiskMarkDlg::MoveForcus)
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

	m_FontScale = GetPrivateProfileInt(_T("Setting"), _T("FontScale"), 100, m_Ini);
	if (m_FontScale > 200 || m_FontScale < 50)
	{
		m_FontScale = 100;
	}

	m_Affinity = GetPrivateProfileInt(_T("Setting"), _T("Affinity"), AFFINITY_OFF, m_Ini);
	if (m_Affinity < 0 || m_Affinity > 1)
	{
		m_Affinity = AFFINITY_OFF;
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

	GetPrivateProfileString(L"Title", L"TEST_UNIT", L"Test Unit", str, 256, m_CurrentLangPath);
	m_ComboUnit.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_COUNT", L"Test Count", str, 256, m_CurrentLangPath);
	m_ComboCount.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_SIZE", L"Test Size", str, 256, m_CurrentLangPath);
	m_ComboSize.SetToolTipText(str);

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

	if(m_TestData == TEST_DATA_ALL0X00)
	{
		SetWindowTitle(_T(""), ALL_0X00_0FILL);
	}
	else
	{
		SetWindowTitle(_T(""), _T(""));
	}

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
	ShowWindow(SW_HIDE);
	UpdateBackground(true);

	m_SizeX = SIZE_X;
	m_SizeY = SIZE_Y;

	SetControlFont();
	
#ifdef PRO_MODE
	m_ButtonAll.InitControl(        8 + OFFSET_X,  8, 120, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_ButtonSequential1.InitControl(8 + OFFSET_X, 96, 120, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_ButtonSequential2.InitControl(8 + OFFSET_X,184, 120, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_ButtonRandom1.InitControl(    8 + OFFSET_X,272, 120, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_ButtonRandom2.InitControl(    8 + OFFSET_X,360, 120, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_ButtonRandom3.InitControl(    8 + OFFSET_X,448, 120, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);

	m_ButtonAll.SetHandCursor(TRUE);
	m_ButtonSequential1.SetHandCursor(TRUE);
	m_ButtonSequential2.SetHandCursor(TRUE);
	m_ButtonRandom1.SetHandCursor(TRUE);
	m_ButtonRandom2.SetHandCursor(TRUE);
	m_ButtonRandom3.SetHandCursor(TRUE);

	m_SequentialRead1.InitControl(140 + OFFSET_X,  96, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_SequentialRead2.InitControl(140 + OFFSET_X, 184, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomRead1.InitControl(    140 + OFFSET_X, 272, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomRead2.InitControl(    140 + OFFSET_X, 360, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomRead3.InitControl(    140 + OFFSET_X, 448, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);

	m_SequentialWrite1.InitControl(316 + OFFSET_X,  96, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_SequentialWrite2.InitControl(316 + OFFSET_X, 184, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomWrite1.InitControl(    316 + OFFSET_X, 272, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomWrite2.InitControl(    316 + OFFSET_X, 360, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomWrite3.InitControl(    316 + OFFSET_X, 448, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);

	m_SequentialMix1.InitControl(492 + OFFSET_X, 96, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_SequentialMix2.InitControl(492 + OFFSET_X, 184, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomMix1.InitControl    (492 + OFFSET_X, 272, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomMix2.InitControl    (492 + OFFSET_X, 360, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomMix3.InitControl    (492 + OFFSET_X, 448, 172, 80, m_ZoomRatio, IP(L"line"), 1, SS_RIGHT, CStaticCx::OwnerDrawGlass | m_IsHighContrast);

	m_Comment.MoveWindow((int)((8 + OFFSET_X) * m_ZoomRatio), (int)(536 * m_ZoomRatio), (int)(656 * m_ZoomRatio), (int)(40 * m_ZoomRatio));

	m_ReadUnit.InitControl( 140 + OFFSET_X, 56, 172, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_WriteUnit.InitControl(316 + OFFSET_X, 56, 172, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_MixUnit.InitControl  (492 + OFFSET_X, 56, 172, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_ComboUnit.MoveWindow((int)((0 + OFFSET_X) * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(56 * m_ZoomRatio), (int)(48 * m_ZoomRatio));
	m_ComboCount.MoveWindow((int)((140 + OFFSET_X) * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(56 * m_ZoomRatio), (int)(48 * m_ZoomRatio));
	m_ComboSize.MoveWindow((int)((204 + OFFSET_X) * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(140 * m_ZoomRatio), (int)(48 * m_ZoomRatio));
	m_ComboDrive.MoveWindow((int)((352 + OFFSET_X) * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(312 * m_ZoomRatio), (int)(48 * m_ZoomRatio));
#else
#ifdef SUISHO_SHIZUKU_SUPPORT
	m_ButtonAll.InitControl(12 + OFFSET_X, 12, 120, 80, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonSequential1.InitControl(12 + OFFSET_X, 100, 120, 80, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonSequential2.InitControl(12 + OFFSET_X, 188, 120, 80, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom1.InitControl(12 + OFFSET_X, 276, 120, 80, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom2.InitControl(12 + OFFSET_X, 364, 120, 80, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom3.InitControl(12 + OFFSET_X, 452, 120, 80, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);

	m_ButtonAll.SetHandCursor(TRUE);
	m_ButtonSequential1.SetHandCursor(TRUE);
	m_ButtonSequential2.SetHandCursor(TRUE);
	m_ButtonRandom1.SetHandCursor(TRUE);
	m_ButtonRandom2.SetHandCursor(TRUE);
	m_ButtonRandom3.SetHandCursor(TRUE);

	m_SequentialRead1.InitControl(140 + OFFSET_X, 100, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_SequentialRead2.InitControl(140 + OFFSET_X, 188, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomRead1.InitControl(140 + OFFSET_X, 276, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomRead2.InitControl(140 + OFFSET_X, 364, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomRead3.InitControl(140 + OFFSET_X, 452, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_SequentialWrite1.InitControl(468 + OFFSET_X, 100, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_SequentialWrite2.InitControl(468 + OFFSET_X, 188, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomWrite1.InitControl(468 + OFFSET_X, 276, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomWrite2.InitControl(468 + OFFSET_X, 364, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomWrite3.InitControl(468 + OFFSET_X, 452, 320, 80, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_Comment.MoveWindow((int)((12 + OFFSET_X) * m_ZoomRatio), (int)(544 * m_ZoomRatio), (int)(776 * m_ZoomRatio), (int)(44 * m_ZoomRatio));

	m_ReadUnit.InitControl(140 + OFFSET_X, 60, 320, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_WriteUnit.InitControl(468 + OFFSET_X, 60, 320, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_ComboCount.InitControl(140 + OFFSET_X, 12, 60, 500, m_ZoomRatio);
	m_ComboSize.InitControl(204 + OFFSET_X, 12, 140, 500, m_ZoomRatio);
	m_ComboDrive.InitControl(348 + OFFSET_X, 12, 320, 500, m_ZoomRatio);
	m_ComboUnit.InitControl(672 + OFFSET_X, 12, 116, 500, m_ZoomRatio);

#else

	m_ButtonAll.InitControl        (8 + OFFSET_X,   8, 72, 48, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonSequential1.InitControl(8 + OFFSET_X,  60, 72, 48, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonSequential2.InitControl(8 + OFFSET_X, 112, 72, 48, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom1.InitControl    (8 + OFFSET_X, 164, 72, 48, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom2.InitControl    (8 + OFFSET_X, 216, 72, 48, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom3.InitControl    (8 + OFFSET_X, 268, 72, 48, m_ZoomRatio, IP(L"button"), 3, SS_CENTER, CButtonCx::OwnerDrawImage | m_IsHighContrast);

	m_ButtonAll.SetHandCursor(TRUE);
	m_ButtonSequential1.SetHandCursor(TRUE);
	m_ButtonSequential2.SetHandCursor(TRUE);
	m_ButtonRandom1.SetHandCursor(TRUE);
	m_ButtonRandom2.SetHandCursor(TRUE);
	m_ButtonRandom3.SetHandCursor(TRUE);

	m_SequentialRead1.InitControl(84 + OFFSET_X,  60, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_SequentialRead2.InitControl(84 + OFFSET_X, 112, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomRead1.InitControl    (84 + OFFSET_X, 164, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomRead2.InitControl    (84 + OFFSET_X, 216, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomRead3.InitControl    (84 + OFFSET_X, 268, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_SequentialWrite1.InitControl(280 + OFFSET_X,  60, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_SequentialWrite2.InitControl(280 + OFFSET_X, 112, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomWrite1.InitControl    (280 + OFFSET_X, 164, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomWrite2.InitControl    (280 + OFFSET_X, 216, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_RandomWrite3.InitControl    (280 + OFFSET_X, 268, 192, 48, m_ZoomRatio, IP(L"meter"), 2, SS_RIGHT, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_Comment.MoveWindow((int)((8 + OFFSET_X) * m_ZoomRatio), (int)(324 * m_ZoomRatio), (int)(464 * m_ZoomRatio), (int)(28 * m_ZoomRatio));

	m_ReadUnit.InitControl  (84 + OFFSET_X, 36, 192, 24, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_WriteUnit.InitControl(280 + OFFSET_X, 36, 192, 24, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_ComboCount.InitControl( 84 + OFFSET_X, 8,  40, 300, m_ZoomRatio);
	m_ComboSize.InitControl (128 + OFFSET_X, 8,  80, 300, m_ZoomRatio);
	m_ComboDrive.InitControl(212 + OFFSET_X, 8, 188, 300, m_ZoomRatio);
	m_ComboUnit.InitControl (404 + OFFSET_X, 8,  68, 300, m_ZoomRatio);
#endif
#endif

	if (m_EditBrush != NULL)
	{
		m_EditBrush->DeleteObject();
		m_EditBrush->CreateSolidBrush(m_EditBg);
	}

	UpdateScore();
	Invalidate();
	ShowWindow(SW_SHOW);

	m_ComboUnit.ShowWindow(SW_HIDE);
	m_ComboCount.ShowWindow(SW_HIDE);
	m_ComboSize.ShowWindow(SW_HIDE);
	m_ComboDrive.ShowWindow(SW_HIDE);

#ifdef SUISHO_SHIZUKU_SUPPORT
	SetLayeredWindow(m_Comment.m_hWnd, 192);
	SetLayeredWindow(m_ComboUnit.m_hWnd, 192);
	SetLayeredWindow(m_ComboCount.m_hWnd, 192);
	SetLayeredWindow(m_ComboSize.m_hWnd, 192);
	SetLayeredWindow(m_ComboDrive.m_hWnd, 192);
#else
	SetLayeredWindow(m_Comment.m_hWnd, 240);
	SetLayeredWindow(m_ComboUnit.m_hWnd, 240);
	SetLayeredWindow(m_ComboCount.m_hWnd, 240);
	SetLayeredWindow(m_ComboSize.m_hWnd, 240);
	SetLayeredWindow(m_ComboDrive.m_hWnd, 240);
#endif

	COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
	m_ComboUnit.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, 240);
	m_ComboCount.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, 240);
	m_ComboSize.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, 240);
	m_ComboDrive.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, 240);

	m_ComboUnit.ShowWindow(SW_SHOW);
	m_ComboCount.ShowWindow(SW_SHOW);
	m_ComboSize.ShowWindow(SW_SHOW);
	m_ComboDrive.ShowWindow(SW_SHOW);

	SetClientRect((DWORD)(m_SizeX* m_ZoomRatio), (DWORD)(m_SizeY* m_ZoomRatio), 1);

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

#ifdef PRO_MODE
	m_ButtonAll.SetFontEx(m_FontFace, 24, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonSequential1.SetFontEx(m_FontFace, 20, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonSequential2.SetFontEx(m_FontFace, 20, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonRandom1.SetFontEx(m_FontFace, 20, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonRandom2.SetFontEx(m_FontFace, 20, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonRandom3.SetFontEx(m_FontFace, 20, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_SequentialRead1.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_SequentialRead2.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomRead1.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomRead2.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomRead3.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_SequentialWrite1.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_SequentialWrite2.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomWrite1.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomWrite2.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomWrite3.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_SequentialMix1.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_SequentialMix2.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomMix1.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomMix2.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomMix3.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_Comment.SetFontEx(m_FontFace, 32, m_ZoomRatio, FW_BOLD);

	m_ReadUnit.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_WriteUnit.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_MixUnit.SetFontEx(m_FontFace, 32, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_ComboUnit.SetFontEx(m_FontFace, 24, m_ZoomRatio);
	m_ComboCount.SetFontEx(m_FontFace, 24, m_ZoomRatio);
	m_ComboSize.SetFontEx(m_FontFace, 24, m_ZoomRatio);
	m_ComboDrive.SetFontEx(m_FontFace, 24, m_ZoomRatio);
	
	m_ButtonRandom1.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonSequential1.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonSequential2.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonRandom2.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonRandom3.SetMargin(8, 0, 8, 0, m_ZoomRatio);
#else

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_ButtonAll.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonSequential1.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonSequential2.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonRandom1.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonRandom2.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonRandom3.SetFontEx(m_FontFace, (int)(20 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);

	m_SequentialRead1.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_SequentialRead2.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomRead1.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomRead2.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomRead3.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);

	m_SequentialWrite1.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_SequentialWrite2.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomWrite1.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomWrite2.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomWrite3.SetFontEx(m_FontFace, (int)(64 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);

	m_Comment.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, FW_BOLD);

	m_ReadUnit.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);
	m_WriteUnit.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);

	m_ComboUnit.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ComboText, FW_NORMAL, m_FontType);
	m_ComboCount.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ComboText, FW_NORMAL, m_FontType);
	m_ComboSize.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ComboText, FW_NORMAL, m_FontType);
	m_ComboDrive.SetFontEx(m_FontFace, (int)(24 * scale), m_ZoomRatio, textAlpha, m_ComboText, FW_NORMAL, m_FontType);

	m_ButtonRandom1.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonSequential1.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonSequential2.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonRandom2.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonRandom3.SetMargin(8, 0, 8, 0, m_ZoomRatio);

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
	m_ButtonSequential1.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonSequential2.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonRandom1.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonRandom2.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);
	m_ButtonRandom3.SetFontEx(m_FontFace, (int)(12 * scale), m_ZoomRatio, textAlpha, m_ButtonText, FW_BOLD, m_FontType);

	m_SequentialRead1.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_SequentialRead2.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomRead1.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomRead2.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomRead3.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);

	m_SequentialWrite1.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_SequentialWrite2.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomWrite1.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomWrite2.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);
	m_RandomWrite3.SetFontEx(m_FontFace, (int)(36 * scale), m_ZoomRatio, textAlpha, m_MeterText, FW_BOLD, m_FontType);

	m_Comment.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, FW_BOLD);

	m_ReadUnit.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);
	m_WriteUnit.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_LabelText, FW_BOLD, m_FontType);

	m_ComboUnit.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, FW_NORMAL, m_FontType);
	m_ComboCount.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, FW_NORMAL, m_FontType);
	m_ComboSize.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, FW_NORMAL, m_FontType);
	m_ComboDrive.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, textAlpha, m_ComboText, FW_NORMAL, m_FontType);

	m_ButtonRandom1.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonSequential1.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonSequential2.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonRandom2.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonRandom3.SetMargin(4, 0, 4, 0, m_ZoomRatio);

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
#endif

#endif

	m_ComboUnit.SetBgColor(m_ComboBg, m_ComboSelected);
	m_ComboCount.SetBgColor(m_ComboBg, m_ComboSelected);
	m_ComboSize.SetBgColor(m_ComboBg, m_ComboSelected);
	m_ComboDrive.SetBgColor(m_ComboBg, m_ComboSelected);
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

	m_SequentialMultiSize1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiSize1"), 8, m_Ini);
	 if (m_SequentialMultiSize1 <= 0 || m_SequentialMultiSize1 > 8)
	{
		m_SequentialMultiSize1 = 8;
	}

	m_SequentialMultiQueues1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiQueues1"), 1, m_Ini);
	// if (m_SequentialMultiQueues1 <= 0 || m_SequentialMultiQueues1 > MAX_QUEUES)
	{
		m_SequentialMultiQueues1 = 1;
	}

	m_SequentialMultiThreads1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiThreads1"), 1, m_Ini);
	if (m_SequentialMultiThreads1 <= 0 || m_SequentialMultiThreads1 > MAX_THREADS)
	{
		m_SequentialMultiThreads1 = 1;
	}

	m_SequentialMultiQueues2 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiQueues2"), 32, m_Ini);
	if (m_SequentialMultiQueues2 <= 0 || m_SequentialMultiQueues2 > MAX_QUEUES)
	{
		m_SequentialMultiQueues2 = 32;
	}

	m_SequentialMultiThreads2 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiThreads2"), 1, m_Ini);
	if (m_SequentialMultiThreads2 <= 0 || m_SequentialMultiThreads2 > MAX_THREADS)
	{
		m_SequentialMultiThreads2 = 1;
	}
	
	m_RandomMultiQueues1 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiQueues1"), 32, m_Ini);
	if (m_RandomMultiQueues1 <= 0 || m_RandomMultiQueues1 > MAX_QUEUES)
	{
		m_RandomMultiQueues1 = 32;
	}

	m_RandomMultiThreads1 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiThreads1"), 16, m_Ini);
	if (m_RandomMultiThreads1 <= 0 || m_RandomMultiThreads1 > MAX_THREADS)
	{
		m_RandomMultiThreads1 = 16;
	}

	m_RandomMultiQueues2 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiQueues2"), 32, m_Ini);
	if (m_RandomMultiQueues2 <= 0 || m_RandomMultiQueues2 > MAX_QUEUES)
	{
		m_RandomMultiQueues2 = 32;
	}

	m_RandomMultiThreads2 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiThreads2"), 1, m_Ini);
	if (m_RandomMultiThreads2 <= 0 || m_RandomMultiThreads2 > MAX_THREADS)
	{
		m_RandomMultiThreads2 = 1;
	}

	m_RandomMultiQueues3 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiQueues3"), 1, m_Ini);
	if (m_RandomMultiQueues3 <= 0 || m_RandomMultiQueues3 > MAX_QUEUES)
	{
		m_RandomMultiQueues3 = 1;
	}

	m_RandomMultiThreads3 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiThreads3"), 1, m_Ini);
	if (m_RandomMultiThreads3 <= 0 || m_RandomMultiThreads3 > MAX_THREADS)
	{
		m_RandomMultiThreads3 = 1;
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

	UpdateData(TRUE);
	
	if (m_ComboDrive.GetCurSel() == m_MaxIndexTestDrive)
	{
		BROWSEINFO  bi;
		ZeroMemory(&bi, sizeof(BROWSEINFO));
		ITEMIDLIST __unaligned *idl;
		LPMALLOC    g_pMalloc;
		TCHAR       szTmp[MAX_PATH];

		SHGetMalloc(&g_pMalloc);
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
			g_pMalloc->Release();
		}
		m_ComboDrive.SetToolTipText(m_TestTargetPath);
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
	if(m_WinThread != NULL)
	{
		AfxMessageBox(m_MesStopBenchmark);
		return ;
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
	m_RandomRead4KBScore1 = 0.0;
	m_RandomWrite4KBScore1 = 0.0;
	m_RandomRead4KBScore2 = 0.0;
	m_RandomWrite4KBScore2 = 0.0;
	m_RandomRead4KBScore3 = 0.0;
	m_RandomWrite4KBScore3 = 0.0;

#ifdef PRO_MODE
	m_SequentialMixScore1 = 0.0;
	m_SequentialMixScore2 = 0.0;
	m_RandomMix4KBScore1 = 0.0;
	m_RandomMix4KBScore2 = 0.0;
	m_RandomMix4KBScore3 = 0.0;
#endif

	UpdateScore();
}

void CDiskMarkDlg::UpdateScore()
{
	UpdateData(TRUE);

	SetMeter(&m_SequentialRead1, m_SequentialReadScore1, -1);
	SetMeter(&m_SequentialWrite1, m_SequentialWriteScore1, -1);
	SetMeter(&m_SequentialRead2, m_SequentialReadScore2, 128 * 1024);
	SetMeter(&m_SequentialWrite2, m_SequentialWriteScore2, 128 * 1024);
	SetMeter(&m_RandomRead1, m_RandomRead4KBScore1, 4096);
	SetMeter(&m_RandomWrite1, m_RandomWrite4KBScore1, 4096);
	SetMeter(&m_RandomRead2, m_RandomRead4KBScore2, 4096);
	SetMeter(&m_RandomWrite2, m_RandomWrite4KBScore2, 4096);
	SetMeter(&m_RandomRead3, m_RandomRead4KBScore3, 4096);
	SetMeter(&m_RandomWrite3, m_RandomWrite4KBScore3, 4096);

#ifdef PRO_MODE
	SetMeter(&m_SequentialMix1, m_SequentialMixScore1, -1);
	SetMeter(&m_SequentialMix2, m_SequentialMixScore2, 128 * 1024);
	SetMeter(&m_RandomMix1, m_RandomMix4KBScore1, 4096);
	SetMeter(&m_RandomMix2, m_RandomMix4KBScore2, 4096);
	SetMeter(&m_RandomMix3, m_RandomMix4KBScore3, 4096);
#endif
}

void CDiskMarkDlg::SetScoreToolTip(CStaticCx *control, double score, int blockSize)
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
		cstr.Format(L"%.3f MB/s\r\n%.3f GB/s\r\n%.3f IOPS\r\n%.3f μs", score, score / 1000, score * 1000 * 1000 / blockSize, blockSize / score);
	}
	control->SetToolTipText(cstr);
}

void CDiskMarkDlg::OnSequential1()
{
	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_SequentialReadScore1 = 0.0;
		m_SequentialWriteScore1 = 0.0;
#ifdef PRO_MODE
		m_SequentialMixScore1 = 0.0;
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

void CDiskMarkDlg::OnSequential2()
{
	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_SequentialReadScore2 = 0.0;
		m_SequentialWriteScore2 = 0.0;
#ifdef PRO_MODE
		m_SequentialMixScore2 = 0.0;
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

void CDiskMarkDlg::OnRandom4KB1()
{
	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomRead4KBScore1 = 0.0;
		m_RandomWrite4KBScore1 = 0.0;
#ifdef PRO_MODE
		m_RandomMix4KBScore1 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchRandom4KB1, (void*)this);
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

void CDiskMarkDlg::OnRandom4KB2()
{
	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomRead4KBScore2 = 0.0;
		m_RandomWrite4KBScore2 = 0.0;
#ifdef PRO_MODE
		m_RandomMix4KBScore2 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchRandom4KB2, (void*)this);
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

void CDiskMarkDlg::OnRandom4KB3()
{
	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomRead4KBScore3 = 0.0;
		m_RandomWrite4KBScore3 = 0.0;
#ifdef PRO_MODE
		m_RandomMix4KBScore3 = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchRandom4KB3, (void*)this);
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

void CDiskMarkDlg::OnAll()
{
	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		InitScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBenchAll, (void*)this);
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
	SetMenu(menu);
}

void CDiskMarkDlg::ChangeButtonStatus(BOOL status)
{
	if(status)
	{
		m_ComboUnit.EnableWindow(TRUE);
		m_ComboCount.EnableWindow(TRUE);
		m_ComboSize.EnableWindow(TRUE);
		m_ComboDrive.EnableWindow(TRUE);

		m_ButtonAll.SetWindowTextW(L"All");

		CString title;
		title.Format(L"SEQ%dM\r\nT%d", m_SequentialMultiSize1, m_SequentialMultiThreads1);
		m_ButtonSequential1.SetWindowTextW(title);
		title.Format(L"SEQ128K\r\nQ%dT%d", m_SequentialMultiQueues2, m_SequentialMultiThreads2);
		m_ButtonSequential2.SetWindowTextW(title);
		title.Format(L"RND4K\r\nQ%dT%d", m_RandomMultiQueues1, m_RandomMultiThreads1);
		m_ButtonRandom1.SetWindowTextW(title);
		title.Format(L"RND4K\r\nQ%dT%d", m_RandomMultiQueues2, m_RandomMultiThreads2);
		m_ButtonRandom2.SetWindowTextW(title);
		title.Format(L"RND4K\r\nQ%dT%d", m_RandomMultiQueues3, m_RandomMultiThreads3);
		m_ButtonRandom3.SetWindowTextW(title);

		CString toolTip;
		toolTip.Format(L"Sequential 8MiB, Threads = %d", m_SequentialMultiThreads1);
		m_ButtonSequential1.SetToolTipText(toolTip);
		toolTip.Format(L"Sequential 128KiB, Queues=%d, Threads=%d", m_SequentialMultiQueues2, m_SequentialMultiThreads2);
		m_ButtonSequential2.SetToolTipText(toolTip);
		toolTip.Format(L"Random 4KiB, Queues=%d, Threads=%d", m_RandomMultiQueues1, m_RandomMultiThreads1);
		m_ButtonRandom1.SetToolTipText(toolTip);
		toolTip.Format(L"Random 4KiB, Queues=%d, Threads=%d", m_RandomMultiQueues2, m_RandomMultiThreads2);
		m_ButtonRandom2.SetToolTipText(toolTip);
		toolTip.Format(L"Random 4KiB, Queues=%d, Threads=%d", m_RandomMultiQueues3, m_RandomMultiThreads3);
		m_ButtonRandom3.SetToolTipText(toolTip);
	}
	else
	{
		m_ComboUnit.EnableWindow(FALSE);
		m_ComboCount.EnableWindow(FALSE);
		m_ComboSize.EnableWindow(FALSE);
		m_ComboDrive.EnableWindow(FALSE);

		m_ButtonAll.SetWindowTextW(L"Stop");
		m_ButtonSequential1.SetWindowTextW(L"Stop");
		m_ButtonSequential2.SetWindowTextW(L"Stop");
		m_ButtonRandom1.SetWindowTextW(L"Stop");
		m_ButtonRandom2.SetWindowTextW(L"Stop");
		m_ButtonRandom3.SetWindowTextW(L"Stop");

		CString toolTip = L"";
		m_ButtonSequential1.SetToolTipText(toolTip);
		m_ButtonSequential2.SetToolTipText(toolTip);
		m_ButtonRandom1.SetToolTipText(toolTip);
		m_ButtonRandom2.SetToolTipText(toolTip);
		m_ButtonRandom3.SetToolTipText(toolTip);
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

	SetWindowTitle(wstr, lstr);
	return 0;
}

void CDiskMarkDlg::SetMeter(CStaticCx* control, double score, int blockSize)
{
	CString cstr;

	double meterRatio = 0.0;
	if(score > 0.1)
	{
		meterRatio = 0.16666666666666 * log10(score*10);
	}
	else
	{
		meterRatio = 0;
	}

	if (meterRatio > 1.0)
	{
		meterRatio = 1.0;
	}


	if (m_IndexTestUnit == SCORE_UNIT::SCORE_IOPS)
	{
		if (blockSize == -1)
		{
			cstr.Format(_T("----"));
		}
		else
		{
			double iops = score * 1000 * 1000 / blockSize;
			cstr.Format(_T("%d"), (int)iops);
		}
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_US)
	{
		if (blockSize == -1)
		{
			cstr.Format(_T("----"));
		}
		else if (score <= 0.0)
		{
			cstr.Format(_T("%.2f"), 0.0);
		}
		else
		{
			double latency = blockSize / score;
			if (latency >= 1000000.0)
			{
				cstr.Format(_T("%d"), (int)latency);
			}
			else if (latency >= 100000.0)
			{
				cstr.Format(_T("%.1f"), latency);
			}
			else
			{
				cstr.Format(_T("%.2f"), latency);
			}
		}
	}
	else if(m_IndexTestUnit == SCORE_UNIT::SCORE_GBS)
	{
		if (score >= 1000000.0)
		{
			cstr.Format(_T("%d"), (int)(score / 1000.0));
		}
		else if (score >= 100000.0)
		{
			cstr.Format(_T("%.1f"), score / 1000.0);
		}
		else if (score >= 10000.0)
		{
			cstr.Format(_T("%.2f"), score / 1000.0);
		}
		else
		{
			cstr.Format(_T("%.3f"), score / 1000.0);
		}
	}
	else
	{
		if(score >= 1000000.0)
		{
			cstr.Format(_T("%d"), (int)score);
		}
		else if (score >= 100000.0)
		{
			cstr.Format(_T("%.1f"), score);
		}
		else
		{
			cstr.Format(_T("%.2f"), score);
		}
	}

	UpdateData(FALSE);
	control->SetMeter(TRUE, meterRatio);
	control->SetWindowTextW(cstr);

	SetScoreToolTip(control, score, blockSize);
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

	GetPrivateProfileString(L"Title", L"TEST_UNIT", L"Test Unit", str, 256, m_CurrentLangPath);
	m_ComboUnit.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_COUNT", L"Test Count", str, 256, m_CurrentLangPath);
	m_ComboCount.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_SIZE", L"Test Size", str, 256, m_CurrentLangPath);
	m_ComboSize.SetToolTipText(str);

	UpdateData(FALSE);
}

void CDiskMarkDlg::ChangeLang(CString LangName)
{
	m_CurrentLangPath.Format(_T("%s\\%s.lang"), m_LangDir, LangName);

	CString cstr;
	CMenu *menu = GetMenu();
	CMenu subMenu;

	cstr = i18n(_T("Menu"), _T("FILE"));
	menu->ModifyMenu(0, MF_BYPOSITION | MF_STRING, 0, cstr);
	cstr = i18n(_T("Menu"), _T("Settings"));
	menu->ModifyMenu(1, MF_BYPOSITION | MF_STRING, 1, cstr);
	cstr = i18n(_T("Menu"), _T("THEME"));
	menu->ModifyMenu(2, MF_BYPOSITION | MF_STRING, 2, cstr);
	cstr = i18n(_T("Menu"), _T("HELP"));
	menu->ModifyMenu(3, MF_BYPOSITION | MF_STRING, 3, cstr);
	cstr = i18n(_T("Menu"), _T("LANGUAGE"));
	if(cstr.Find(_T("Language")) >= 0)
	{
		cstr = _T("&Language");
		menu->ModifyMenu(4, MF_BYPOSITION | MF_STRING, 4, cstr);
	}
	else
	{
		menu->ModifyMenu(4, MF_BYPOSITION | MF_STRING, 4, cstr + _T("(&Language)"));
	}

	cstr = i18n(_T("Menu"), _T("FILE_EXIT")) + _T("\tAlt + F4");
	menu->ModifyMenu(ID_FILE_EXIT, MF_STRING, ID_FILE_EXIT, cstr);
	cstr = i18n(_T("Menu"), _T("SAVE")) + _T("\tCtrl + S");
	menu->ModifyMenu(ID_RESULT_SAVE, MF_STRING, ID_RESULT_SAVE, cstr);

	cstr = i18n(_T("Menu"), _T("EDIT_COPY")) + _T("\tCtrl + C");
	menu->ModifyMenu(ID_EDIT_COPY, MF_STRING, ID_EDIT_COPY, cstr);
	cstr = i18n(_T("Menu"), _T("QUEUES_THREADS")) + _T("\tCtrl + Q");
	menu->ModifyMenu(ID_SETTINGS_QUEUESTHREADS, MF_STRING, ID_SETTINGS_QUEUESTHREADS, cstr);

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
	subMenu.Attach(menu->GetSubMenu(2)->GetSafeHmenu());
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
		subMenu.Attach(menu.GetSubMenu(4)->GetSafeHmenu()); // 5th is "Language".
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

void CDiskMarkDlg::ResultText(RESULT_TEXT type)
{
	CString cstr, clip;

	UpdateData(TRUE);

#ifdef PRO_MODE
	clip = _T("\
------------------------------------------------------------------------------\r\n\
%PRODUCT% %VERSION%%EDITION% (C) %COPY_YEAR% hiyohiyo\r\n\
                                  Crystal Dew World: https://crystalmark.info/\r\n\
------------------------------------------------------------------------------\r\n\
* MB/s = 1,000,000 bytes/s [SATA/600 = 600,000,000 bytes/s]\r\n\
* KB = 1000 bytes, KiB = 1024 bytes\r\n\
\r\n\
%SequentialRead1%\r\n\
%SequentialWrite1%\r\n\
%SequentialMix1%\r\n\
%SequentialRead2%\r\n\
%SequentialWrite2%\r\n\
%SequentialMix2%\r\n\
%RandomRead4KB1%\r\n\
%RandomWrite4KB1%\r\n\
%RandomMix4KB1%\r\n\
%RandomRead4KB2%\r\n\
%RandomWrite4KB2%\r\n\
%RandomMix4KB2%\r\n\
%RandomRead4KB3%\r\n\
%RandomWrite4KB3%\r\n\
%RandomMix4KB3%\r\n\
\r\n\
  Mix Ratio : %MixRatio%\r\n\
       Test : %TestSize% (x%TestCount%) %TestMode% [%IntervalTime%]\r\n\
       Date : %Date%\r\n\
         OS : %OS%\r\n\
  %Comment%\
");
#else
	clip = _T("\
------------------------------------------------------------------------------\r\n\
%PRODUCT% %VERSION%%EDITION% (C) %COPY_YEAR% hiyohiyo\r\n\
                                  Crystal Dew World: https://crystalmark.info/\r\n\
------------------------------------------------------------------------------\r\n\
* MB/s = 1,000,000 bytes/s [SATA/600 = 600,000,000 bytes/s]\r\n\
* KB = 1000 bytes, KiB = 1024 bytes\r\n\
\r\n\
%SequentialRead1%\r\n\
%SequentialWrite1%\r\n\
%SequentialRead2%\r\n\
%SequentialWrite2%\r\n\
%RandomRead4KB1%\r\n\
%RandomWrite4KB1%\r\n\
%RandomRead4KB2%\r\n\
%RandomWrite4KB2%\r\n\
%RandomRead4KB3%\r\n\
%RandomWrite4KB3%\r\n\
\r\n\
   Test: %TestSize% (x%TestCount%)%TestMode% [%IntervalTime%] %Affinity%\r\n\
   Date: %Date%\r\n\
     OS: %OS%\r\n\
%Comment%\
");
#endif

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

#ifdef PRO_MODE
	cstr.Format(_T("  Sequential Mix 128KiB (Q=%3d,T=%2d) : %9.3f MB/s"), m_SequentialMultiQueues1, m_SequentialMultiThreads1, m_SequentialMixScore1);
	clip.Replace(_T("%SequentialMix1%"), cstr);
	cstr.Format(_T("   Sequential Read (Q=%3d,T=%2d) : %9.3f MB/s"), m_SequentialMultiQueues2, m_SequentialMultiThreads2, m_SequentialReadScore2);
	clip.Replace(_T("%SequentialRead2%"), cstr);
	cstr.Format(_T("  Sequential Write (Q=%3d,T=%2d) : %9.3f MB/s"), m_SequentialMultiQueues2, m_SequentialMultiThreads2, m_SequentialWriteScore2);
	clip.Replace(_T("%SequentialWrite2%"), cstr);
	cstr.Format(_T("    Sequential Mix (Q=%3d,T=%2d) : %9.3f MB/s"), m_SequentialMultiQueues2, m_SequentialMultiThreads2, m_SequentialMixScore2);
	clip.Replace(_T("%SequentialMix2%"), cstr);
#endif

	double iops = 0.0;
	double latency = 0.0;

	cstr.Format(_T("   Sequential Read %dMiB (T=%2d): %8.3f MB/s"), m_SequentialMultiSize1, m_SequentialMultiThreads1, m_SequentialReadScore1);
	clip.Replace(_T("%SequentialRead1%"), cstr);
	cstr.Format(_T("  Sequential Write %dMiB (T=%2d): %8.3f MB/s"), m_SequentialMultiSize1, m_SequentialMultiThreads1, m_SequentialWriteScore1);
	clip.Replace(_T("%SequentialWrite1%"), cstr);

	iops = 0.0; latency = 0.0; iops = m_SequentialReadScore2 * 1000 * 1000 / (128 * 1024); if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T("  Seq Read 128KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_SequentialMultiQueues2, m_SequentialMultiThreads2, m_SequentialReadScore2, iops, latency);
	clip.Replace(_T("%SequentialRead2%"), cstr);
	iops = 0.0; latency = 0.0; iops = m_SequentialWriteScore2 * 1000 * 1000 / (128 * 1024); if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T(" Seq Write 128KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_SequentialMultiQueues2, m_SequentialMultiThreads2, m_SequentialWriteScore2, iops, latency);
	clip.Replace(_T("%SequentialWrite2%"), cstr);

	iops = 0.0; latency = 0.0; iops = m_RandomRead4KBScore1 * 1000 * 1000 / 4096; if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T(" Random Read 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues1, m_RandomMultiThreads1, m_RandomRead4KBScore1, iops, latency);
	clip.Replace(_T("%RandomRead4KB1%"), cstr);
	iops = 0.0; latency = 0.0; iops = m_RandomWrite4KBScore1 * 1000 * 1000 / 4096; if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T("Random Write 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues1, m_RandomMultiThreads1, m_RandomWrite4KBScore1, iops, latency);
	clip.Replace(_T("%RandomWrite4KB1%"), cstr);
	iops = 0.0; latency = 0.0; iops = m_RandomRead4KBScore2 * 1000 * 1000 / 4096; if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T(" Random Read 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues2, m_RandomMultiThreads2, m_RandomRead4KBScore2, iops, latency);
	clip.Replace(_T("%RandomRead4KB2%"), cstr);
	iops = 0.0; latency = 0.0; iops = m_RandomWrite4KBScore2 * 1000 * 1000 / 4096; if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T("Random Write 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues2, m_RandomMultiThreads2, m_RandomWrite4KBScore2, iops, latency);
	clip.Replace(_T("%RandomWrite4KB2%"), cstr);
	iops = 0.0; latency = 0.0; iops = m_RandomRead4KBScore3 * 1000 * 1000 / 4096;if (iops > 0.0){latency = 1.0 * 1000 * 1000 / iops;}
	cstr.Format(_T(" Random Read 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues3, m_RandomMultiThreads3, m_RandomRead4KBScore3, iops, latency);
	clip.Replace(_T("%RandomRead4KB3%"), cstr);
	iops = 0.0; latency = 0.0; iops = m_RandomWrite4KBScore3 * 1000 * 1000 / 4096; if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T("Random Write 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues3, m_RandomMultiThreads3, m_RandomWrite4KBScore3, iops, latency);
	clip.Replace(_T("%RandomWrite4KB3%"), cstr);


#ifdef PRO_MODE
	iops = 0.0; latency = 0.0; iops = m_RandomMix4KBScore1 * 1000 * 1000 / 4096; if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T("  Random Mix 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues1, m_RandomMultiThreads1, m_RandomMix4KBScore1, iops, latency);
	clip.Replace(_T("%RandomMix4KB1%"), cstr);
	iops = 0.0; latency = 0.0; iops = m_RandomMix4KBScore2 * 1000 * 1000 / 4096; if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T("  Random Mix 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues2, m_RandomMultiThreads2, m_RandomMix4KBScore2, iops, latency);
	clip.Replace(_T("%RandomMix4KB2%"), cstr);
	iops = 0.0; latency = 0.0; iops = m_RandomMix4KBScore3 * 1000 * 1000 / 4096; if (iops > 0.0) { latency = 1.0 * 1000 * 1000 / iops; }
	cstr.Format(_T("  Random Mix 4KiB (Q=%3d,T=%2d): %8.3f MB/s [%9.1f IOPS] <%9.2f us>"), m_RandomMultiQueues3, m_RandomMultiThreads3, m_RandomMix4KBScore3, iops, latency);
	clip.Replace(_T("%RandomMix4KB3%"), cstr);

	cstr.Format(_T("Read %d%%, Write %d%%"), 70, 30);
	clip.Replace(_T("%MixRatio%"), cstr);
#endif



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

	if (m_Affinity == AFFINITY_OFF)
	{
		cstr = L"<Affinity=OFF>";
	}
	else
	{
		cstr = L"<Affinity=ON>";
	}
	clip.Replace(_T("%Affinity%"), cstr);

	m_Comment.GetWindowText(cstr);
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
	SetWindowTitle(_T(""), _T(""));
}

void CDiskMarkDlg::OnModeAll0x00()
{
	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_MODE_DEFAULT, ID_MODE_ALL0X00, ID_MODE_ALL0X00, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_TestData = TEST_DATA_ALL0X00;
	WritePrivateProfileString(_T("Setting"), _T("TestData"), _T("1"), m_Ini);
	SetWindowTitle(_T(""), ALL_0X00_0FILL);
}


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

void CDiskMarkDlg::MoveForcus()
{
	GotoDlgCtrl(GetDlgItem(IDOK));
}

void CDiskMarkDlg::UpdateUnitLabel()
{
	if (m_IndexTestUnit == SCORE_UNIT::SCORE_IOPS)
	{
		m_ReadUnit.SetWindowTextW(L"Read [IOPS]");
		m_WriteUnit.SetWindowTextW(L"Write [IOPS]");
#ifdef PRO_MODE
		m_MixUnit.SetWindowTextW(L"Mix [IOPS]");
#endif
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_US)
	{
		m_ReadUnit.SetWindowTextW(L"Read [μs]");
		m_WriteUnit.SetWindowTextW(L"Write [μs]");
#ifdef PRO_MODE
		m_MixUnit.SetWindowTextW(L"Mix [μs]");
#endif
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_GBS)
	{
		m_ReadUnit.SetWindowTextW(L"Read [GB/s]");
		m_WriteUnit.SetWindowTextW(L"Write [GB/s]");
#ifdef PRO_MODE
		m_MixUnit.SetWindowTextW(L"Mix [GB/s]");
#endif
	}
	else
	{
		m_ReadUnit.SetWindowTextW(L"Read [MB/s]");
		m_WriteUnit.SetWindowTextW(L"Write [MB/s]");
#ifdef PRO_MODE
		m_MixUnit.SetWindowTextW(L"Mix [MB/s]");
#endif
	}
}

void CDiskMarkDlg::SetWindowTitle(CString message, CString mode)
{
	CString title;

	if (!message.IsEmpty())
	{
		title.Format(_T("%s - %s"), PRODUCT_SHORT_NAME, message.GetString());
	}
	else if (!mode.IsEmpty())
	{
		title.Format(_T("%s %s %s %s"), PRODUCT_NAME, PRODUCT_VERSION, PRODUCT_EDITION, mode.GetString());
	}
	else
	{
		title.Format(_T("%s %s %s"), PRODUCT_NAME, PRODUCT_VERSION, PRODUCT_EDITION);
	}

	if (m_AdminMode)
	{
		title += L" [ADMIN]";
	}

	SetWindowText(title);
}

HBRUSH CDiskMarkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogCx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID()) {
	case IDC_COMMENT:
		pDC->SetTextColor(m_EditText);
		pDC->SetBkColor(m_EditBg);
		hbr = (HBRUSH)m_EditBrush->GetSafeHandle();
		break;
	default:
		break;
	}
	return hbr;
}