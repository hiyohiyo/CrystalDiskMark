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

#include <math.h>

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
	: CMainDialogFx(CDiskMarkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconMini = AfxGetApp()->LoadIcon(IDI_TRAY_ICON);

	m_AboutDlg = NULL;
	m_SettingsDlg = NULL;

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_DefaultTheme = L"Shizuku";
	m_RecommendTheme = L"Shizuku";
	m_ThemeKeyName = L"ThemeShizuku";
#elif KUREI_KEI_SUPPORT
	m_DefaultTheme = L"KureiKei";
	m_RecommendTheme = L"KureiKeiRecoding";
	m_ThemeKeyName = L"ThemeKureiKei";
#else
	m_DefaultTheme = L"Default";
	m_ThemeKeyName = L"Theme";
#endif

	m_BackgroundName = L"Background";

	m_AdminMode = IsUserAnAdmin();
}

CDiskMarkDlg::~CDiskMarkDlg()
{
}

void CDiskMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CMainDialogFx::DoDataExchange(pDX);

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

	DDX_Control(pDX, IDC_COMBO_COUNT, m_ComboCount);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_ComboSize);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_ComboDrive);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_ComboUnit);
	DDX_Control(pDX, IDC_COMBO_MODE, m_ComboMode);

	DDX_Control(pDX, IDC_READ_UNIT, m_ReadUnit);
	DDX_Control(pDX, IDC_WRITE_UNIT, m_WriteUnit);

#ifdef MIX_MODE
	DDX_Control(pDX, IDC_MIX_UNIT, m_MixUnit);
#endif

	DDX_Text(pDX, IDC_COMBO_COUNT, m_ValueTestCount);
	DDX_Text(pDX, IDC_COMBO_SIZE, m_ValueTestSize);
	DDX_Text(pDX, IDC_COMBO_DRIVE, m_ValueTestDrive);
	DDX_Text(pDX, IDC_COMBO_UNIT, m_ValueTestUnit);
	DDX_Text(pDX, IDC_COMBO_MODE, m_ValueTestMode);
	DDX_CBIndex(pDX, IDC_COMBO_COUNT, m_IndexTestCount);
	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_IndexTestSize);
	DDX_CBIndex(pDX, IDC_COMBO_DRIVE, m_IndexTestDrive);
	DDX_CBIndex(pDX, IDC_COMBO_UNIT, m_IndexTestUnit);
	DDX_CBIndex(pDX, IDC_COMBO_MODE, m_IndexTestMode);
#ifdef MIX_MODE
	DDX_CBIndex(pDX, IDC_COMBO_MIX, m_IndexTestMix);
#endif
}

BEGIN_MESSAGE_MAP(CDiskMarkDlg, CMainDialogFx)
	//}}AFX_MSG_MAP
#ifdef SUISHO_SHIZUKU_SUPPORT
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
#endif
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_COPY, OnCopy)
	ON_MESSAGE(WM_UPDATE_SCORE, OnUpdateScore)
	ON_MESSAGE(WM_UPDATE_MESSAGE, OnUpdateMessage)
	ON_MESSAGE(WM_EXIT_BENCHMARK, OnExitBenchmark)

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
	ON_COMMAND(ID_CRYSTALDEWWORLD, &CDiskMarkDlg::OnCrystalDewWorld)
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
	ON_COMMAND(ID_SAVE_TEXT, &CDiskMarkDlg::OnSaveText)
	ON_COMMAND(ID_SAVE_IMAGE, &CDiskMarkDlg::OnSaveImage)
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
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CDiskMarkDlg::OnCbnSelchangeComboMode)

	ON_CBN_CLOSEUP(IDC_COMBO_COUNT, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_SIZE, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_DRIVE, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_UNIT, &CDiskMarkDlg::MoveForcus)
	ON_CBN_CLOSEUP(IDC_COMBO_MODE, &CDiskMarkDlg::MoveForcus)
#ifdef MIX_MODE
	ON_CBN_SELCHANGE(IDC_COMBO_MIX, &CDiskMarkDlg::OnCbnSelchangeComboMix)
	ON_CBN_CLOSEUP(IDC_COMBO_MIX, &CDiskMarkDlg::MoveForcus)
#endif

END_MESSAGE_MAP()

LRESULT CDiskMarkDlg::OnQueryEndSession(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

BOOL CDiskMarkDlg::CheckThemeEdition(CString name)
{
#ifdef SUISHO_SHIZUKU_SUPPORT
	if(name.Find(L"Shizuku") == 0) { return TRUE; }
#elif KUREI_KEI_SUPPORT
	if(name.Find(L"KureiKei") == 0) { return TRUE; }
#else
	if(name.Find(L"Shizuku") != 0 && name.Find(L"KureiKei") != 0 && name.Find(L".") != 0) { return TRUE; }
#endif

	return FALSE;
}

int CALLBACK EnumFontFamExProcDefaultFont(ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, int FontType, LPARAM lParam)
{
	if (_tcscmp(lpelfe->elfLogFont.lfFaceName, DEFAULT_FONT_FACE_1) == 0)
	{
		*((BOOL*)lParam) = TRUE;
	}
	return TRUE;
}

BOOL CDiskMarkDlg::IsNormMode()
{
	if (m_SequentialSize1 == 1024
	&& m_SequentialQueues1 == 8
	&& m_SequentialThreads1 == 1
	&& m_SequentialSize2 == 1024
	&& m_SequentialQueues2 == 1
	&& m_SequentialThreads2 == 1
	&& m_RandomSize1 == 4
	&& m_RandomQueues1 == 32
	&& m_RandomThreads1 == 1
	&& m_RandomSize2 == 4
	&& m_RandomQueues2 == 1
	&& m_RandomThreads2 == 1
	&& m_Affinity == 0
	)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDiskMarkDlg::IsNVMeMode()
{
	if (m_SequentialSize1 == 1024
	&& m_SequentialQueues1 == 8
	&& m_SequentialThreads1 == 1
	&& m_SequentialSize2 == 1024
	&& m_SequentialQueues2 == 1
	&& m_SequentialThreads2 == 1
	&& m_RandomSize1 == 4
	&& m_RandomQueues1 == 32
	&& m_RandomThreads1 == 16
	&& m_RandomSize2 == 4
	&& m_RandomQueues2 == 1
	&& m_RandomThreads2 == 1
	&& m_Affinity == 0
	)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDiskMarkDlg::IsAtaMode()
{
	if (m_SequentialSize1 == 128
		&& m_SequentialQueues1 == 32
		&& m_SequentialThreads1 == 1
		&& m_SequentialSize2 == 128
		&& m_SequentialQueues2 == 1
		&& m_SequentialThreads2 == 1
		&& m_RandomSize1 == 4
		&& m_RandomQueues1 == 32
		&& m_RandomThreads1 == 1
		&& m_RandomSize2 == 4
		&& m_RandomQueues2 == 1
		&& m_RandomThreads2 == 1
		&& m_Affinity == 0
		)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDiskMarkDlg::OnInitDialog()
{
	CMainDialogFx::OnInitDialog();

	m_hAccelerator = ::LoadAccelerators(AfxGetInstanceHandle(),
		                                MAKEINTRESOURCE(IDR_ACCELERATOR));

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIconMini, FALSE);

	TCHAR str[256];

	CClientDC dc(this);
	LOGFONT logfont;
	CString defaultFontFace;
	BOOL hasDefaultFont = FALSE;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = DEFAULT_CHARSET;
	::EnumFontFamiliesExW(dc.m_hDC, &logfont, (FONTENUMPROC)EnumFontFamExProcDefaultFont, (INT_PTR)(&hasDefaultFont), 0);

	if (hasDefaultFont)
	{
		defaultFontFace = DEFAULT_FONT_FACE_1;
	}
	else
	{
		defaultFontFace = DEFAULT_FONT_FACE_2;
	}

	GetPrivateProfileString(L"Setting", L"FontFace", defaultFontFace, str, 256, m_Ini);
	m_FontFace = str;

	m_TestData = GetPrivateProfileInt(L"Setting", L"TestData", TEST_DATA_RANDOM, m_Ini);
	if (m_TestData != TEST_DATA_ALL0X00)
	{
		m_TestData = TEST_DATA_RANDOM;
	}

	m_Profile = GetPrivateProfileInt(L"Setting", L"Profile", PROFILE_DEFAULT, m_Ini);
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

	m_FontScale = GetPrivateProfileInt(L"Setting", L"FontScale", 100, m_Ini);
	if (m_FontScale > 200 || m_FontScale < 50)
	{
		m_FontScale = 100;
		m_FontRatio = 1.0;
	}
	else
	{
		m_FontRatio = m_FontScale / 100.0;
	}

	m_Affinity = GetPrivateProfileInt(L"Setting", L"Affinity", AFFINITY_DISABLED, m_Ini);
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

	m_IndexTestCount = GetPrivateProfileInt(L"Setting", L"TestCount", 4, m_Ini);
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
	m_IndexTestSize = GetPrivateProfileInt(L"Setting", L"TestSize", 6, m_Ini);
	if (m_IndexTestSize < 0 || m_IndexTestSize > 13)
	{
		m_IndexTestSize = 6;	// default value is 1GiB;
	}
	m_ComboSize.SetCurSel(m_IndexTestSize);

	m_IndexTestUnit = GetPrivateProfileInt(L"Setting", L"TestUnit", 0, m_Ini);
	if (m_IndexTestUnit < 0 || m_IndexTestUnit >= 3)
	{
		m_IndexTestUnit = 0;
	}
	m_ComboUnit.SetCurSel(m_IndexTestUnit);

	m_IndexTestUnit = GetPrivateProfileInt(L"Setting", L"TestMode", 0, m_Ini);
	if (m_IndexTestMode < 0 || m_IndexTestMode >= 3)
	{
		m_IndexTestMode = 0;
	}
	m_ComboUnit.SetCurSel(m_IndexTestMode);

	// Mode
	m_ComboMode.AddString(L"NORM");
	m_ComboMode.AddString(L"NVMe");
	m_ComboMode.AddString(L"ATA");
	m_ComboMode.AddString(L"EDIT");

	if (IsNormMode())
	{
		m_IndexTestMode = 0;
	}
	else if (IsNVMeMode())
	{
		m_IndexTestMode = 1;
	}
	else if (IsAtaMode())
	{
		m_IndexTestMode = 2;
	}
	else
	{
		m_IndexTestMode = 3;
	}
	m_ComboMode.SetCurSel(m_IndexTestMode);

	m_IndexTestMix = GetPrivateProfileInt(L"Setting", L"TestMix", 6, m_Ini);
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
	
	switch(GetPrivateProfileInt(L"Setting", L"ZoomType", 0, m_Ini))
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

	SetClientSize((int)(m_SizeX * m_ZoomRatio), (int)(m_SizeY * m_ZoomRatio));

	m_bShowWindow = TRUE;

//	ChangeButtonStatus(TRUE);
	CenterWindow();

	UpdateDialogSize();
	ChangeButtonStatus(TRUE);

	m_bInitializing = FALSE;

	SetForegroundWindow();

	return TRUE;
}

void CDiskMarkDlg::UpdateDialogSize()
{
	CDialogFx::UpdateDialogSize();

	ShowWindow(SW_HIDE);
	int offsetX = 0;
	int comboDriveX = 0;

	m_SizeX = SIZE_X;
	m_SizeY = SIZE_Y;
#ifdef MIX_MODE
	if (m_MixMode)
	{
		m_SizeX = SIZE_X_MIX;
	}
#endif
	SetClientSize((int)(m_SizeX * m_ZoomRatio), (int)(m_SizeY * m_ZoomRatio), 1);

#ifdef SUISHO_SHIZUKU_SUPPORT
	if (m_CharacterPosition == 0)
	{
		offsetX = OFFSET_X;
	}
#endif

	UpdateBackground(TRUE, m_bDarkMode);
	SetControlFont();

	if (m_Profile != PROFILE_DEFAULT && m_Profile != PROFILE_DEFAULT_MIX)
	{
#ifdef SUISHO_SHIZUKU_SUPPORT
		comboDriveX = 120;
#else
		comboDriveX = 72;
#endif
	}
	else
	{
		comboDriveX = 0;
	}

	m_TestRead1.SetDrawFrame(m_bHighContrast);
	m_TestRead2.SetDrawFrame(m_bHighContrast);
	m_TestRead3.SetDrawFrame(m_bHighContrast);
	m_TestRead4.SetDrawFrame(m_bHighContrast);
	m_TestWrite1.SetDrawFrame(m_bHighContrast);
	m_TestWrite2.SetDrawFrame(m_bHighContrast);
	m_TestWrite3.SetDrawFrame(m_bHighContrast);
	m_TestWrite4.SetDrawFrame(m_bHighContrast);
	m_Comment.SetDrawFrame(m_bHighContrast);

#ifdef MIX_MODE
	m_TestMix1.SetDrawFrame(m_bHighContrast);
	m_TestMix2.SetDrawFrame(m_bHighContrast);
	m_TestMix3.SetDrawFrame(m_bHighContrast);
	m_TestMix4.SetDrawFrame(m_bHighContrast);
#endif

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_ButtonAll.InitControl(  12 + offsetX,   8, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ButtonTest1.InitControl(12 + offsetX,  96, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ButtonTest2.InitControl(12 + offsetX, 184, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ButtonTest3.InitControl(12 + offsetX, 272, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ButtonTest4.InitControl(12 + offsetX, 360, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);

	m_ButtonAll.SetHandCursor(TRUE);
	m_ButtonTest1.SetHandCursor(TRUE);
	m_ButtonTest2.SetHandCursor(TRUE);
	m_ButtonTest3.SetHandCursor(TRUE);
	m_ButtonTest4.SetHandCursor(TRUE);

	m_TestRead1.InitControl(140 + offsetX,  96, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestRead2.InitControl(140 + offsetX, 184, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestRead3.InitControl(140 + offsetX, 272, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestRead4.InitControl(140 + offsetX, 360, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);

	m_TestWrite1.InitControl(468 + offsetX,  96, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestWrite2.InitControl(468 + offsetX, 184, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestWrite3.InitControl(468 + offsetX, 272, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestWrite4.InitControl(468 + offsetX, 360, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);

	m_Comment.InitControl(12 + offsetX, 452, 776, 40, m_ZoomRatio, &m_BkDC, IP(L"Comment"), 1, ES_LEFT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_Comment.SetMargin(0, 4, 0, 4, m_ZoomRatio);
	m_Comment.Adjust();

	m_ReadUnit.InitControl( 140 + offsetX, 52, 200, 40, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE);
	m_WriteUnit.InitControl(468 + offsetX, 52, 200, 40, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE);
	m_ComboUnit.InitControl(672 + offsetX, 52, 116, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);

	m_ComboCount.InitControl(140 + offsetX, 8, 60, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboSize.InitControl( 204 + offsetX, 8, 140, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboDrive.InitControl(348 + offsetX, 8, 320, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboMode.InitControl(672 + offsetX, 8, 116, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);

#else

	m_ButtonAll.InitControl(  8 + offsetX, 8, 72, 48, m_ZoomRatio,   &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ButtonTest1.InitControl(8 + offsetX, 60, 72, 48, m_ZoomRatio,  &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ButtonTest2.InitControl(8 + offsetX, 112, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ButtonTest3.InitControl(8 + offsetX, 164, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ButtonTest4.InitControl(8 + offsetX, 216, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);

	m_ButtonAll.SetHandCursor(TRUE);
	m_ButtonTest1.SetHandCursor(TRUE);
	m_ButtonTest2.SetHandCursor(TRUE);
	m_ButtonTest3.SetHandCursor(TRUE);
	m_ButtonTest4.SetHandCursor(TRUE);

	m_TestRead1.InitControl(84 + offsetX, 60, 192, 48, m_ZoomRatio,  &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestRead2.InitControl(84 + offsetX, 112, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestRead3.InitControl(84 + offsetX, 164, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestRead4.InitControl(84 + offsetX, 216, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);

	m_TestWrite1.InitControl(280 + offsetX, 60, 192, 48, m_ZoomRatio,  &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestWrite2.InitControl(280 + offsetX, 112, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestWrite3.InitControl(280 + offsetX, 164, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestWrite4.InitControl(280 + offsetX, 216, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	if (m_MixMode)
	{
		m_Comment.InitControl(8 + offsetX, 268, 664, 24, m_ZoomRatio, &m_BkDC, IP(L"CommentL"), 1, ES_LEFT, OwnerDrawImage, m_bHighContrast, FALSE);
	}
	else
	{
		m_Comment.InitControl(8 + offsetX, 268, 464, 24, m_ZoomRatio, &m_BkDC, IP(L"Comment"), 1, ES_LEFT, OwnerDrawImage, m_bHighContrast, FALSE);
	}
	m_Comment.SetMargin(0, 4, 0, 4, m_ZoomRatio);
	m_Comment.Adjust();

	m_ReadUnit.InitControl(84 + offsetX, 36, 124, 24, m_ZoomRatio,   &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE);
	m_WriteUnit.InitControl(280 + offsetX, 36, 124, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE);
	m_ComboUnit.InitControl(404 + offsetX, 34, 68, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);

	m_ComboCount.InitControl(84 + offsetX, 8, 40, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboSize.InitControl(128 + offsetX, 8, 80, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboDrive.InitControl(212 + offsetX, 8, 188, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboMode.InitControl(404 + offsetX, 8, 68, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
#endif

	m_TestRead1.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestRead2.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestRead3.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestRead4.SetMargin(0, 0, 0, 4, m_ZoomRatio);

	m_TestWrite1.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestWrite2.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestWrite3.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestWrite4.SetMargin(0, 0, 0, 4, m_ZoomRatio);

	m_ComboCount.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboSize.SetMargin (0, 4, 0, 0, m_ZoomRatio);
	m_ComboDrive.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboUnit.SetMargin (0, 4, 0, 0, m_ZoomRatio);
	m_ComboMode.SetMargin(0, 4, 0, 0, m_ZoomRatio);

#ifdef MIX_MODE
	m_TestMix1.InitControl(480 + offsetX, 60, 192, 48, m_ZoomRatio,  &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestMix2.InitControl(480 + offsetX, 112, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestMix3.InitControl(480 + offsetX, 164, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestMix4.InitControl(480 + offsetX, 216, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ComboMix.InitControl(480 + offsetX, 8, 192, 300, m_ZoomRatio,  &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_MixUnit.InitControl(480 + offsetX, 36, 124, 24, m_ZoomRatio,   &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, FALSE);

	m_TestMix1.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestMix2.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestMix3.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_TestMix4.SetMargin(0, 0, 0, 4, m_ZoomRatio);
	m_ComboMix.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	if (m_MixMode)
	{
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

	m_Comment.Adjust();

	UpdateScore();

	Invalidate();

	m_ComboCount.ShowWindow(SW_HIDE);
	m_ComboSize.ShowWindow(SW_HIDE);
	m_ComboDrive.ShowWindow(SW_HIDE);
	m_ComboUnit.ShowWindow(SW_HIDE);
	m_ComboMode.ShowWindow(SW_HIDE);

	COMBOBOXINFO info = { 0 };
	info.cbSize = sizeof(COMBOBOXINFO);
	m_ComboCount.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);
	m_ComboSize.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);
	m_ComboDrive.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);
	m_ComboUnit.GetComboBoxInfo(&info);
	SetLayeredWindow(info.hwndList, m_ComboAlpha);
	m_ComboMode.GetComboBoxInfo(&info);
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
	m_ComboMode.ShowWindow(SW_SHOW);

	if (m_Profile != PROFILE_DEFAULT && m_Profile != PROFILE_DEFAULT_MIX)
	{
		m_ComboUnit.ShowWindow(SW_HIDE);
	}
	else
	{
		m_ComboUnit.ShowWindow(SW_SHOW);
	}

	UpdateComboTooltip();

	ShowWindow(SW_SHOW);
}

void CDiskMarkDlg::UpdateComboTooltip()
{
	TCHAR str[256];
	GetPrivateProfileString(L"Title", L"TEST_COUNT", L"Test Count", str, 256, m_CurrentLangPath);
	m_ComboCount.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_SIZE", L"Test Size", str, 256, m_CurrentLangPath);
	m_ComboSize.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_UNIT", L"Test Unit", str, 256, m_CurrentLangPath);
	m_ComboUnit.SetToolTipText(str);
	GetPrivateProfileString(L"Title", L"TEST_MODE", L"Test Mode", str, 256, m_CurrentLangPath);
	m_ComboMode.SetToolTipText(str);
#ifdef MIX_MODE
	if (m_MixMode)
	{
		GetPrivateProfileString(L"Title", L"TEST_MIX", L"Mix Ratio", str, 256, m_CurrentLangPath);
		m_ComboMix.SetToolTipText(str);
	}
#endif
	m_ComboDrive.SetToolTipText(m_ComboDrive.GetToolTipText().GetString());
}

void CDiskMarkDlg::SetLayeredWindow(HWND hWnd, BYTE alpha)
{
	if (IsWin2k()) { return; }

	::SetWindowLong(hWnd, GWL_EXSTYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	::SetWindowLong(hWnd, GWL_EXSTYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	if (m_bHighContrast)
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
#ifdef SUISHO_SHIZUKU_SUPPORT
	BYTE textAlpha = 255;
	COLORREF textColor = RGB(0, 0, 0);
#else
	BYTE textAlpha = 255;
	COLORREF textColor = RGB(0, 0, 0);
#endif

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_ButtonAll.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_ButtonTest1.SetFontEx(m_FontFace, 20, 20, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_ButtonTest2.SetFontEx(m_FontFace, 20, 20, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_ButtonTest3.SetFontEx(m_FontFace, 20, 20, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_ButtonTest4.SetFontEx(m_FontFace, 20, 20, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);

	m_TestRead1.SetFontEx(m_FontFace, 56, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestRead2.SetFontEx(m_FontFace, 56, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestRead3.SetFontEx(m_FontFace, 56, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestRead4.SetFontEx(m_FontFace, 56, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);

	m_TestWrite1.SetFontEx(m_FontFace, 56, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestWrite2.SetFontEx(m_FontFace, 56, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestWrite3.SetFontEx(m_FontFace, 56, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestWrite4.SetFontEx(m_FontFace, 56, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);

	m_Comment.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_EditText, FW_BOLD);

	m_ReadUnit.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD);
	m_WriteUnit.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD);

	m_ComboCount.SetFontEx(m_FontFace, 24, 24, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_ComboSize.SetFontEx(m_FontFace, 24, 24, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_ComboDrive.SetFontEx(m_FontFace, 24, 24, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_ComboUnit.SetFontEx(m_FontFace, 24, 24, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_ComboMode.SetFontEx(m_FontFace, 24, 24, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);

	m_ButtonTest1.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonTest2.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonTest3.SetMargin(8, 0, 8, 0, m_ZoomRatio);
	m_ButtonTest4.SetMargin(8, 0, 8, 0, m_ZoomRatio);

	m_ComboCount.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
	m_ComboSize.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
	m_ComboDrive.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
	m_ComboUnit.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
	m_ComboMode.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
#else
	m_ButtonAll.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_ButtonTest1.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_ButtonTest2.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_ButtonTest3.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);
	m_ButtonTest4.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD);

	m_TestRead1.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestRead2.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestRead3.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestRead4.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);

	m_TestWrite1.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestWrite2.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestWrite3.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	m_TestWrite4.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);

	m_Comment.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_EditText, FW_BOLD);

	m_ReadUnit.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD);
	m_WriteUnit.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD);

	m_ComboCount.SetFontEx(m_FontFace, 14, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_ComboSize.SetFontEx(m_FontFace, 14, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_ComboDrive.SetFontEx(m_FontFace, 14, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_ComboUnit.SetFontEx(m_FontFace, 14, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
	m_ComboMode.SetFontEx(m_FontFace, 14, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);

	m_ButtonTest3.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonTest1.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonTest2.SetMargin(4, 0, 4, 0, m_ZoomRatio);
	m_ButtonTest4.SetMargin(4, 0, 4, 0, m_ZoomRatio);

	m_ComboCount.SetItemHeightAll(22, m_ZoomRatio, m_FontRatio);
	m_ComboSize.SetItemHeightAll(22, m_ZoomRatio, m_FontRatio);
	m_ComboDrive.SetItemHeightAll(22, m_ZoomRatio, m_FontRatio);
	m_ComboUnit.SetItemHeightAll(22, m_ZoomRatio, m_FontRatio);
	m_ComboMode.SetItemHeightAll(22, m_ZoomRatio, m_FontRatio);

#ifdef MIX_MODE
	if(m_MixMode)
	{
		m_ComboMix.SetFontEx(m_FontFace, 14, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL);
		m_ComboMix.SetItemHeightAll(24, m_ZoomRatio, m_FontRatio);

		m_MixUnit.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD);

		m_TestMix1.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
		m_TestMix2.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
		m_TestMix3.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
		m_TestMix4.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD);
	}
#endif

#endif

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

	m_SequentialSize1 = GetPrivateProfileInt(L"Setting", L"SequentialSize1", 1024, m_Ini);
	if (m_SequentialSize1 <= 0 || m_SequentialSize1 > 8192)
	{
		m_SequentialSize1 = 1024;
	}
	m_SequentialQueues1 = GetPrivateProfileInt(L"Setting", L"SequentialQueues1", 8, m_Ini);
	if (m_SequentialQueues1 <= 0 || m_SequentialQueues1 > MAX_QUEUES)
	{
		m_SequentialQueues1 = 8;
	}
	m_SequentialThreads1 = GetPrivateProfileInt(L"Setting", L"SequentialThreads1", 1, m_Ini);
	if (m_SequentialThreads1 <= 0 || m_SequentialThreads1 > MAX_THREADS)
	{
		m_SequentialThreads1 = 1;
	}

	m_SequentialSize2 = GetPrivateProfileInt(L"Setting", L"SequentialSize2", 1024, m_Ini);
	if (m_SequentialSize2 <= 0 || m_SequentialSize2 > 8192)
	{
		m_SequentialSize2 = 1024;
	}
	m_SequentialQueues2 = GetPrivateProfileInt(L"Setting", L"SequentialQueues2", 1, m_Ini);
	if (m_SequentialQueues2 <= 0 || m_SequentialQueues2 > MAX_QUEUES)
	{
		m_SequentialQueues2 = 1;
	}
	m_SequentialThreads2 = GetPrivateProfileInt(L"Setting", L"SequentialThreads2", 1, m_Ini);
	if (m_SequentialThreads2 <= 0 || m_SequentialThreads2 > MAX_THREADS)
	{
		m_SequentialThreads2 = 1;
	}
	
	m_RandomSize1 = GetPrivateProfileInt(L"Setting", L"RandomSize1", 4, m_Ini);
	if (m_RandomSize1 < 4 || m_RandomSize1 > 8192)
	{
		m_RandomSize1 = 4;
	}
	m_RandomQueues1 = GetPrivateProfileInt(L"Setting", L"RandomQueues1", 32, m_Ini);
	if (m_RandomQueues1 <= 0 || m_RandomQueues1 > MAX_QUEUES)
	{
		m_RandomQueues1 = 32;
	}
	m_RandomThreads1 = GetPrivateProfileInt(L"Setting", L"RandomThreads1", 16, m_Ini);
	if (m_RandomThreads1 <= 0 || m_RandomThreads1 > MAX_THREADS)
	{
		m_RandomThreads1 = 16;
	}

	m_RandomSize2 = GetPrivateProfileInt(L"Setting", L"RandomSize2", 4, m_Ini);
	if (m_RandomSize2 < 4 || m_RandomSize2 > 8192)
	{
		m_RandomSize2 = 4;
	}
	m_RandomQueues2 = GetPrivateProfileInt(L"Setting", L"RandomQueues2", 1, m_Ini);
	if (m_RandomQueues2 <= 0 || m_RandomQueues2 > MAX_QUEUES)
	{
		m_RandomQueues2 = 1;
	}
	m_RandomThreads2 = GetPrivateProfileInt(L"Setting", L"RandomThreads2", 1, m_Ini);
	if (m_RandomThreads2 <= 0 || m_RandomThreads2 > MAX_THREADS)
	{
		m_RandomThreads2 = 1;
	}

	m_Affinity = GetPrivateProfileInt(L"Setting", L"Affinity", 0, m_Ini);
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
					WritePrivateProfileString(L"Setting", L"TargetPath", szTmp, m_Ini);
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
		CMainDialogFx::OnPaint();
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
	m_AboutDlg->Create(CAboutDlg::IDD, m_AboutDlg, ID_ABOUT, this);
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
	cstr.Format(L"%d", m_IndexTestUnit);
	WritePrivateProfileString(L"Setting", L"TestUnit", cstr, m_Ini);
	cstr.Format(L"%d", m_IndexTestCount);
	WritePrivateProfileString(L"Setting", L"TestCount", cstr, m_Ini);
	cstr.Format(L"%d", m_IndexTestSize);
	WritePrivateProfileString(L"Setting", L"TestSize", cstr, m_Ini);
#ifdef MIX_MODE
	if (m_MixMode)
	{
		cstr.Format(L"%d", m_IndexTestMix);
		WritePrivateProfileString(L"Setting", L"TestMix", cstr, m_Ini);
	}
#endif

	if(m_IndexTestDrive != m_MaxIndexTestDrive)
	{ 
		cstr.Format(L"%d", m_ValueTestDrive.GetAt(0) - 'A');
	}
	else
	{
		cstr.Format(L"%d", 99);
	}
	WritePrivateProfileString(L"Setting", L"DriveLetter", cstr, m_Ini);

	CMainDialogFx::OnCancel();
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
		SetMeter(&m_TestRead1, m_SequentialReadScore1, m_SequentialReadLatency1, m_SequentialSize1 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead2, m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead3, m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1 * 1024, SCORE_IOPS);
		SetMeter(&m_TestRead4, m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1 * 1024, SCORE_US);
		SetMeter(&m_TestWrite1, m_SequentialWriteScore1, m_SequentialWriteLatency1, m_SequentialSize1 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite2, m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite3, m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1 * 1024, SCORE_IOPS);
		SetMeter(&m_TestWrite4, m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1 * 1024, SCORE_US);

#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix1, m_SequentialMixScore1, m_SequentialMixLatency1, m_SequentialSize1 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix2, m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix3, m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1 * 1024, SCORE_IOPS);
			SetMeter(&m_TestMix4, m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1 * 1024, SCORE_US);
		}
#endif
	}
	else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		SetMeter(&m_TestRead1, m_SequentialReadScore1, m_SequentialReadLatency1, 1 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead2, m_RandomReadScore1, m_RandomReadLatency1, 4 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead3, m_RandomReadScore1, m_RandomReadLatency1, 4 * 1024, SCORE_IOPS);
		SetMeter(&m_TestRead4, m_RandomReadScore1, m_RandomReadLatency1, 4 * 1024, SCORE_US);
		SetMeter(&m_TestWrite1, m_SequentialWriteScore1, m_SequentialWriteLatency1, 1 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite2, m_RandomWriteScore1, m_RandomWriteLatency1, 4 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite3, m_RandomWriteScore1, m_RandomWriteLatency1, 4 * 1024, SCORE_IOPS);
		SetMeter(&m_TestWrite4, m_RandomWriteScore1, m_RandomWriteLatency1, 4 * 1024, SCORE_US);

#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix1, m_SequentialMixScore1, m_SequentialMixLatency1, 1 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix2, m_RandomMixScore1, m_RandomMixLatency1, 4 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix3, m_RandomMixScore1, m_RandomMixLatency1, 4 * 1024, SCORE_IOPS);
			SetMeter(&m_TestMix4, m_RandomMixScore1, m_RandomMixLatency1, 4 * 1024, SCORE_US);
		}
#endif
	}
	else
	{
		SetMeter(&m_TestRead1, m_SequentialReadScore1, m_SequentialReadLatency1, m_SequentialSize1 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead2, m_SequentialReadScore2, m_SequentialReadLatency2, m_SequentialSize2 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead3, m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead4, m_RandomReadScore2, m_RandomReadLatency2, m_RandomSize2 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite1, m_SequentialWriteScore1, m_SequentialWriteLatency1, m_SequentialSize1 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite2, m_SequentialWriteScore2, m_SequentialWriteLatency2, m_SequentialSize2 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite3, m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1 * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite4, m_RandomWriteScore2, m_RandomWriteLatency2, m_RandomSize2 * 1024, m_IndexTestUnit);

#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix1, m_SequentialMixScore1, m_SequentialMixLatency1, m_SequentialSize1 * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix2, m_SequentialMixScore2, m_SequentialMixLatency2, m_SequentialSize2 * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix3, m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1 * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix4, m_RandomMixScore2, m_RandomMixLatency2, m_RandomSize2 * 1024, m_IndexTestUnit);
		}
#endif
	}

}

void CDiskMarkDlg::SetScoreToolTip(CStaticFx* control, double score, double latency, int blockSize)
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
		m_ComboCount.EnableWindow(TRUE);
		m_ComboSize.EnableWindow(TRUE);
		m_ComboDrive.EnableWindow(TRUE);
		m_ComboUnit.EnableWindow(TRUE);
		m_ComboMode.EnableWindow(TRUE);

		m_ButtonAll.SetWindowTextW(L"All");


		if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
		{
			if (m_SequentialSize1 >= 1024)
			{
				title.Format(L"SEQ%dM\r\nQ%dT%d", m_SequentialSize1 / 1024, m_SequentialQueues1, m_SequentialThreads1);
			}
			else
			{
				title.Format(L"SEQ%dK\r\nQ%dT%d", m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1);
			}
			m_ButtonTest1.SetWindowTextW(title);
			if (m_RandomSize1 >= 1024)
			{
				title.Format(L"RND%dK\r\nQ%dT%d", m_RandomSize1 / 1024, m_RandomQueues1, m_RandomThreads1);
			}
			else
			{
				title.Format(L"RND%dK\r\nQ%dT%d", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			}
			m_ButtonTest2.SetWindowTextW(title);
			title.Format(L"RND%dK\r\n(IOPS)", m_RandomSize1);
			m_ButtonTest3.SetWindowTextW(title);
			title.Format(L"RND%dK\r\n(μs)", m_RandomSize1);
			m_ButtonTest4.SetWindowTextW(title);
			if (m_SequentialSize1 >= 1024)
			{
				toolTip.Format(L"Sequential %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_SequentialSize1 / 1024, m_SequentialQueues1, m_SequentialThreads1);
			}
			else
			{
				toolTip.Format(L"Sequential %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1);
			}
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
			title.Format(L"SEQ%dM\r\nQ%dT%d", 1024 / 1024, 1, 1);
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
			if (m_SequentialSize1 >= 1024)
			{
				title.Format(L"SEQ%dM\r\nQ%dT%d", m_SequentialSize1 / 1024, m_SequentialQueues1, m_SequentialThreads1);
			}
			else
			{
				title.Format(L"SEQ%dK\r\nQ%dT%d", m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1);
			}
			m_ButtonTest1.SetWindowTextW(title);
			if (m_SequentialSize2 >= 1024)
			{
				title.Format(L"SEQ%dM\r\nQ%dT%d", m_SequentialSize2 / 1024, m_SequentialQueues2, m_SequentialThreads2);
			}
			else
			{
				title.Format(L"SEQ%dK\r\nQ%dT%d", m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2);
			}
			m_ButtonTest2.SetWindowTextW(title);
			if (m_RandomSize1 >= 1024)
			{
				title.Format(L"RND%dM\r\nQ%dT%d", m_RandomSize1 / 1024, m_RandomQueues1, m_RandomThreads1);
			}
			else
			{
				title.Format(L"RND%dK\r\nQ%dT%d", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			}
			m_ButtonTest3.SetWindowTextW(title);
			if (m_RandomSize2 >= 1024)
			{
				title.Format(L"RND%dM\r\nQ%dT%d", m_RandomSize2 / 1024, m_RandomQueues2, m_RandomThreads2);
			}
			else
			{
				title.Format(L"RND%dK\r\nQ%dT%d", m_RandomSize2, m_RandomQueues2, m_RandomThreads2);
			}
			m_ButtonTest4.SetWindowTextW(title);

			if (m_SequentialSize1 >= 1024)
			{
				toolTip.Format(L"Sequential %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_SequentialSize1 / 1024, m_SequentialQueues1, m_SequentialThreads1);
			}
			else
			{
				toolTip.Format(L"Sequential %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1);
			}
			m_ButtonTest1.SetToolTipText(toolTip);
			if (m_SequentialSize2 >= 1024)
			{
				toolTip.Format(L"Sequential %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_SequentialSize2 / 1024, m_SequentialQueues2, m_SequentialThreads2);
			}
			else
			{
				toolTip.Format(L"Sequential %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2);
			}
			m_ButtonTest2.SetToolTipText(toolTip);
			if (m_RandomSize1 >= 1024)
			{
				toolTip.Format(L"Random %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_RandomSize1 / 1024, m_RandomQueues1, m_RandomThreads1);
			}
			else
			{
				toolTip.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			}
			toolTip.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d", m_RandomSize1, m_RandomQueues1, m_RandomThreads1);
			m_ButtonTest3.SetToolTipText(toolTip);
			if (m_RandomSize2 >= 1024)
			{
				toolTip.Format(L"Random %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_RandomSize2 / 1024, m_RandomQueues2, m_RandomThreads2);
			}
			else
			{
				toolTip.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", m_RandomSize2, m_RandomQueues2, m_RandomThreads2);
			}
			m_ButtonTest4.SetToolTipText(toolTip);
		}
	}
	else
	{
#ifdef MIX_MODE
		m_ComboMix.EnableWindow(FALSE);
#endif
		m_ComboCount.EnableWindow(FALSE);
		m_ComboSize.EnableWindow(FALSE);
		m_ComboDrive.EnableWindow(FALSE);
		m_ComboUnit.EnableWindow(FALSE);
		m_ComboMode.EnableWindow(FALSE);

		m_ButtonAll.SetWindowTextW(L"Stop");
		m_ButtonTest1.SetWindowTextW(L"Stop");
		m_ButtonTest2.SetWindowTextW(L"Stop");
		m_ButtonTest3.SetWindowTextW(L"Stop");
		m_ButtonTest4.SetWindowTextW(L"Stop");
	}
}

LRESULT CDiskMarkDlg::OnUpdateMessage(WPARAM wParam, LPARAM lParam)
{
	CString wstr = L"";
	CString lstr = L"";

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

void CDiskMarkDlg::SetMeter(CStaticFx* control, double score, double latency, int blockSize, int unit)
{
	CString cstr;

	double meterRatio = 0.0;

	if (unit == SCORE_UNIT::SCORE_US)
	{
		if (latency > 0.0000000001)
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
			cstr.Format(L"%d", (int)iops);
		}
		else
		{
			cstr.Format(L"%.2f", iops);
		}
	}
	else if (unit == SCORE_UNIT::SCORE_US)
	{
		if (score <= 0.0)
		{
			cstr.Format(L"%.2f", 0.0);
		}
		else if (latency >= 1000000.0)
		{
			cstr.Format(L"%d", (int)latency);
		}
		else
		{
			cstr.Format(L"%.2f", latency);
		}
	}
	else if (unit == SCORE_UNIT::SCORE_GBS)
	{
		cstr.Format(L"%.3f", score / 1000.0);
	}
	else
	{
		if (score >= 1000000.0)
		{
			cstr.Format(L"%d", (int)score);
		}
		else
		{
			cstr.Format(L"%.2f", score);
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

	while( pDrive[0] != L'\0' )
	{
		ULARGE_INTEGER freeBytesAvailableToCaller = {0};
		ULARGE_INTEGER totalNumberOfBytes = {0};
		ULARGE_INTEGER totalNumberOfFreeBytes = {0};

	//	_tcsupr_s(pDrive, sizeof(TCHAR) * 4);
		int result = GetDriveType(pDrive);
	
		int forward = (int)_tcslen( pDrive );

		if(result == DRIVE_FIXED || result == DRIVE_REMOTE || result == DRIVE_REMOVABLE || result == DRIVE_RAMDISK)
		{
			pDrive[1] = L'\0';
			cstr.Format(L"%C: ", pDrive[0]);
			if(GetDiskFreeSpaceEx(cstr, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes) != 0)
			{
				select += cstr;
				if(totalNumberOfBytes.QuadPart < ((ULONGLONG)8 * 1024 * 1024 * 1024)) // < 8 GB
				{
					cstr.Format(L"%s: %.0f%% (%.0f/%.0fMiB)", pDrive,
							(double)(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100,
							(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / 1024 / 1024.0,
							totalNumberOfBytes.QuadPart  / 1024 / 1024.0);
				}
				else
				{
					cstr.Format(L"%s: %.0f%% (%.0f/%.0fGiB)", pDrive,
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
	m_CurrentLangPath.Format(L"%s\\%s.lang", (LPTSTR)m_LangDir.GetString(), (LPTSTR)LangName.GetString());

	CString cstr;
	CMenu *menu = GetMenu();
	CMenu subMenu;

	cstr = i18n(L"Menu", L"FILE");
	menu->ModifyMenu(0, MF_BYPOSITION | MF_STRING, 0, cstr);
	cstr = i18n(L"Menu", L"SETTINGS");
	menu->ModifyMenu(1, MF_BYPOSITION | MF_STRING, 1, cstr);
	cstr = i18n(L"Menu", L"PROFILE");
	menu->ModifyMenu(2, MF_BYPOSITION | MF_STRING, 2, cstr);
	cstr = i18n(L"Menu", L"THEME");
	menu->ModifyMenu(3, MF_BYPOSITION | MF_STRING, 3, cstr);
	cstr = i18n(L"Menu", L"HELP");
	menu->ModifyMenu(4, MF_BYPOSITION | MF_STRING, 4, cstr);
	cstr = i18n(L"Menu", L"LANGUAGE");
	if(cstr.Find(L"Language") >= 0)
	{
		cstr = L"&Language";
		menu->ModifyMenu(5, MF_BYPOSITION | MF_STRING, 5, cstr);
	}
	else
	{
		menu->ModifyMenu(5, MF_BYPOSITION | MF_STRING, 5, cstr + L"(&Language)");
	}

	cstr = i18n(L"Menu", L"FILE_EXIT") + L"\tAlt + F4";
	menu->ModifyMenu(ID_EXIT, MF_STRING, ID_EXIT, cstr);
	cstr = i18n(L"Menu", L"SAVE_TEXT") + L"\tCtrl + T";
	menu->ModifyMenu(ID_SAVE_TEXT, MF_STRING, ID_SAVE_TEXT, cstr);
	cstr = i18n(L"Menu", L"SAVE_IMAGE") + L"\tCtrl + S";
	menu->ModifyMenu(ID_SAVE_IMAGE, MF_STRING, ID_SAVE_IMAGE, cstr);

	cstr = i18n(L"Menu", L"EDIT_COPY") + L"\tCtrl + Shift + C";
	menu->ModifyMenu(ID_COPY, MF_STRING, ID_COPY, cstr);
	cstr = i18n(L"Menu", L"QUEUES_THREADS") + L"\tCtrl + Q";
	menu->ModifyMenu(ID_SETTINGS_QUEUESTHREADS, MF_STRING, ID_SETTINGS_QUEUESTHREADS, cstr);

	cstr = i18n(L"Menu", L"PROFILE_DEFAULT");
	menu->ModifyMenu(ID_PROFILE_DEFAULT, MF_STRING, ID_PROFILE_DEFAULT, cstr);
	cstr = i18n(L"Menu", L"PROFILE_PEAK");
	menu->ModifyMenu(ID_PROFILE_PEAK, MF_STRING, ID_PROFILE_PEAK, cstr);
	cstr = i18n(L"Menu", L"PROFILE_REAL");
	menu->ModifyMenu(ID_PROFILE_REAL, MF_STRING, ID_PROFILE_REAL, cstr);

#ifdef MIX_MODE
	cstr = i18n(L"Menu", L"PROFILE_DEFAULT") + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_DEFAULT_MIX, MF_STRING, ID_PROFILE_DEFAULT_MIX, cstr);
	cstr = i18n(L"Menu", L"PROFILE_PEAK") + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_PEAK_MIX, MF_STRING, ID_PROFILE_PEAK_MIX, cstr);
	cstr = i18n(L"Menu", L"PROFILE_REAL") + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_REAL_MIX, MF_STRING, ID_PROFILE_REAL_MIX, cstr);
#endif

	cstr = i18n(L"Menu", L"HELP") + L"\tF1";
	menu->ModifyMenu(ID_HELP, MF_STRING, ID_HELP, cstr);
	cstr = i18n(L"Menu", L"HELP_ABOUT");
	menu->ModifyMenu(ID_ABOUT, MF_STRING, ID_ABOUT, cstr);

	subMenu.Attach(menu->GetSubMenu(1)->GetSafeHmenu());
	cstr = i18n(L"Menu", L"TEST_DATA");
	subMenu.ModifyMenu(0, MF_BYPOSITION, 0, cstr);
	cstr = i18n(L"Menu", L"INTERVAL_TIME");
	subMenu.ModifyMenu(1, MF_BYPOSITION, 1, cstr);
	subMenu.Detach();

	CString second, minute;

	second = i18n(L"Menu", L"SECOND");
	minute = i18n(L"Menu", L"MINUTE");
	menu->ModifyMenu(ID_INTERVAL_TIME_0, MF_STRING, ID_INTERVAL_TIME_0, L"0 " + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_1, MF_STRING, ID_INTERVAL_TIME_1, L"1 " + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_3, MF_STRING, ID_INTERVAL_TIME_3, L"3 " + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_5, MF_STRING, ID_INTERVAL_TIME_5, L"5 " + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_10, MF_STRING, ID_INTERVAL_TIME_10, L"10 " + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_30, MF_STRING, ID_INTERVAL_TIME_30, L"30 " + second);
	menu->ModifyMenu(ID_INTERVAL_TIME_60, MF_STRING, ID_INTERVAL_TIME_60, L"1 " + minute);
	menu->ModifyMenu(ID_INTERVAL_TIME_180, MF_STRING, ID_INTERVAL_TIME_180, L"3 " + minute);
	menu->ModifyMenu(ID_INTERVAL_TIME_300, MF_STRING, ID_INTERVAL_TIME_300, L"5 " + minute);
	menu->ModifyMenu(ID_INTERVAL_TIME_600, MF_STRING, ID_INTERVAL_TIME_600, L"10 " + minute);

	cstr = i18n(L"Menu", L"DEFAULT_RANDOM");
	menu->ModifyMenu(ID_MODE_DEFAULT, MF_STRING, ID_MODE_DEFAULT, cstr);
	cstr = i18n(L"Menu", L"ALL_ZERO");
	menu->ModifyMenu(ID_MODE_ALL0X00, MF_STRING, ID_MODE_ALL0X00, cstr);

	// Theme
	subMenu.Attach(menu->GetSubMenu(3)->GetSafeHmenu());
	cstr = i18n(L"Menu", L"ZOOM");
	subMenu.ModifyMenu(0, MF_BYPOSITION, 0, cstr);
	subMenu.Detach();

	cstr = i18n(L"Menu", L"AUTO");
	menu->ModifyMenu(ID_ZOOM_AUTO, MF_STRING, ID_ZOOM_AUTO, cstr);

	cstr = i18n(L"Menu", L"FONT_SETTING") + L"\tCtrl + F";
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

	m_MesStopBenchmark = i18n(L"Message", L"STOP_BENCHMARK");
	m_MesDiskCapacityError = i18n(L"Message", L"DISK_CAPACITY_ERROR");
	m_MesDiskCreateFileError = i18n(L"Message", L"DISK_CREATE_FILE_ERROR");
	m_MesDiskWriteError = i18n(L"Message", L"DISK_WRITE_ERROR");
	m_MesDiskReadError = i18n(L"Message", L"DISK_READ_ERROR");
	m_MesDiskSpdNotFound = i18n(L"Message", L"DISK_SPD_NOT_FOUND");

	InitDrive();

	WritePrivateProfileString(L"Setting", L"Language", LangName, m_Ini);
}

BOOL CDiskMarkDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Select Theme
	if (WM_THEME_ID <= wParam && wParam < WM_THEME_ID + (UINT)m_MenuArrayTheme.GetSize())
	{
		CMainDialogFx::OnCommand(wParam, lParam);
		return TRUE;
	}

	// Select Language
	if(WM_LANGUAGE_ID <= wParam && wParam < WM_LANGUAGE_ID + (UINT)m_MenuArrayLang.GetSize())
	{
		CMenu menu;
		CMenu subMenu;
		CMenu subMenuAN;
		CMenu subMenuOZ;
		menu.Attach(GetMenu()->GetSafeHmenu());
		subMenu.Attach(menu.GetSubMenu(MENU_LANG_INDEX)->GetSafeHmenu());
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

		UpdateComboTooltip();
	}

	return CMainDialogFx::OnCommand(wParam, lParam);
}

void CDiskMarkDlg::OnCopy()
{
	SaveText(L"");
}

void CDiskMarkDlg::OnSaveText()
{
	CString path;
	SYSTEMTIME st;
	GetLocalTime(&st);
	path.Format(L"%s_%04d%02d%02d%0d%02d%02d", PRODUCT_NAME, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	CString filter = L"TEXT (*.txt)|*.txt||";
	CFileDialog save(FALSE, L"txt", path, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, filter);

	if (save.DoModal() == IDOK)
	{
		SaveText(save.GetPathName());
	}
}

void CDiskMarkDlg::OnSaveImage()
{
	SaveImage();
}

CString CDiskMarkDlg::GetRandomResultString(double score, double latency, int size, int queues, int threads)
{
	CString result;
	double iops = 0.0;

	iops = score * 1000 * 1000 / ((double)size * 1024);


	if (size >= 1024)
	{
		result.Format(L"Random %3dMiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size, queues, threads, score, iops, latency);
	}
	else
	{
		result.Format(L"Random %3dKiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size, queues, threads, score, iops, latency);
	}

	if (size > 1000)
	{
		result = L" " + result;

	}
	else if (size > 100)
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

	if (size >= 1024)
	{
		result.Format(L"Sequential %3dMiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size / 1024, queues , threads, score, iops, latency);
	}
	else
	{
		result.Format(L"Sequential %3dKiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size, queues, threads, score, iops, latency);
	}

	return result;
}

void CDiskMarkDlg::SaveText(CString fileName)
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

	clip.Replace(L"%PRODUCT%", PRODUCT_NAME);
	clip.Replace(L"%VERSION%", PRODUCT_VERSION);
	
	cstr = PRODUCT_EDITION;
	if(! cstr.IsEmpty())
	{
		clip.Replace(L"%EDITION%", L" " PRODUCT_EDITION);
	}
	else
	{
		clip.Replace(L"%EDITION%", PRODUCT_EDITION);
	}	
	clip.Replace(L"%COPY_YEAR%", PRODUCT_COPY_YEAR);

	double iops = 0.0;
	double latency = 0.0;

	if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		clip.Replace(L"%SequentialRead1%", GetSequentialResultString(m_SequentialReadScore1, m_SequentialReadLatency1, 1, 1, 1));
		clip.Replace(L"%SequentialWrite1%", GetSequentialResultString(m_SequentialWriteScore1, m_SequentialWriteLatency1, 1, 1, 1));
		clip.Replace(L"%RandomRead1%", GetRandomResultString(m_RandomReadScore1, m_RandomReadLatency1, 4, 1, 1));
		clip.Replace(L"%RandomWrite1%", GetRandomResultString(m_RandomWriteScore1, m_RandomWriteLatency1, 4, 1, 1));

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip.Replace(L"%SequentialMix1%", GetSequentialResultString(m_SequentialMixScore1, m_SequentialMixLatency1, 1, 1, 1));
			clip.Replace(L"%RandomMix1%", GetRandomResultString(m_RandomMixScore1, m_RandomMixLatency1, 4, 1, 1));
			cstr.Format(L"Read %d%%/Write %d%%", 100 - m_MixRatio, m_MixRatio);
			clip.Replace(L"%MixRatio%", cstr);
		}
#endif
	}
	else 
	{
		clip.Replace(L"%SequentialRead1%", GetSequentialResultString(m_SequentialReadScore1, m_SequentialReadLatency1, m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1));
		clip.Replace(L"%SequentialRead2%", GetSequentialResultString(m_SequentialReadScore2, m_SequentialReadLatency2, m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2));
		clip.Replace(L"%SequentialWrite1%", GetSequentialResultString(m_SequentialWriteScore1, m_SequentialWriteLatency1, m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1));
		clip.Replace(L"%SequentialWrite2%", GetSequentialResultString(m_SequentialWriteScore2, m_SequentialWriteLatency2, m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2));

		clip.Replace(L"%RandomRead1%", GetRandomResultString(m_RandomReadScore1, m_RandomReadLatency1, m_RandomSize1, m_RandomQueues1, m_RandomThreads1));
		clip.Replace(L"%RandomRead2%", GetRandomResultString(m_RandomReadScore2, m_RandomReadLatency2, m_RandomSize2, m_RandomQueues2, m_RandomThreads2));
		clip.Replace(L"%RandomWrite1%", GetRandomResultString(m_RandomWriteScore1, m_RandomWriteLatency1, m_RandomSize1, m_RandomQueues1, m_RandomThreads1));
		clip.Replace(L"%RandomWrite2%", GetRandomResultString(m_RandomWriteScore2, m_RandomWriteLatency2, m_RandomSize2, m_RandomQueues2, m_RandomThreads2));

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip.Replace(L"%SequentialMix1%", GetSequentialResultString(m_SequentialMixScore1, m_SequentialMixLatency1, m_SequentialSize1, m_SequentialQueues1, m_SequentialThreads1));
			clip.Replace(L"%SequentialMix2%", GetSequentialResultString(m_SequentialMixScore2, m_SequentialMixLatency2, m_SequentialSize2, m_SequentialQueues2, m_SequentialThreads2));
			clip.Replace(L"%RandomMix1%", GetRandomResultString(m_RandomMixScore1, m_RandomMixLatency1, m_RandomSize1, m_RandomQueues1, m_RandomThreads1));
			clip.Replace(L"%RandomMix2%", GetRandomResultString(m_RandomMixScore2, m_RandomMixLatency2, m_RandomSize2, m_RandomQueues2, m_RandomThreads2));

			cstr.Format(L"Read %d%%/Write %d%%", 100 - m_MixRatio, m_MixRatio);
			clip.Replace(L"%MixRatio%", cstr);
		}
#endif
	}

	if (m_ValueTestSize.Find(L"MiB") == -1)
	{
		cstr.Format(L"%d GiB", _tstoi(m_ValueTestSize));
	}
	else
	{
		cstr.Format(L"%d MiB", _tstoi(m_ValueTestSize));
	}

	clip.Replace(L"%TestSize%", cstr);
	cstr.Format(L"%d", _tstoi(m_ValueTestCount));
	clip.Replace(L"%TestCount%", cstr);

	if (m_Affinity == AFFINITY_DISABLED)
	{
		cstr = L"<DefaultAffinity=DISABLED>";
	}
	else
	{
		cstr = L"<DefaultAffinity=ENABLED>";
	}
	clip.Replace(L"%Affinity%", cstr);

	m_Comment.GetWindowText(cstr);
	if (cstr.IsEmpty())
	{
		clip.Replace(L"%Comment%", L"");
	}else
	{
		clip.Replace(L"%Comment%", L"Comment: " + cstr + L"\r\n");
	}

	if(m_TestData == TEST_DATA_ALL0X00)
	{
		clip.Replace(L"%TestMode%", L" " ALL_0X00_0FILL);
	}
	else
	{
		clip.Replace(L"%TestMode%", L"");
	}
	cstr.Format(L"Interval: %d sec", m_IntervalTime);
	clip.Replace(L"%IntervalTime%", cstr);

	/*
	if (m_FragmenteCounts > 0)
	{
		cstr.Format(L"[%d]", m_FragmenteCounts);
		clip.Replace(L"%FragmenteCounts%", cstr);
	}
	else
	{
		clip.Replace(L"%FragmenteCounts%", ""));
	}
	*/

	GetOsName(cstr);
	clip.Replace(L"%OS%", cstr);

	SYSTEMTIME st;
	GetLocalTime(&st);
	cstr.Format(L"%04d/%02d/%02d %d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	clip.Replace(L"%Date%", cstr);

	if (fileName.IsEmpty())
	{
		if (OpenClipboard())
		{
			HGLOBAL clipbuffer;
			TCHAR* buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, sizeof(TCHAR) * (clip.GetLength() + 1));
			buffer = (TCHAR*)GlobalLock(clipbuffer);
			_tcscpy_s(buffer, clip.GetLength() + 1, LPCTSTR(clip));
			GlobalUnlock(clipbuffer);
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
		CT2A utf8(clip, CP_UTF8);

		CFile file;
		if (file.Open(fileName, CFile::modeCreate | CFile::modeWrite))
		{
			file.Write((char*)utf8, (UINT)strlen(utf8));
			file.Close();
		}
	}
}

void CDiskMarkDlg::OnZoom100()
{
	if (CheckRadioZoomType(ID_ZOOM_100, 100))
	{
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom125()
{
	if (CheckRadioZoomType(ID_ZOOM_125, 125))
	{
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom150()
{
	if (CheckRadioZoomType(ID_ZOOM_150, 150))
	{
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom200()
{
	if (CheckRadioZoomType(ID_ZOOM_200, 200))
	{
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom250()
{
	if (CheckRadioZoomType(ID_ZOOM_250, 250))
	{
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoom300()
{
	if (CheckRadioZoomType(ID_ZOOM_300, 300))
	{
		UpdateDialogSize();
	}
}

void CDiskMarkDlg::OnZoomAuto()
{
	if (CheckRadioZoomType(ID_ZOOM_AUTO, 0))
	{
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
	cstr.Format(L"%d", value);
	WritePrivateProfileString(L"Setting", L"ZoomType", cstr, m_Ini);

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
	cstr.Format(L"%d", value);
	WritePrivateProfileString(L"Setting", L"IntervalTime", cstr, m_Ini);

	return TRUE;
}

void CDiskMarkDlg::CheckIntervalTime()
{
	int id;
	m_IntervalTime = GetPrivateProfileInt(L"Setting", L"IntervalTime", 5, m_Ini);

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
		WritePrivateProfileString(L"Setting", L"IntervalTime", L"5", m_Ini); 
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
	WritePrivateProfileString(L"Setting", L"TestData", L"0", m_Ini);
	SetWindowTitle(L"");
}

void CDiskMarkDlg::OnModeAll0x00()
{
	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_MODE_DEFAULT, ID_MODE_ALL0X00, ID_MODE_ALL0X00, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_TestData = TEST_DATA_ALL0X00;
	WritePrivateProfileString(L"Setting", L"TestData", L"1", m_Ini);
	SetWindowTitle(L"");
}

void CDiskMarkDlg::OnProfileDefault()
{
	ProfileDefault();
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	ChangeButtonStatus(TRUE);
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
	WritePrivateProfileString(L"Setting", L"Profile", L"0", m_Ini);
}

void CDiskMarkDlg::OnProfilePeak()
{
	ProfilePeak();
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	ChangeButtonStatus(TRUE);
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
	WritePrivateProfileString(L"Setting", L"Profile", L"1", m_Ini);
}

void CDiskMarkDlg::OnProfileReal()
{
	ProfileReal();
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	ChangeButtonStatus(TRUE);
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
	WritePrivateProfileString(L"Setting", L"Profile", L"2", m_Ini);
}

#ifdef MIX_MODE
void CDiskMarkDlg::OnProfileDefaultMix()
{
	ProfileDefaultMix();
	UpdateUnitLabel();
	InitScore();
	UpdateScore();
	UpdateDialogSize();
	ChangeButtonStatus(TRUE);
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
	WritePrivateProfileString(L"Setting", L"Profile", L"3", m_Ini);
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
	WritePrivateProfileString(L"Setting", L"Profile", L"4", m_Ini);
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
	WritePrivateProfileString(L"Setting", L"Profile", L"5", m_Ini);
}
#endif

void CDiskMarkDlg::OnSettingsQueuesThreads()
{
	if (! m_DiskBenchStatus)
	{
		CSettingsDlg dlg(this);
		dlg.DoModal();
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);

		if (IsNormMode())
		{
			m_IndexTestMode = 0;
		}
		else if (IsNVMeMode())
		{
			m_IndexTestMode = 1;
		}
		else if (IsAtaMode())
		{
			m_IndexTestMode = 2;
		}
		else
		{
			m_IndexTestMode = 3;
		}
		m_ComboMode.SetCurSel(m_IndexTestMode);
		UpdateData(FALSE);
	}
}

void CDiskMarkDlg::SettingsQueuesThreads(int type)
{
	switch (type)
	{
	case 0:// NORM
		WritePrivateProfileString(L"Setting", L"SequentialSize1", L"1024", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialQueues1", L"8", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialThreads1", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialSize2", L"1024", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialQueues2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialThreads2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomSize1", L"4", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomQueues1", L"32", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomThreads1", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomSize2", L"4", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomQueues2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomThreads2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"Affinity", L"0", m_Ini);
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);
		break;
	case 1: // NVMe
		WritePrivateProfileString(L"Setting", L"SequentialSize1", L"1024", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialQueues1", L"8", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialThreads1", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialSize2", L"1024", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialQueues2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialThreads2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomSize1", L"4", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomQueues1", L"32", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomThreads1", L"16", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomSize2", L"4", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomQueues2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomThreads2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"Affinity", L"0", m_Ini);
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);
		break;
	case 2: // Ata
		WritePrivateProfileString(L"Setting", L"SequentialSize1", L"128", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialQueues1", L"32", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialThreads1", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialSize2", L"128", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialQueues2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"SequentialThreads2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomSize1", L"4", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomQueues1", L"32", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomThreads1", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomSize2", L"4", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomQueues2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"RandomThreads2", L"1", m_Ini);
		WritePrivateProfileString(L"Setting", L"Affinity", L"0", m_Ini);
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);
		break;
	default:
		OnSettingsQueuesThreads();
		break;
	}
}

typedef BOOL(WINAPI *FuncEnableNonClientDpiScaling) (HWND hwnd);

BOOL CDiskMarkDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CMainDialogFx::OnNcCreate(lpCreateStruct))
		return FALSE;

	HMODULE hModule = GetModuleHandle(L"User32.dll");
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
	CFontSelectionDlg fontSelection(this);
	if (fontSelection.DoModal() == IDOK)
	{
		m_FontFace = fontSelection.GetFontFace();
		m_FontScale = fontSelection.GetFontScale();
		m_FontRatio = m_FontScale / 100.0;
	//	SetControlFont();
	//	Invalidate();
		CString cstr;
		WritePrivateProfileString(L"Setting", L"FontFace", L"\"" + m_FontFace + L"\"", m_Ini);
		cstr.Format(L"%d", m_FontScale);
		WritePrivateProfileString(L"Setting", L"FontScale", cstr, m_Ini);

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

void CDiskMarkDlg::OnCbnSelchangeComboMode()
{
	UpdateData(TRUE);
	SettingsQueuesThreads(m_IndexTestMode);
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
	// GotoDlgCtrl(GetDlgItem(IDOK));
}

void CDiskMarkDlg::UpdateUnitLabel()
{
	if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		m_ReadUnit.SetWindowTextW(L"Read (MB/s)");
		m_WriteUnit.SetWindowTextW(L"Write (MB/s)");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix (MB/s)");
#endif
		return;
	}
	else if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		m_ReadUnit.SetWindowTextW(L"Read (MB/s)");
		m_WriteUnit.SetWindowTextW(L"Write (MB/s)");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix (MB/s)");
#endif
		return;
	}

	if (m_IndexTestUnit == SCORE_UNIT::SCORE_IOPS)
	{
		m_ReadUnit.SetWindowTextW(L"Read");
		m_WriteUnit.SetWindowTextW(L"Write");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix");
#endif
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_US)
	{
		m_ReadUnit.SetWindowTextW(L"Read");
		m_WriteUnit.SetWindowTextW(L"Write");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix");
#endif
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_GBS)
	{
		m_ReadUnit.SetWindowTextW(L"Read");
		m_WriteUnit.SetWindowTextW(L"Write");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix");
#endif
	}
	else
	{
		m_ReadUnit.SetWindowTextW(L"Read");
		m_WriteUnit.SetWindowTextW(L"Write");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix");
#endif
	}
}

void CDiskMarkDlg::SetWindowTitle(CString message)
{
	CString title;

	if (!message.IsEmpty())
	{
		title.Format(L"%s - %s", PRODUCT_SHORT_NAME, message.GetString());
	}
	else
	{
		title.Format(L"%s %s %s", PRODUCT_NAME, PRODUCT_VERSION, PRODUCT_EDITION);
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
