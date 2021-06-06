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

#ifdef TSUKUMO_TOKKA_SUPPORT
	m_DefaultTheme = L"Tokka01_Cute";
	m_RecommendTheme = L"Tokka01_Cute";
	m_ThemeKeyName = L"ThemeTokka";

	m_MarginButtonTop = 16;
	m_MarginButtonLeft = 0;
	m_MarginButtonBottom = 16;
	m_MarginButtonRight = 0;
	m_MarginMeterTop = 0;
	m_MarginMeterLeft = 0;
	m_MarginMeterBottom = 0;
	m_MarginMeterRight = 16;
	m_MarginCommentTop = 0;
	m_MarginCommentLeft = 16;
	m_MarginCommentBottom = 0;
	m_MarginCommentRight = 16;
	m_MarginDemoTop = 24;
	m_MarginDemoLeft = 24;
	m_MarginDemoBottom = 24;
	m_MarginDemoRight = 24;
#elif SUISHO_SHIZUKU_SUPPORT
	m_DefaultTheme = L"Shizuku";
	m_RecommendTheme = L"ShizukuIdol";
	m_ThemeKeyName = L"ThemeShizuku";

	m_MarginButtonTop = 8;
	m_MarginButtonLeft = 0;
	m_MarginButtonBottom = 8;
	m_MarginButtonRight = 0;
	m_MarginMeterTop = 0;
	m_MarginMeterLeft = 0;
	m_MarginMeterBottom = 0;
	m_MarginMeterRight = 16;
	m_MarginCommentTop = 0;
	m_MarginCommentLeft = 16;
	m_MarginCommentBottom = 0;
	m_MarginCommentRight = 64;
	m_MarginDemoTop = 24;
	m_MarginDemoLeft = 24;
	m_MarginDemoBottom = 24;
	m_MarginDemoRight = 24;
#else
	m_DefaultTheme = L"Default";
	m_ThemeKeyName = L"Theme";

	m_MarginButtonTop = 4;
	m_MarginButtonLeft = 0;
	m_MarginButtonBottom = 4;
	m_MarginButtonRight = 0;
	m_MarginMeterTop = 0;
	m_MarginMeterLeft = 0;
	m_MarginMeterBottom = 0;
	m_MarginMeterRight = 4;
	m_MarginCommentTop = 0;
	m_MarginCommentLeft = 4;
	m_MarginCommentBottom = 0;
	m_MarginCommentRight = 4;
	m_MarginDemoTop = 8;
	m_MarginDemoLeft = 8;
	m_MarginDemoBottom = 8;
	m_MarginDemoRight = 8;
#endif

	m_BackgroundName = L"Background";
	m_RandomThemeLabel = L"Random";
	m_RandomThemeName = L"";

	m_AdminMode = IsUserAnAdmin();
}


void CDiskMarkDlg::UpdateThemeInfo()
{
	CMainDialogFx::UpdateThemeInfo();

	CString theme = m_ThemeDir + m_CurrentTheme + L"\\theme.ini";

#ifdef TSUKUMO_TOKKA_SUPPORT
	m_MarginButtonTop = GetPrivateProfileInt(L"Margin", L"ButtonTop", 16, theme);
	m_MarginButtonLeft = GetPrivateProfileInt(L"Margin", L"ButtonLeft", 0, theme);
	m_MarginButtonBottom = GetPrivateProfileInt(L"Margin", L"ButtonBottom", 16, theme);
	m_MarginButtonRight = GetPrivateProfileInt(L"Margin", L"ButtonRight", 0, theme);
	m_MarginMeterTop = GetPrivateProfileInt(L"Margin", L"MeterTop", 0, theme);
	m_MarginMeterLeft = GetPrivateProfileInt(L"Margin", L"MeterLeft", 0, theme);
	m_MarginMeterBottom = GetPrivateProfileInt(L"Margin", L"MeterBottom", 0, theme);
	m_MarginMeterRight = GetPrivateProfileInt(L"Margin", L"MeterRight", 16, theme);
	m_MarginCommentTop = GetPrivateProfileInt(L"Margin", L"CommentTop", 0, theme);
	m_MarginCommentLeft = GetPrivateProfileInt(L"Margin", L"CommentLeft", 4, theme);
	m_MarginCommentBottom = GetPrivateProfileInt(L"Margin", L"CommentBottom", 0, theme);
	m_MarginCommentRight = GetPrivateProfileInt(L"Margin", L"CommentRight", 4, theme);
	m_MarginDemoTop = GetPrivateProfileInt(L"Margin", L"DemoTop", 24, theme);
	m_MarginDemoLeft = GetPrivateProfileInt(L"Margin", L"DemoLeft", 24, theme);
	m_MarginDemoBottom = GetPrivateProfileInt(L"Margin", L"DemoBottom", 24, theme);
	m_MarginDemoRight = GetPrivateProfileInt(L"Margin", L"DemoRight", 24, theme);
#elif SUISHO_SHIZUKU_SUPPORT
	m_MarginButtonTop = GetPrivateProfileInt(L"Margin", L"ButtonTop", 8, theme);
	m_MarginButtonLeft = GetPrivateProfileInt(L"Margin", L"ButtonLeft", 0, theme);
	m_MarginButtonBottom = GetPrivateProfileInt(L"Margin", L"ButtonBottom", 8, theme);
	m_MarginButtonRight = GetPrivateProfileInt(L"Margin", L"ButtonRight", 0, theme);
	m_MarginMeterTop = GetPrivateProfileInt(L"Margin", L"MeterTop", 0, theme);
	m_MarginMeterLeft = GetPrivateProfileInt(L"Margin", L"MeterLeft", 0, theme);
	m_MarginMeterBottom = GetPrivateProfileInt(L"Margin", L"MeterBottom", 0, theme);
	m_MarginMeterRight = GetPrivateProfileInt(L"Margin", L"MeterRight", 16, theme);
	m_MarginCommentTop = GetPrivateProfileInt(L"Margin", L"CommentTop", 0, theme);
	m_MarginCommentLeft = GetPrivateProfileInt(L"Margin", L"CommentLeft", 16, theme);
	m_MarginCommentBottom = GetPrivateProfileInt(L"Margin", L"CommentBottom", 0, theme);
	m_MarginCommentRight = GetPrivateProfileInt(L"Margin", L"CommentRight", 16, theme);
	m_MarginDemoTop = GetPrivateProfileInt(L"Margin", L"DemoTop", 24, theme);
	m_MarginDemoLeft = GetPrivateProfileInt(L"Margin", L"DemoLeft", 24, theme);
	m_MarginDemoBottom = GetPrivateProfileInt(L"Margin", L"DemoBottom", 24, theme);
	m_MarginDemoRight = GetPrivateProfileInt(L"Margin", L"DemoRight", 24, theme);
#else
	m_MarginButtonTop = GetPrivateProfileInt(L"Margin", L"ButtonTop", 4, theme);
	m_MarginButtonLeft = GetPrivateProfileInt(L"Margin", L"ButtonLeft", 0, theme);
	m_MarginButtonBottom = GetPrivateProfileInt(L"Margin", L"ButtonBottom", 4, theme);
	m_MarginButtonRight = GetPrivateProfileInt(L"Margin", L"ButtonRight", 0, theme);
	m_MarginMeterTop = GetPrivateProfileInt(L"Margin", L"MeterTop", 0, theme);
	m_MarginMeterLeft = GetPrivateProfileInt(L"Margin", L"MeterLeft", 0, theme);
	m_MarginMeterBottom = GetPrivateProfileInt(L"Margin", L"MeterBottom", 0, theme);
	m_MarginMeterRight = GetPrivateProfileInt(L"Margin", L"MeterRight", 4, theme);
	m_MarginCommentTop = GetPrivateProfileInt(L"Margin", L"CommentTop", 0, theme);
	m_MarginCommentLeft = GetPrivateProfileInt(L"Margin", L"CommentLeft", 8, theme);
	m_MarginCommentBottom = GetPrivateProfileInt(L"Margin", L"CommentBottom", 0, theme);
	m_MarginCommentRight = GetPrivateProfileInt(L"Margin", L"CommentRight", 8, theme);
	m_MarginDemoTop = GetPrivateProfileInt(L"Margin", L"DemoTop", 8, theme);
	m_MarginDemoLeft = GetPrivateProfileInt(L"Margin", L"DemoLeft", 8, theme);
	m_MarginDemoBottom = GetPrivateProfileInt(L"Margin", L"DemoBottom", 8, theme);
	m_MarginDemoRight = GetPrivateProfileInt(L"Margin", L"DemoRight", 8, theme);
#endif
}


CDiskMarkDlg::~CDiskMarkDlg()
{
}

void CDiskMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CMainDialogFx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_ALL, m_ButtonAll);
	DDX_Control(pDX, IDC_BUTTON_TEST_0, m_ButtonTest0);
	DDX_Control(pDX, IDC_BUTTON_TEST_1, m_ButtonTest1);
	DDX_Control(pDX, IDC_BUTTON_TEST_2, m_ButtonTest2);
	DDX_Control(pDX, IDC_BUTTON_TEST_3, m_ButtonTest3);

	DDX_Control(pDX, IDC_TEST_READ_0, m_TestRead0);
	DDX_Control(pDX, IDC_TEST_READ_1, m_TestRead1);
	DDX_Control(pDX, IDC_TEST_READ_2, m_TestRead2);
	DDX_Control(pDX, IDC_TEST_READ_3, m_TestRead3);

	DDX_Control(pDX, IDC_TEST_WRITE_0, m_TestWrite0);
	DDX_Control(pDX, IDC_TEST_WRITE_1, m_TestWrite1);
	DDX_Control(pDX, IDC_TEST_WRITE_2, m_TestWrite2);
	DDX_Control(pDX, IDC_TEST_WRITE_3, m_TestWrite3);

#ifdef MIX_MODE
	DDX_Control(pDX, IDC_TEST_MIX_0, m_TestMix0);
	DDX_Control(pDX, IDC_TEST_MIX_1, m_TestMix1);
	DDX_Control(pDX, IDC_TEST_MIX_2, m_TestMix2);
	DDX_Control(pDX, IDC_TEST_MIX_3, m_TestMix3);
	DDX_Control(pDX, IDC_COMBO_MIX, m_ComboMix);
#endif

	DDX_Control(pDX, IDC_COMMENT, m_Comment);

	DDX_Control(pDX, IDC_COMBO_COUNT, m_ComboCount);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_ComboSize);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_ComboDrive);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_ComboUnit);

	DDX_Control(pDX, IDC_DEMO_SETTING, m_DemoSetting);
	DDX_Control(pDX, IDC_READ_UNIT, m_ReadUnit);
	DDX_Control(pDX, IDC_WRITE_UNIT, m_WriteUnit);

#ifdef MIX_MODE
	DDX_Control(pDX, IDC_MIX_UNIT, m_MixUnit);
#endif

	DDX_Text(pDX, IDC_COMBO_COUNT, m_ValueTestCount);
	DDX_Text(pDX, IDC_COMBO_SIZE, m_ValueTestSize);
	DDX_Text(pDX, IDC_COMBO_DRIVE, m_ValueTestDrive);
	DDX_Text(pDX, IDC_COMBO_UNIT, m_ValueTestUnit);
	DDX_CBIndex(pDX, IDC_COMBO_COUNT, m_IndexTestCount);
	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_IndexTestSize);
	DDX_CBIndex(pDX, IDC_COMBO_DRIVE, m_IndexTestDrive);
	DDX_CBIndex(pDX, IDC_COMBO_UNIT, m_IndexTestUnit);
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
	ON_WM_LBUTTONDOWN()

	ON_COMMAND(ID_ZOOM_100, &CDiskMarkDlg::OnZoom100)
	ON_COMMAND(ID_ZOOM_125, &CDiskMarkDlg::OnZoom125)
	ON_COMMAND(ID_ZOOM_150, &CDiskMarkDlg::OnZoom150)
	ON_COMMAND(ID_ZOOM_200, &CDiskMarkDlg::OnZoom200)
	ON_COMMAND(ID_ZOOM_250, &CDiskMarkDlg::OnZoom250)
	ON_COMMAND(ID_ZOOM_300, &CDiskMarkDlg::OnZoom300)
	ON_COMMAND(ID_ZOOM_AUTO, &CDiskMarkDlg::OnZoomAuto)

	ON_COMMAND(ID_HELP, &CDiskMarkDlg::OnHelp)
	ON_COMMAND(ID_CRYSTALDEWWORLD, &CDiskMarkDlg::OnCrystalDewWorld)
	ON_COMMAND(ID_MODE_DEFAULT, &CDiskMarkDlg::OnModeDefault)
	ON_COMMAND(ID_MODE_ALL0X00, &CDiskMarkDlg::OnModeAll0x00)

	ON_COMMAND(ID_SETTING_DEFAULT, &CDiskMarkDlg::OnSettingDefault)
	ON_COMMAND(ID_SETTING_NVME_8, &CDiskMarkDlg::OnSettingNVMe8)
//	ON_COMMAND(ID_SETTING_NVME_9, &CDiskMarkDlg::OnSettingNVMe9)

	ON_COMMAND(ID_PROFILE_DEFAULT, &CDiskMarkDlg::OnProfileDefault)
	ON_COMMAND(ID_PROFILE_REAL, &CDiskMarkDlg::OnProfileReal)
	ON_COMMAND(ID_PROFILE_PEAK, &CDiskMarkDlg::OnProfilePeak)
	ON_COMMAND(ID_PROFILE_DEMO, &CDiskMarkDlg::OnProfileDemo)

#ifdef MIX_MODE
	ON_COMMAND(ID_PROFILE_DEFAULT_MIX, &CDiskMarkDlg::OnProfileDefaultMix)
	ON_COMMAND(ID_PROFILE_REAL_MIX, &CDiskMarkDlg::OnProfileRealMix)
	ON_COMMAND(ID_PROFILE_PEAK_MIX, &CDiskMarkDlg::OnProfilePeakMix)
#endif

	ON_COMMAND(ID_BENCHMARK_READ_WRITE, &CDiskMarkDlg::OnBenchmarkReadWrite)
	ON_COMMAND(ID_BENCHMARK_READ_ONLY, &CDiskMarkDlg::OnBenchmarkReadOnly)
	ON_COMMAND(ID_BENCHMARK_WRITE_ONLY, &CDiskMarkDlg::OnBenchmarkWriteOnly)

	//}}AFX_MSG_MAP
	ON_COMMAND(ID_SAVE_TEXT, &CDiskMarkDlg::OnSaveText)
	ON_COMMAND(ID_SAVE_IMAGE, &CDiskMarkDlg::OnSaveImage)
	ON_COMMAND(ID_SETTINGS_QUEUESTHREADS, &CDiskMarkDlg::OnSettingsQueuesThreads)
	ON_COMMAND(ID_FONT_SETTING, &CDiskMarkDlg::OnFontSetting)
	ON_WM_NCCREATE()
	ON_MESSAGE(WM_QUERYENDSESSION, &CDiskMarkDlg::OnQueryEndSession)

	ON_BN_CLICKED(IDC_BUTTON_ALL, &CDiskMarkDlg::OnAll)
	ON_BN_CLICKED(IDC_BUTTON_TEST_0, &CDiskMarkDlg::OnTest0)
	ON_BN_CLICKED(IDC_BUTTON_TEST_1, &CDiskMarkDlg::OnTest1)
	ON_BN_CLICKED(IDC_BUTTON_TEST_2, &CDiskMarkDlg::OnTest2)
	ON_BN_CLICKED(IDC_BUTTON_TEST_3, &CDiskMarkDlg::OnTest3)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVE, &CDiskMarkDlg::OnCbnSelchangeComboDrive)
	ON_CBN_SELCHANGE(IDC_COMBO_UNIT, &CDiskMarkDlg::OnCbnSelchangeComboUnit)
#ifdef MIX_MODE
	ON_CBN_SELCHANGE(IDC_COMBO_MIX, &CDiskMarkDlg::OnCbnSelchangeComboMix)
#endif

END_MESSAGE_MAP()

LRESULT CDiskMarkDlg::OnQueryEndSession(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

BOOL CDiskMarkDlg::CheckThemeEdition(CString name)
{
#ifdef TSUKUMO_TOKKA_SUPPORT
	if (name.Find(L"Tokka") == 0) { return TRUE; }
#elif SUISHO_SHIZUKU_SUPPORT
	if(name.Find(L"Shizuku") == 0) { return TRUE; }
#elif KUREI_KEI_SUPPORT
	if(name.Find(L"KureiKei") == 0) { return TRUE; }
#else
	if(name.Find(L"Shizuku") != 0 && name.Find(L"Tokka") != 0 && name.Find(L"KureiKei") != 0 && name.Find(L".") != 0) { return TRUE; }
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

BOOL CDiskMarkDlg::IsDefaultMode()
{
	if (m_BenchSize[0] == 1024 && m_BenchQueues[0] == 8  && m_BenchThreads[0] == 1 && m_BenchType[0] == BENCH_SEQ
	&&  m_BenchSize[1] == 1024 && m_BenchQueues[1] == 1  && m_BenchThreads[1] == 1 && m_BenchType[1] == BENCH_SEQ
	&&  m_BenchSize[2] == 4    && m_BenchQueues[2] == 32 && m_BenchThreads[2] == 1 && m_BenchType[2] == BENCH_RND
	&&  m_BenchSize[3] == 4    && m_BenchQueues[3] == 1  && m_BenchThreads[3] == 1 && m_BenchType[3] == BENCH_RND
	&&  m_BenchSize[4] == 1024 && m_BenchQueues[4] == 8  && m_BenchThreads[4] == 1 && m_BenchType[4] == BENCH_SEQ
	&&  m_BenchSize[5] == 4    && m_BenchQueues[5] == 32 && m_BenchThreads[5] == 1 && m_BenchType[5] == BENCH_RND
	&&  m_BenchSize[8] == 1024 && m_BenchQueues[8] == 8  && m_BenchThreads[8] == 1 && m_BenchType[8] == BENCH_SEQ
	)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDiskMarkDlg::IsNVMe8Mode()
{
	if (m_BenchSize[0] == 1024 && m_BenchQueues[0] == 8  && m_BenchThreads[0] == 1  && m_BenchType[0] == BENCH_SEQ
	&&  m_BenchSize[1] == 128  && m_BenchQueues[1] == 32 && m_BenchThreads[1] == 1  && m_BenchType[1] == BENCH_SEQ
	&&  m_BenchSize[2] == 4    && m_BenchQueues[2] == 32 && m_BenchThreads[2] == 16 && m_BenchType[2] == BENCH_RND
	&&  m_BenchSize[3] == 4    && m_BenchQueues[3] == 1  && m_BenchThreads[3] == 1  && m_BenchType[3] == BENCH_RND
	&&  m_BenchSize[4] == 1024 && m_BenchQueues[4] == 8  && m_BenchThreads[4] == 1  && m_BenchType[4] == BENCH_SEQ
	&&  m_BenchSize[5] == 4    && m_BenchQueues[5] == 32 && m_BenchThreads[5] == 16 && m_BenchType[5] == BENCH_RND
	&&  m_BenchSize[8] == 1024 && m_BenchQueues[8] == 8  && m_BenchThreads[8] == 1  && m_BenchType[8] == BENCH_SEQ
	)
	{
		return TRUE;
	}
	return FALSE;
}

/*
BOOL CDiskMarkDlg::IsNVMe9Mode()
{
	if (m_BenchSize[0] == 1024 && m_BenchQueues[0] == 8  && m_BenchThreads[0] == 4  && m_BenchType[0] == BENCH_SEQ
	&&  m_BenchSize[1] == 128  && m_BenchQueues[1] == 32 && m_BenchThreads[1] == 4  && m_BenchType[1] == BENCH_SEQ
	&&  m_BenchSize[2] == 4    && m_BenchQueues[2] == 32 && m_BenchThreads[2] == 16 && m_BenchType[2] == BENCH_RND
	&&  m_BenchSize[3] == 4    && m_BenchQueues[3] == 1  && m_BenchThreads[3] == 1  && m_BenchType[3] == BENCH_RND
	&&  m_BenchSize[4] == 1024 && m_BenchQueues[4] == 8  && m_BenchThreads[4] == 4  && m_BenchType[4] == BENCH_SEQ
	&&  m_BenchSize[5] == 4    && m_BenchQueues[5] == 32 && m_BenchThreads[5] == 16 && m_BenchType[5] == BENCH_RND
	&&  m_BenchSize[8] == 1024 && m_BenchQueues[8] == 8  && m_BenchThreads[8] == 4  && m_BenchType[8] == BENCH_SEQ
	)
	{
		return TRUE;
	}
	return FALSE;
}
*/

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

#ifdef MIX_MODE
	if (PROFILE_DEFAULT > m_Profile || m_Profile > PROFILE_REAL_MIX)
#else
	if (PROFILE_DEFAULT > m_Profile || m_Profile > PROFILE_DEMO)
#endif
	{
		m_Profile = PROFILE_DEFAULT;
	}

	m_Benchmark = GetPrivateProfileInt(L"Setting", L"Benchmark", BENCHMARK_READ_WRITE, m_Ini);
	if (BENCHMARK_READ > m_Benchmark || m_Benchmark > BENCHMARK_READ_WRITE)
	{
		m_Benchmark = BENCHMARK_READ_WRITE;
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

	m_FontRender = GetPrivateProfileInt(L"Setting", L"FontRender", CLEARTYPE_NATURAL_QUALITY, m_Ini);
	if (m_FontRender > CLEARTYPE_NATURAL_QUALITY)
	{
		m_FontRender = CLEARTYPE_NATURAL_QUALITY;
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

	m_WinThread = NULL;
	m_DiskBenchStatus = FALSE;

	InitThemeLang();
	InitMenu();
	UpdateThemeInfo();
	ChangeLang(m_CurrentLang);

	UpdateQueuesThreads();

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

	SetClientSize(m_SizeX, m_SizeY, m_ZoomRatio);

	m_bShowWindow = TRUE;

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
	SetClientSize(m_SizeX, m_SizeY, m_ZoomRatio);

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

	m_TestRead0.SetDrawFrame(m_bHighContrast);
	m_TestRead1.SetDrawFrame(m_bHighContrast);
	m_TestRead2.SetDrawFrame(m_bHighContrast);
	m_TestRead3.SetDrawFrame(m_bHighContrast);
	m_TestWrite0.SetDrawFrame(m_bHighContrast);
	m_TestWrite1.SetDrawFrame(m_bHighContrast);
	m_TestWrite2.SetDrawFrame(m_bHighContrast);
	m_TestWrite3.SetDrawFrame(m_bHighContrast);
	m_Comment.SetDrawFrame(m_bHighContrast);

#ifdef MIX_MODE
	m_TestMix0.SetDrawFrame(m_bHighContrast);
	m_TestMix1.SetDrawFrame(m_bHighContrast);
	m_TestMix2.SetDrawFrame(m_bHighContrast);
	m_TestMix3.SetDrawFrame(m_bHighContrast);
#endif

	if (m_Profile == PROFILE_DEMO)
	{
		m_ReadUnit.ShowWindow(SW_HIDE);
		m_WriteUnit.ShowWindow(SW_HIDE);

		m_TestRead1.ShowWindow(SW_HIDE);
		m_TestRead2.ShowWindow(SW_HIDE);
		m_TestRead3.ShowWindow(SW_HIDE);
		m_TestWrite1.ShowWindow(SW_HIDE);
		m_TestWrite2.ShowWindow(SW_HIDE);
		m_TestWrite3.ShowWindow(SW_HIDE);

		m_DemoSetting.ShowWindow(SW_SHOW);
	}
	else
	{
		m_ReadUnit.ShowWindow(SW_SHOW);
		m_WriteUnit.ShowWindow(SW_SHOW);

		m_TestRead1.ShowWindow(SW_SHOW);
		m_TestRead2.ShowWindow(SW_SHOW);
		m_TestRead3.ShowWindow(SW_SHOW);
		m_TestWrite1.ShowWindow(SW_SHOW);
		m_TestWrite2.ShowWindow(SW_SHOW);
		m_TestWrite3.ShowWindow(SW_SHOW);

		m_DemoSetting.ShowWindow(SW_HIDE);
	}

	if (m_Profile == PROFILE_DEMO)
	{
#ifdef SUISHO_SHIZUKU_SUPPORT
		m_ButtonAll.InitControl(12 + offsetX, 8, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonAll.SetHandCursor(TRUE);
/*
		m_TestRead0.SetGlassColor(m_Glass, m_GlassAlpha);
		m_TestWrite0.SetGlassColor(m_Glass, m_GlassAlpha);

		m_TestRead0.InitControl(12 + offsetX, 96, 384, 348, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawGlass, m_bHighContrast, FALSE);
		m_TestWrite0.InitControl(404 + offsetX, 96, 384, 348, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawGlass, m_bHighContrast, FALSE);
*/
		m_TestRead0.InitControl(12 + offsetX, 96, 384, 348, m_ZoomRatio, &m_BkDC, IP(L"Demo"), 1, SS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestWrite0.InitControl(404 + offsetX, 96, 384, 348, m_ZoomRatio, &m_BkDC, IP(L"Demo"), 1, SS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);

		m_Comment.InitControl(12 + offsetX, 452, 776, 40, m_ZoomRatio, &m_BkDC, IP(L"Comment"), 1, ES_LEFT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_Comment.SetMargin(m_MarginCommentTop, m_MarginCommentLeft, m_MarginCommentBottom, m_MarginCommentRight, m_ZoomRatio);
		m_Comment.Adjust();

		m_DemoSetting.InitControl(140 + offsetX, 56, 528, 40, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);
		m_ReadUnit.InitControl(12 + offsetX, 96, 120, 32, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);
		m_WriteUnit.InitControl(672 + offsetX, 96, 116, 32, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);

		m_ComboCount.InitControl(140 + offsetX, 8, 60, 500, m_ZoomRatio, &m_BkDC, IP(L"Count"), 1, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboSize.InitControl(204 + offsetX, 8, 140, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboDrive.InitControl(348 + offsetX, 8, 320, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboUnit.InitControl(672 + offsetX, 8, 116, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
#else
		m_TestRead0.SetDrawFrameEx(TRUE, m_Frame);
		m_TestWrite0.SetDrawFrameEx(TRUE, m_Frame);

		m_ButtonAll.InitControl(8 + offsetX, 8, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonAll.SetHandCursor(TRUE);

		m_TestRead0.SetGlassColor(m_Glass, m_GlassAlpha);
		m_TestWrite0.SetGlassColor(m_Glass, m_GlassAlpha);

		m_TestRead0.InitControl(8 + offsetX, 64, 228, 196, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawGlass, m_bHighContrast, FALSE);
		m_TestWrite0.InitControl(244 + offsetX, 64, 228, 196, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawGlass, m_bHighContrast, FALSE);

		m_Comment.InitControl(8 + offsetX, 268, 464, 24, m_ZoomRatio, &m_BkDC, IP(L"Comment"), 1, ES_LEFT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_Comment.SetMargin(m_MarginCommentTop, m_MarginCommentLeft, m_MarginCommentBottom, m_MarginCommentRight, m_ZoomRatio);
		m_Comment.Adjust();

		m_DemoSetting.InitControl(84 + offsetX, 36, 320, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);
		m_ReadUnit.InitControl(84 + offsetX, 36, 124, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);
		m_WriteUnit.InitControl(280 + offsetX, 36, 124, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);

		m_ComboCount.InitControl(84 + offsetX, 8, 40, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboSize.InitControl(128 + offsetX, 8, 80, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboDrive.InitControl(212 + offsetX, 8, 188, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboUnit.InitControl(404 + offsetX, 8, 68, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
#endif
	}
	else
	{
		m_TestRead0.SetDrawFrameEx(FALSE, m_Frame);
		m_TestWrite0.SetDrawFrameEx(FALSE, m_Frame);

#ifdef SUISHO_SHIZUKU_SUPPORT
		m_ButtonAll.InitControl(12 + offsetX, 8, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonTest0.InitControl(12 + offsetX, 96, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonTest1.InitControl(12 + offsetX, 184, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonTest2.InitControl(12 + offsetX, 272, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonTest3.InitControl(12 + offsetX, 360, 120, 80, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);

		m_ButtonAll.SetHandCursor(TRUE);
		m_ButtonTest0.SetHandCursor(TRUE);
		m_ButtonTest1.SetHandCursor(TRUE);
		m_ButtonTest2.SetHandCursor(TRUE);
		m_ButtonTest3.SetHandCursor(TRUE);

		m_TestRead0.InitControl(140 + offsetX, 96, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestRead1.InitControl(140 + offsetX, 184, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestRead2.InitControl(140 + offsetX, 272, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestRead3.InitControl(140 + offsetX, 360, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);

		m_TestWrite0.InitControl(468 + offsetX, 96, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestWrite1.InitControl(468 + offsetX, 184, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestWrite2.InitControl(468 + offsetX, 272, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestWrite3.InitControl(468 + offsetX, 360, 320, 80, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);

		m_Comment.InitControl(12 + offsetX, 452, 776, 40, m_ZoomRatio, &m_BkDC, IP(L"Comment"), 1, ES_LEFT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_Comment.SetMargin(m_MarginCommentTop, m_MarginCommentLeft, m_MarginCommentBottom, m_MarginCommentRight, m_ZoomRatio);
		m_Comment.Adjust();

		m_ReadUnit.InitControl(140 + offsetX, 56, 320, 40, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);
		m_WriteUnit.InitControl(468 + offsetX, 56, 320, 40, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);

		m_ComboCount.InitControl(140 + offsetX, 8, 60, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboSize.InitControl(204 + offsetX, 8, 140, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboUnit.InitControl(672 + offsetX, 8, 116, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);

		if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_REAL)
		{
			m_ComboDrive.InitControl(348 + offsetX, 8, 440, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		}
		else
		{
			m_ComboDrive.InitControl(348 + offsetX, 8, 320, 500, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		}
#else

		m_ButtonAll.InitControl(8 + offsetX, 8, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonTest0.InitControl(8 + offsetX, 60, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonTest1.InitControl(8 + offsetX, 112, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonTest2.InitControl(8 + offsetX, 164, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);
		m_ButtonTest3.InitControl(8 + offsetX, 216, 72, 48, m_ZoomRatio, &m_BkDC, IP(L"Button"), 3, BS_CENTER, OwnerDrawImage, m_bHighContrast, FALSE);

		m_ButtonAll.SetHandCursor(TRUE);
		m_ButtonTest0.SetHandCursor(TRUE);
		m_ButtonTest1.SetHandCursor(TRUE);
		m_ButtonTest2.SetHandCursor(TRUE);
		m_ButtonTest3.SetHandCursor(TRUE);

		m_TestRead0.InitControl(84 + offsetX, 60, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestRead1.InitControl(84 + offsetX, 112, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestRead2.InitControl(84 + offsetX, 164, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestRead3.InitControl(84 + offsetX, 216, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);

		m_TestWrite0.InitControl(280 + offsetX, 60, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestWrite1.InitControl(280 + offsetX, 112, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestWrite2.InitControl(280 + offsetX, 164, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		m_TestWrite3.InitControl(280 + offsetX, 216, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
		if (m_MixMode)
		{
			m_Comment.InitControl(8 + offsetX, 268, 664, 24, m_ZoomRatio, &m_BkDC, IP(L"CommentL"), 1, ES_LEFT, OwnerDrawImage, m_bHighContrast, FALSE);
		}
		else
		{
			m_Comment.InitControl(8 + offsetX, 268, 464, 24, m_ZoomRatio, &m_BkDC, IP(L"Comment"), 1, ES_LEFT, OwnerDrawImage, m_bHighContrast, FALSE);
		}
		m_Comment.SetMargin(m_MarginCommentTop, m_MarginCommentLeft, m_MarginCommentBottom, m_MarginCommentRight, m_ZoomRatio);
		m_Comment.Adjust();

		m_ReadUnit.InitControl(84 + offsetX, 36, 192, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);
		m_WriteUnit.InitControl(280 + offsetX, 36, 192, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);

		m_ComboCount.InitControl(84 + offsetX, 8, 40, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboSize.InitControl(128 + offsetX, 8, 80, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		m_ComboUnit.InitControl(404 + offsetX, 8, 68, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);

		if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_REAL || m_Profile == PROFILE_PEAK_MIX || m_Profile == PROFILE_REAL_MIX)
		{
			m_ComboDrive.InitControl(212 + offsetX, 8, 260, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		}
		else
		{
			m_ComboDrive.InitControl(212 + offsetX, 8, 188, 300, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
		}
#endif
	}

	if(m_Profile == PROFILE_DEMO)
	{
		m_TestRead0.SetMargin(m_MarginDemoTop, m_MarginDemoLeft, m_MarginDemoBottom, m_MarginDemoRight, m_ZoomRatio);
		m_TestWrite0.SetMargin(m_MarginDemoTop, m_MarginDemoLeft, m_MarginDemoBottom, m_MarginDemoRight, m_ZoomRatio);
	}
	else
	{
		m_TestRead0.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
		m_TestRead1.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
		m_TestRead2.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
		m_TestRead3.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);

		m_TestWrite0.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
		m_TestWrite1.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
		m_TestWrite2.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
		m_TestWrite3.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
	}

	m_ComboCount.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboSize.SetMargin (0, 4, 0, 0, m_ZoomRatio);
	m_ComboDrive.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboUnit.SetMargin (0, 4, 0, 0, m_ZoomRatio);

#ifdef MIX_MODE
	m_TestMix0.InitControl(480 + offsetX, 60, 192, 48, m_ZoomRatio,  &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestMix1.InitControl(480 + offsetX, 112, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestMix2.InitControl(480 + offsetX, 164, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_TestMix3.InitControl(480 + offsetX, 216, 192, 48, m_ZoomRatio, &m_BkDC, IP(L"Meter"), 2, SS_RIGHT, OwnerDrawImage, m_bHighContrast, FALSE);
	m_ComboMix.InitControl(480 + offsetX, 8, 192, 300, m_ZoomRatio,  &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawGlass, m_bHighContrast, FALSE, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_MixUnit.InitControl(480 + offsetX, 36, 192, 24, m_ZoomRatio,   &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, FALSE);

	m_TestMix0.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
	m_TestMix1.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
	m_TestMix2.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
	m_TestMix3.SetMargin(m_MarginMeterTop, m_MarginMeterLeft, m_MarginMeterBottom, m_MarginMeterRight, m_ZoomRatio);
	m_ComboMix.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	if (m_MixMode)
	{
		m_TestMix0.ShowWindow(SW_SHOW);
		m_TestMix1.ShowWindow(SW_SHOW);
		m_TestMix2.ShowWindow(SW_SHOW);
		m_TestMix3.ShowWindow(SW_SHOW);
		m_ComboMix.ShowWindow(SW_SHOW);
		m_MixUnit.ShowWindow(SW_SHOW);
	}
	else
	{
		m_TestMix0.ShowWindow(SW_HIDE);
		m_TestMix1.ShowWindow(SW_HIDE);
		m_TestMix2.ShowWindow(SW_HIDE);
		m_TestMix3.ShowWindow(SW_HIDE);
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

	if (m_Profile != PROFILE_DEFAULT && m_Profile != PROFILE_DEFAULT_MIX && m_Profile != PROFILE_DEMO)
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
	m_ComboCount.SetToolTipText(i18n(L"Title", L"TEST_COUNT"));
	m_ComboSize.SetToolTipText(i18n(L"Title", L"TEST_SIZE"));
	m_ComboUnit.SetToolTipText(i18n(L"Title", L"TEST_UNIT"));
#ifdef MIX_MODE
	if (m_MixMode)
	{
		m_ComboMix.SetToolTipText(i18n(L"Title", L"TEST_MIX"));
	}
#endif
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
	m_ButtonAll.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);
	m_ButtonTest0.SetFontEx(m_FontFace, 20, 20, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);
	m_ButtonTest1.SetFontEx(m_FontFace, 20, 20, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);
	m_ButtonTest2.SetFontEx(m_FontFace, 20, 20, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);
	m_ButtonTest3.SetFontEx(m_FontFace, 20, 20, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);

	if (m_Profile == PROFILE_DEMO)
	{
		m_TestRead0.SetFontEx(m_FontFace, 80, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
		m_TestWrite0.SetFontEx(m_FontFace,80, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	}
	else
	{
		m_TestRead0.SetFontEx(m_FontFace, 52, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
		m_TestWrite0.SetFontEx(m_FontFace, 52, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	}

	m_TestRead1.SetFontEx(m_FontFace, 52, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_TestRead2.SetFontEx(m_FontFace, 52, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_TestRead3.SetFontEx(m_FontFace, 52, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);

	m_TestWrite1.SetFontEx(m_FontFace, 52, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_TestWrite2.SetFontEx(m_FontFace, 52, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_TestWrite3.SetFontEx(m_FontFace, 52, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);

	m_Comment.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_EditText, FW_BOLD, m_FontRender);

	m_ReadUnit.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD, m_FontRender);
	m_WriteUnit.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD, m_FontRender);
	m_DemoSetting.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);

	m_ComboCount.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboSize.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboDrive.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboUnit.SetFontEx(m_FontFace, 28, 28, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);

	m_ButtonTest0.SetMargin(m_MarginButtonTop, m_MarginButtonLeft, m_MarginButtonBottom, m_MarginButtonRight, m_ZoomRatio);
	m_ButtonTest1.SetMargin(m_MarginButtonTop, m_MarginButtonLeft, m_MarginButtonBottom, m_MarginButtonRight, m_ZoomRatio);
	m_ButtonTest2.SetMargin(m_MarginButtonTop, m_MarginButtonLeft, m_MarginButtonBottom, m_MarginButtonRight, m_ZoomRatio);
	m_ButtonTest3.SetMargin(m_MarginButtonTop, m_MarginButtonLeft, m_MarginButtonBottom, m_MarginButtonRight, m_ZoomRatio);

	m_ComboCount.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
	m_ComboSize.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
	m_ComboDrive.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
	m_ComboUnit.SetItemHeightAll(40, m_ZoomRatio, m_FontRatio);
#else
	m_ButtonAll.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);
	m_ButtonTest0.SetFontEx(m_FontFace, 12, 16, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);
	m_ButtonTest1.SetFontEx(m_FontFace, 12, 16, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);
	m_ButtonTest2.SetFontEx(m_FontFace, 12, 16, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);
	m_ButtonTest3.SetFontEx(m_FontFace, 12, 16, m_ZoomRatio, m_FontRatio, m_ButtonText, FW_BOLD, m_FontRender);

	if (m_Profile == PROFILE_DEMO)
	{
		m_TestRead0.SetFontEx(m_FontFace, 48, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
		m_TestWrite0.SetFontEx(m_FontFace, 48, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	}
	else
	{
		m_TestRead0.SetFontEx(m_FontFace, 35, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
		m_TestWrite0.SetFontEx(m_FontFace, 35, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	}

	m_TestRead1.SetFontEx(m_FontFace, 35, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_TestRead2.SetFontEx(m_FontFace, 35, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_TestRead3.SetFontEx(m_FontFace, 35, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);

	m_TestWrite1.SetFontEx(m_FontFace, 35, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_TestWrite2.SetFontEx(m_FontFace, 35, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_TestWrite3.SetFontEx(m_FontFace, 35, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);

	m_Comment.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_EditText, FW_BOLD, m_FontRender);

	m_ReadUnit.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD, m_FontRender);
	m_WriteUnit.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD, m_FontRender);
	m_DemoSetting.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);

	m_ComboCount.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboSize.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboDrive.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboUnit.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);

	m_ButtonTest0.SetMargin(m_MarginButtonTop, m_MarginButtonLeft, m_MarginButtonBottom, m_MarginButtonRight, m_ZoomRatio);
	m_ButtonTest1.SetMargin(m_MarginButtonTop, m_MarginButtonLeft, m_MarginButtonBottom, m_MarginButtonRight, m_ZoomRatio);
	m_ButtonTest2.SetMargin(m_MarginButtonTop, m_MarginButtonLeft, m_MarginButtonBottom, m_MarginButtonRight, m_ZoomRatio);
	m_ButtonTest3.SetMargin(m_MarginButtonTop, m_MarginButtonLeft, m_MarginButtonBottom, m_MarginButtonRight, m_ZoomRatio);

	m_ComboCount.SetItemHeightAll(24, m_ZoomRatio, m_FontRatio);
	m_ComboSize.SetItemHeightAll(24, m_ZoomRatio, m_FontRatio);
	m_ComboDrive.SetItemHeightAll(24, m_ZoomRatio, m_FontRatio);
	m_ComboUnit.SetItemHeightAll(24, m_ZoomRatio, m_FontRatio);

#ifdef MIX_MODE
	if(m_MixMode)
	{
		m_ComboMix.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
		m_ComboMix.SetItemHeightAll(24, m_ZoomRatio, m_FontRatio);

		m_MixUnit.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio, m_LabelText, FW_BOLD, m_FontRender);

		m_TestMix0.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
		m_TestMix1.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
		m_TestMix2.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
		m_TestMix3.SetFontEx(m_FontFace, 35, 35, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	}
#endif

#endif

}

void CDiskMarkDlg::UpdateQueuesThreads()
{
	CString cstr;

	int type[9] =   {    0,    0,  1, 1,    0,  1,    0, 1,    0 };
	int size[9] =   { 1024, 1024,  4, 4, 1024,  4, 1024, 4, 1024 };
	int queues[9] = {    8,    1, 32, 1,    8, 32,    1, 1,    8 };
	int threads[9] ={    1,    1,  1, 1,    1,  1,    1, 1,    1 };
	int measureTimes[5] = { 5, 10, 20, 30, 60 };
	int intervalTimes[10] = { 0, 1, 3, 5, 10, 30, 60, 180, 300, 600 };

	for (int i = 0; i < 9; i++)
	{
		cstr.Format(L"BenchType%d", i);
		m_BenchType[i] = GetPrivateProfileInt(L"Setting", cstr, type[i], m_Ini);
		if (m_BenchType[i] < 0 || m_BenchSize[i] > 1) { m_BenchSize[i] = type[i]; }

		cstr.Format(L"BenchSize%d", i);
		m_BenchSize[i] = GetPrivateProfileInt(L"Setting", cstr, size[i], m_Ini);
		if (m_BenchSize[i] <= 0 || m_BenchSize[i] > 8192) { m_BenchSize[i] = size[i]; }

		cstr.Format(L"BenchQueues%d", i);
		m_BenchQueues[i] = GetPrivateProfileInt(L"Setting", cstr,queues[i], m_Ini);
		if (m_BenchQueues[i] <= 0 || m_BenchQueues[i] > MAX_QUEUES) { m_BenchQueues[i] = queues[i]; }

		cstr.Format(L"BenchThreads%d", i);
		m_BenchThreads[i] = GetPrivateProfileInt(L"Setting", cstr, threads[i], m_Ini);
		if (m_BenchThreads[i] <= 0 || m_BenchThreads[i] > MAX_THREADS) { m_BenchThreads[i] = threads[i]; }
	}

	m_TestData = GetPrivateProfileInt(L"Setting", L"TestData", TEST_DATA_RANDOM, m_Ini);
	if (m_TestData < 0 || m_TestData > 1)
	{
		m_TestData = TEST_DATA_RANDOM;
	}
	SetWindowTitle(L"");

	BOOL bMeasureflag = FALSE;
	m_MeasureTime = GetPrivateProfileInt(L"Setting", L"MeasureTime", 5, m_Ini);
	for (int i = 0; i < 5; i++)
	{
		if (m_MeasureTime == measureTimes[i])
		{
			bMeasureflag = TRUE;
		}
	}
	if (! bMeasureflag)
	{
		m_MeasureTime = 5;
	}

	BOOL bIntervalFlag = FALSE;
	m_IntervalTime = GetPrivateProfileInt(L"Setting", L"IntervalTime", 5, m_Ini);
	for (int i = 0; i < 10; i++)
	{
		if (m_IntervalTime == intervalTimes[i])
		{
			bIntervalFlag = TRUE;
		}
	}
	if (! bIntervalFlag)
	{
		m_IntervalTime = 5;
	}

	CheckRadioPresetMode();
}

void CDiskMarkDlg::SettingsQueuesThreads(int type)
{
	CString key, value;

	switch (type)
	{
	case 0:// Default
		{
			int type[9] =   {    0,    0,  1, 1,    0,  1,    0, 1,    0 };
			int size[9] =   { 1024, 1024,  4, 4, 1024,  4, 1024, 4, 1024 };
			int queues[9] = {    8,    1, 32, 1,    8, 32,    1, 1,    8 };
			int threads[9] ={    1,    1,  1, 1,    1,  1,    1, 1,    1 };

			for (int i = 0; i < 9; i++)
			{
				key.Format(L"BenchType%d", i); value.Format(L"%d", type[i]);
				WritePrivateProfileString(L"Setting", key, value, m_Ini);
				key.Format(L"BenchSize%d", i); value.Format(L"%d", size[i]);
				WritePrivateProfileString(L"Setting", key, value, m_Ini);
				key.Format(L"BenchQueues%d", i); value.Format(L"%d",queues[i]);
				WritePrivateProfileString(L"Setting", key, value, m_Ini);
				key.Format(L"BenchThreads%d", i); value.Format(L"%d", threads[i]);
				WritePrivateProfileString(L"Setting", key, value, m_Ini);
			}
			WritePrivateProfileString(L"Setting", L"Affinity", L"1", m_Ini);
		}
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);
		break;
	case 1: // NVMe SSD Ver.8
		{
			int type[9] =    {    0,    0,  1, 1,    0,  1,    0, 1,    0 };
			int size[9] =    { 1024,  128,  4, 4, 1024,  4, 1024, 4, 1024 };
			int queues[9] =  {    8,   32, 32, 1,    8, 32,    1, 1,    8 };
			int threads[9] = {    1,    1, 16, 1,    1, 16,    1, 1,    1 };
			for (int i = 0; i < 9; i++)
			{
				key.Format(L"BenchType%d", i); value.Format(L"%d", type[i]);
				WritePrivateProfileString(L"Setting", key, value, m_Ini);
				key.Format(L"BenchSize%d", i); value.Format(L"%d", size[i]);
				WritePrivateProfileString(L"Setting", key, value, m_Ini);
				key.Format(L"BenchQueues%d", i); value.Format(L"%d", queues[i]);
				WritePrivateProfileString(L"Setting", key, value, m_Ini);
				key.Format(L"BenchThreads%d", i); value.Format(L"%d", threads[i]);
				WritePrivateProfileString(L"Setting", key, value, m_Ini);
			}
			WritePrivateProfileString(L"Setting", L"Affinity", L"1", m_Ini);
		}
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);
		break;
	/*
	case 2: // NVMe SSD Ver.9
	{
		int type[9] =    {    0,    0,  1, 1,    0,  1,    0, 1,    0 };
		int size[9] =    { 1024,  128,  4, 4, 1024,  4, 1024, 4, 1024 };
		int queues[9] =  {    8,   32, 32, 1,    8, 32,    1, 1,    8 };
		int threads[9] = {    4,    4, 16, 1,    4, 16,    1, 1,    4 };
		for (int i = 0; i < 9; i++)
		{
			key.Format(L"BenchType%d", i); value.Format(L"%d", type[i]);
			WritePrivateProfileString(L"Setting", key, value, m_Ini);
			key.Format(L"BenchSize%d", i); value.Format(L"%d", size[i]);
			WritePrivateProfileString(L"Setting", key, value, m_Ini);
			key.Format(L"BenchQueues%d", i); value.Format(L"%d", queues[i]);
			WritePrivateProfileString(L"Setting", key, value, m_Ini);
			key.Format(L"BenchThreads%d", i); value.Format(L"%d", threads[i]);
			WritePrivateProfileString(L"Setting", key, value, m_Ini);
		}
		WritePrivateProfileString(L"Setting", L"Affinity", L"0", m_Ini);
	}
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);
	break;
	*/
	default:
		OnSettingsQueuesThreads();
		break;
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
				m_ComboDrive.SetToolTipText(i18n(L"Title", L"TEST_DRIVE"));

			}
			g_pMalloc->Release();
		}
	}
	else
	{
		m_ComboDrive.SetToolTipText(i18n(L"Title", L"TEST_DRIVE"));
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
	for (int i = 0; i < 9; i++)
	{
		m_ReadScore[i] = 0.0;
		m_ReadLatency[i] = 0.0;
		m_WriteScore[i] = 0.0;
		m_WriteLatency[i] = 0.0;
#ifdef MIX_MODE
		m_MixScore[i] = 0.0;
		m_MixLatency[i] = 0.0;
#endif
	}

	UpdateScore();
}

void CDiskMarkDlg::UpdateScore()
{
	UpdateData(TRUE);
	if (m_Profile == PROFILE_DEMO)
	{
		SetMeter(&m_TestRead0, m_ReadScore[8], m_ReadLatency[8], m_BenchSize[8] * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite0, m_WriteScore[8], m_WriteLatency[8], m_BenchSize[8] * 1024, m_IndexTestUnit);
	}
	else if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		SetMeter(&m_TestRead0, m_ReadScore[4], m_ReadLatency[4], m_BenchSize[4] * 1024, SCORE_MBS);
		SetMeter(&m_TestRead1, m_ReadScore[5], m_ReadLatency[5], m_BenchSize[5] * 1024, SCORE_MBS);
		SetMeter(&m_TestRead2, m_ReadScore[5], m_ReadLatency[5], m_BenchSize[5] * 1024, SCORE_IOPS);
		SetMeter(&m_TestRead3, m_ReadScore[5], m_ReadLatency[5], m_BenchSize[5] * 1024, SCORE_US);
		SetMeter(&m_TestWrite0, m_WriteScore[4], m_WriteLatency[4], m_BenchSize[4] * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite1, m_WriteScore[5], m_WriteLatency[5], m_BenchSize[5] * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite2, m_WriteScore[5], m_WriteLatency[5], m_BenchSize[5] * 1024, SCORE_IOPS);
		SetMeter(&m_TestWrite3, m_WriteScore[5], m_WriteLatency[5], m_BenchSize[5] * 1024, SCORE_US);
#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix0, m_MixScore[4], m_MixLatency[4], m_BenchSize[4] * 1024, SCORE_MBS);
			SetMeter(&m_TestMix1, m_MixScore[5], m_MixLatency[5], m_BenchSize[5] * 1024, SCORE_MBS);
			SetMeter(&m_TestMix2, m_MixScore[5], m_MixLatency[5], m_BenchSize[5] * 1024, SCORE_IOPS);
			SetMeter(&m_TestMix3, m_MixScore[5], m_MixLatency[5], m_BenchSize[5] * 1024, SCORE_US);
		}
#endif
	}
	else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		SetMeter(&m_TestRead0, m_ReadScore[6], m_ReadLatency[6], 1024 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead1, m_ReadScore[7], m_ReadLatency[7], 4 * 1024, SCORE_MBS);
		SetMeter(&m_TestRead2, m_ReadScore[7], m_ReadLatency[7], 4 * 1024, SCORE_IOPS);
		SetMeter(&m_TestRead3, m_ReadScore[7], m_ReadLatency[7], 4 * 1024, SCORE_US);
		SetMeter(&m_TestWrite0, m_WriteScore[6], m_WriteLatency[6], 1024 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite1, m_WriteScore[7], m_WriteLatency[7], 4 * 1024, SCORE_MBS);
		SetMeter(&m_TestWrite2, m_WriteScore[7], m_WriteLatency[7], 4 * 1024, SCORE_IOPS);
		SetMeter(&m_TestWrite3, m_WriteScore[7], m_WriteLatency[7], 4 * 1024, SCORE_US);
#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix0, m_MixScore[6], m_MixLatency[6], 1024 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix1, m_MixScore[7], m_MixLatency[7], 4 * 1024, SCORE_MBS);
			SetMeter(&m_TestMix2, m_MixScore[7], m_MixLatency[7], 4 * 1024, SCORE_IOPS);
			SetMeter(&m_TestMix3, m_MixScore[7], m_MixLatency[7], 4 * 1024, SCORE_US);
		}
#endif
	}
	else
	{
		SetMeter(&m_TestRead0, m_ReadScore[0], m_ReadLatency[0], m_BenchSize[0] * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead1, m_ReadScore[1], m_ReadLatency[1], m_BenchSize[1] * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead2, m_ReadScore[2], m_ReadLatency[2], m_BenchSize[2] * 1024, m_IndexTestUnit);
		SetMeter(&m_TestRead3, m_ReadScore[3], m_ReadLatency[3], m_BenchSize[3] * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite0, m_WriteScore[0], m_WriteLatency[0], m_BenchSize[0] * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite1, m_WriteScore[1], m_WriteLatency[1], m_BenchSize[1] * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite2, m_WriteScore[2], m_WriteLatency[2], m_BenchSize[2] * 1024, m_IndexTestUnit);
		SetMeter(&m_TestWrite3, m_WriteScore[3], m_WriteLatency[3], m_BenchSize[3] * 1024, m_IndexTestUnit);
#ifdef MIX_MODE
		if (m_MixMode)
		{
			SetMeter(&m_TestMix0, m_MixScore[0], m_MixLatency[0], m_BenchSize[0] * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix1, m_MixScore[1], m_MixLatency[1], m_BenchSize[1] * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix2, m_MixScore[2], m_MixLatency[2], m_BenchSize[2] * 1024, m_IndexTestUnit);
			SetMeter(&m_TestMix3, m_MixScore[3], m_MixLatency[3], m_BenchSize[3] * 1024, m_IndexTestUnit);
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

		m_ReadScore[4] = 0.0;
		m_WriteScore[4] = 0.0;
		m_ReadLatency[4] = 0.0;
		m_WriteLatency[4] = 0.0;
#ifdef MIX_MODE
		m_MixScore[4] = 0.0;
		m_MixLatency[4] = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBench4, (void*)this);
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

		m_ReadScore[5] = 0.0;
		m_WriteScore[5] = 0.0;
		m_ReadLatency[5] = 0.0;
		m_WriteLatency[5] = 0.0;
#ifdef MIX_MODE
		m_MixScore[5] = 0.0;
		m_MixLatency[5] = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBench5, (void*)this);
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

		m_ReadScore[6] = 0.0;
		m_WriteScore[6] = 0.0;
		m_ReadLatency[6] = 0.0;
		m_WriteLatency[6] = 0.0;
#ifdef MIX_MODE
		m_MixScore[6] = 0.0;
		m_MixLatency[6] = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBench6, (void*)this);
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

		m_ReadScore[7] = 0.0;
		m_WriteScore[7] = 0.0;
		m_ReadLatency[7] = 0.0;
		m_WriteLatency[7] = 0.0;
#ifdef MIX_MODE
		m_MixScore[7] = 0.0;
		m_MixLatency[7] = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBench7, (void*)this);
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

void CDiskMarkDlg::OnTest0()
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

		m_ReadScore[0] = 0.0;
		m_WriteScore[0] = 0.0;
		m_ReadLatency[0] = 0.0;
		m_WriteLatency[0] = 0.0;
#ifdef MIX_MODE
		m_MixScore[0] = 0.0;
		m_MixLatency[0] = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBench0, (void*)this);
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

void CDiskMarkDlg::OnTest1()
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

		m_ReadScore[1] = 0.0;
		m_WriteScore[1] = 0.0;
		m_ReadLatency[1] = 0.0;
		m_WriteLatency[1] = 0.0;
#ifdef MIX_MODE
		m_MixScore[1] = 0.0;
		m_MixLatency[1] = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBench1, (void*)this);
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

	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);

		m_ReadScore[2] = 0.0;
		m_WriteScore[2] = 0.0;
		m_ReadLatency[2] = 0.0;
		m_WriteLatency[2] = 0.0;
#ifdef MIX_MODE
		m_MixScore[2] = 0.0;
		m_MixLatency[2] = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBench2, (void*)this);
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

		m_ReadScore[3] = 0.0;
		m_WriteScore[3] = 0.0;
		m_ReadLatency[3] = 0.0;
		m_WriteLatency[3] = 0.0;
#ifdef MIX_MODE
		m_MixScore[3] = 0.0;
		m_MixLatency[3] = 0.0;
#endif
		UpdateScore();
		m_DiskBenchStatus = TRUE;
		m_WinThread = AfxBeginThread(ExecDiskBench3, (void*)this);
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
		if (m_Profile == PROFILE_DEMO)
		{
			m_WinThread = AfxBeginThread(ExecDiskBenchAllDemo, (void*)this);
		}
		else if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
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

CString CDiskMarkDlg::GetButtonText(int type, int size, int queues, int threads, int unit)
{
	CString text;

	if (size >= 1024)
	{
		if (type == BENCH_RND)
		{
			if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX || m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
			{
				if (unit == SCORE_IOPS)
				{
					text.Format(L"RND%dM\r\n(IOPS)", size / 1024);
				}
				else if (unit == SCORE_US)
				{
					text.Format(L"RND%dM\r\n(μs)", size / 1024);
				}
				else if (unit == SCORE_GBS)
				{
					text.Format(L"RND%dM\r\nQ%dT%d", size / 1024, queues, threads);
				}
				else
				{
					text.Format(L"RND%dM\r\nQ%dT%d", size / 1024, queues, threads);
				}
			}
			else
			{
				if (unit == SCORE_GBS)
				{
					text.Format(L"RND%dM\r\nQ%dT%d", size / 1024, queues, threads);
				}
				else
				{
					text.Format(L"RND%dM\r\nQ%dT%d", size / 1024, queues, threads);
				}
			}
		}
		else
		{
			if (unit == SCORE_GBS)
			{
				text.Format(L"SEQ%dM\r\nQ%dT%d", size / 1024, queues, threads);
			}
			else
			{
				text.Format(L"SEQ%dM\r\nQ%dT%d", size / 1024, queues, threads);
			}
		}
	}
	else
	{
		if (type == BENCH_RND)
		{
			if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX || m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
			{
				if (unit == SCORE_IOPS)
				{
					text.Format(L"RND%dK\r\n(IOPS)", size);
				}
				else if (unit == SCORE_US)
				{
					text.Format(L"RND%dK\r\n(μs)", size);
				}
				else if (unit == SCORE_GBS)
				{
					text.Format(L"RND%dK\r\nQ%dT%d", size, queues, threads);
				}
				else
				{
					text.Format(L"RND%dK\r\nQ%dT%d", size, queues, threads);
				}
			}
			else
			{
				if (unit == SCORE_GBS)
				{
					text.Format(L"RND%dK\r\nQ%dT%d", size, queues, threads);
				}
				else
				{
					text.Format(L"RND%dK\r\nQ%dT%d", size, queues, threads);
				}
			}
		}
		else
		{
			if (unit == SCORE_GBS)
			{
				text.Format(L"SEQ%dK\r\nQ%dT%d", size, queues, threads);
			}
			else
			{
				text.Format(L"SEQ%dK\r\nQ%dT%d", size, queues, threads);
			}
		}
	}

	return text;
}

CString CDiskMarkDlg::GetButtonToolTipText(int type, int size, int queues, int threads, int unit)
{
	CString text;

	if (size >= 1024)
	{
		if (type == BENCH_RND)
		{
			if (unit == SCORE_IOPS)
			{
				text.Format(L"Random %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(IOPS)", size / 1024, queues, threads);
			}
			else if (unit == SCORE_US)
			{
				text.Format(L"Random %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(μs)", size / 1024, queues, threads);
			}
			else if (unit == SCORE_GBS)
			{
				text.Format(L"Random %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(GB/s)", size / 1024, queues, threads);
			}
			else
			{
				text.Format(L"Random %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", size / 1024, queues, threads);
			}
		}
		else
		{
			if (unit == SCORE_GBS)
			{
				text.Format(L"Sequential %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(GB/s)", size / 1024, queues, threads);
			}
			else
			{
				text.Format(L"Sequential %dMiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", size / 1024, queues, threads);
			}
		}
	}
	else
	{
		if (type == BENCH_RND)
		{
			if (unit == SCORE_IOPS)
			{
				text.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(IOPS)", size, queues, threads);
			}
			else if (unit == SCORE_US)
			{
				text.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(μs)", size, queues, threads);
			}
			else if (unit == SCORE_GBS)
			{
				text.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(GB/s)", size, queues, threads);
			}
			else
			{
				text.Format(L"Random %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", size, queues, threads);
			}
		}
		else
		{
			if (unit == SCORE_GBS)
			{
				text.Format(L"Sequential %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(GB/s)", size, queues, threads);
			}
			else
			{
				text.Format(L"Sequential %dKiB\r\nQueues=%d\r\nThreads=%d\r\n(MB/s)", size, queues, threads);
			}
		}
	}

	return text;
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

		m_ButtonAll.SetWindowTextW(L"All");

		if (m_Profile == PROFILE_DEMO)
		{
			m_ButtonTest0.ShowWindow(SW_HIDE);
			m_ButtonTest1.ShowWindow(SW_HIDE);
			m_ButtonTest2.ShowWindow(SW_HIDE);
			m_ButtonTest3.ShowWindow(SW_HIDE);

			CString text, type;
			if (m_BenchType[8] == BENCH_SEQ)
			{
				type = L"SEQ";
			}
			else
			{
				type = L"RND";
			}
			if (m_BenchSize[8] > 1000)
			{
				text.Format(L"%s %dMiB, Q=%d, T=%d", type.GetString(), m_BenchSize[8] / 1024, m_BenchQueues[8], m_BenchThreads[8]);
			}
			else
			{
				text.Format(L"%s %dKiB, Q=%d, T=%d", type.GetString(), m_BenchSize[8], m_BenchQueues[8], m_BenchThreads[8]);
			}

			m_DemoSetting.SetWindowTextW(text);
		}
		else if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
		{
			m_ButtonTest0.ShowWindow(SW_SHOW);
			m_ButtonTest1.ShowWindow(SW_SHOW);
			m_ButtonTest2.ShowWindow(SW_SHOW);
			m_ButtonTest3.ShowWindow(SW_SHOW);

			m_ButtonTest0.SetWindowTextW(GetButtonText(BENCH_SEQ, m_BenchSize[4], m_BenchQueues[4], m_BenchThreads[4], SCORE_MBS));
			m_ButtonTest1.SetWindowTextW(GetButtonText(BENCH_RND, m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5], SCORE_MBS));
			m_ButtonTest2.SetWindowTextW(GetButtonText(BENCH_RND, m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5], SCORE_IOPS));
			m_ButtonTest3.SetWindowTextW(GetButtonText(BENCH_RND, m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5], SCORE_US));

			m_ButtonTest0.SetToolTipText(GetButtonToolTipText(BENCH_SEQ, m_BenchSize[4], m_BenchQueues[4], m_BenchThreads[4], SCORE_MBS));
			m_ButtonTest1.SetToolTipText(GetButtonToolTipText(BENCH_RND, m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5], SCORE_MBS));
			m_ButtonTest2.SetToolTipText(GetButtonToolTipText(BENCH_RND, m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5], SCORE_IOPS));
			m_ButtonTest3.SetToolTipText(GetButtonToolTipText(BENCH_RND, m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5], SCORE_US));
		}
		else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
		{
			m_ButtonTest0.ShowWindow(SW_SHOW);
			m_ButtonTest1.ShowWindow(SW_SHOW);
			m_ButtonTest2.ShowWindow(SW_SHOW);
			m_ButtonTest3.ShowWindow(SW_SHOW);

			m_ButtonTest0.SetWindowTextW(GetButtonText(BENCH_SEQ, 1024, 1, 1, SCORE_MBS));
			m_ButtonTest1.SetWindowTextW(GetButtonText(BENCH_RND, 4, 1, 1, SCORE_MBS));
			m_ButtonTest2.SetWindowTextW(GetButtonText(BENCH_RND, 4, 1, 1, SCORE_IOPS));
			m_ButtonTest3.SetWindowTextW(GetButtonText(BENCH_RND, 4, 1, 1, SCORE_US));

			m_ButtonTest0.SetToolTipText(GetButtonToolTipText(BENCH_SEQ, 1024, 1, 1, SCORE_MBS));
			m_ButtonTest1.SetToolTipText(GetButtonToolTipText(BENCH_RND, 4, 1, 1, SCORE_MBS));
			m_ButtonTest2.SetToolTipText(GetButtonToolTipText(BENCH_RND, 4, 1, 1, SCORE_IOPS));
			m_ButtonTest3.SetToolTipText(GetButtonToolTipText(BENCH_RND, 4, 1, 1, SCORE_US));
		}
		else
		{
			m_ButtonTest0.ShowWindow(SW_SHOW);
			m_ButtonTest1.ShowWindow(SW_SHOW);
			m_ButtonTest2.ShowWindow(SW_SHOW);
			m_ButtonTest3.ShowWindow(SW_SHOW);

			m_ButtonTest0.SetWindowTextW(GetButtonText(m_BenchType[0], m_BenchSize[0], m_BenchQueues[0], m_BenchThreads[0], m_IndexTestUnit));
			m_ButtonTest1.SetWindowTextW(GetButtonText(m_BenchType[1], m_BenchSize[1], m_BenchQueues[1], m_BenchThreads[1], m_IndexTestUnit));
			m_ButtonTest2.SetWindowTextW(GetButtonText(m_BenchType[2], m_BenchSize[2], m_BenchQueues[2], m_BenchThreads[2], m_IndexTestUnit));
			m_ButtonTest3.SetWindowTextW(GetButtonText(m_BenchType[3], m_BenchSize[3], m_BenchQueues[3], m_BenchThreads[3], m_IndexTestUnit));

			m_ButtonTest0.SetToolTipText(GetButtonToolTipText(m_BenchType[0], m_BenchSize[0], m_BenchQueues[0], m_BenchThreads[0], m_IndexTestUnit));
			m_ButtonTest1.SetToolTipText(GetButtonToolTipText(m_BenchType[1], m_BenchSize[1], m_BenchQueues[1], m_BenchThreads[1], m_IndexTestUnit));
			m_ButtonTest2.SetToolTipText(GetButtonToolTipText(m_BenchType[2], m_BenchSize[2], m_BenchQueues[2], m_BenchThreads[2], m_IndexTestUnit));
			m_ButtonTest3.SetToolTipText(GetButtonToolTipText(m_BenchType[3], m_BenchSize[3], m_BenchQueues[3], m_BenchThreads[3], m_IndexTestUnit));
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

		m_ButtonAll.SetWindowTextW(L"Stop");
		m_ButtonTest0.SetWindowTextW(L"Stop");
		m_ButtonTest1.SetWindowTextW(L"Stop");
		m_ButtonTest2.SetWindowTextW(L"Stop");
		m_ButtonTest3.SetWindowTextW(L"Stop");
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
		if (m_Profile == PROFILE_DEMO)
		{
			if (iops >= 100000.0)
			{
				cstr.Format(L"%dk", (int)iops / 1000);
			}
			else
			{
				cstr.Format(L"%d", (int)iops);
			}
		}
		else
		{
			if (iops >= 1000000.0)
			{
				cstr.Format(L"%d", (int)iops);
			}
			else
			{
				cstr.Format(L"%.2f", iops);
			}
		}
	}
	else if (unit == SCORE_UNIT::SCORE_US)
	{
		if (m_Profile == PROFILE_DEMO)
		{
			if (score <= 0.0)
			{
				cstr.Format(L"%.1f", 0.0);
				if (*control == m_TestRead0) { m_TestRead0.SetLabelUnit(L"Read", L"μs"); }
				if (*control == m_TestWrite0) { m_TestWrite0.SetLabelUnit(L"Write", L"μs"); }
			}
			else if (latency >= 1000000.0)
			{
				cstr.Format(L"%d", (int)latency / 1000);
				if (*control == m_TestRead0) { m_TestRead0.SetLabelUnit(L"Read", L"ms"); }
				if (*control == m_TestWrite0) { m_TestWrite0.SetLabelUnit(L"Write", L"ms"); }
			}
			else if (latency >= 1000.0)
			{
				cstr.Format(L"%d", (int)latency);
				if (*control == m_TestRead0) { m_TestRead0.SetLabelUnit(L"Read", L"μs"); }
				if (*control == m_TestWrite0) { m_TestWrite0.SetLabelUnit(L"Write", L"μs"); }
			}
			else
			{
				cstr.Format(L"%.1f", latency);
				if (*control == m_TestRead0) { m_TestRead0.SetLabelUnit(L"Read", L"μs"); }
				if (*control == m_TestWrite0) { m_TestWrite0.SetLabelUnit(L"Write", L"μs"); }
			}
		}
		else
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
	}
	else if (unit == SCORE_UNIT::SCORE_GBS)
	{
		if (m_Profile == PROFILE_DEMO)
		{
			cstr.Format(L"%.1f", score / 1000.0);
		}
		else
		{
			cstr.Format(L"%.3f", score / 1000.0);
		}
	}
	else
	{
		if (m_Profile == PROFILE_DEMO)
		{
			if (score >= 1000.0)
			{
				cstr.Format(L"%d", (int)score);
			}
			else
			{
				cstr.Format(L"%.1f", score);
			}
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
	}

	UpdateData(FALSE);
	if (m_Profile == PROFILE_DEMO)
	{
		control->SetMeter(FALSE, meterRatio);
	}
	else
	{
		control->SetMeter(TRUE, meterRatio);
	}
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
	m_TestDriveLetter = GetPrivateProfileInt(L"Setting", L"DriveLetter", 2, m_Ini); // Default "C:\"

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
			freeBytesAvailableToCaller.QuadPart = 0;
			totalNumberOfBytes.QuadPart = 0;
			totalNumberOfFreeBytes.QuadPart = 0;
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
	    
	TCHAR str[256];
	GetPrivateProfileString(L"Setting", L"TargetPath", L"", str, 256, m_Ini);
	m_TestTargetPath = str;

	if (m_TestDriveLetter == 99)
	{
		m_IndexTestDrive = count;
	}
	m_MaxIndexTestDrive = count;

	UpdateDriveToolTip();

	UpdateData(FALSE);
}

void CDiskMarkDlg::UpdateDriveToolTip()
{
	m_ComboDrive.SetCurSel(m_IndexTestDrive);
	if (m_TestDriveLetter == 99 && !m_TestTargetPath.IsEmpty())
	{
		m_ComboDrive.SetToolTipText(m_TestTargetPath);
	}
	else
	{
		m_ComboDrive.SetToolTipText(i18n(L"Title", L"TEST_DRIVE"));
	}
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

	subMenu.Attach(menu->GetSubMenu(1)->GetSafeHmenu());
	cstr = i18n(L"Menu", L"TEST_DATA");
	subMenu.ModifyMenu(0, MF_BYPOSITION, 0, cstr);
	subMenu.Detach();

	cstr = i18n(L"Menu", L"DEFAULT_RANDOM");
	menu->ModifyMenu(ID_MODE_DEFAULT, MF_STRING, ID_MODE_DEFAULT, cstr);
	cstr = i18n(L"Menu", L"ALL_ZERO");
	menu->ModifyMenu(ID_MODE_ALL0X00, MF_STRING, ID_MODE_ALL0X00, cstr);

	if (m_TestData == TEST_DATA_ALL0X00)
	{
		OnModeAll0x00();
	}
	else
	{
		OnModeDefault();
	}

	cstr = i18n(L"Dialog", L"DEFAULT");
	menu->ModifyMenu(ID_SETTING_DEFAULT, MF_STRING, ID_SETTING_DEFAULT, cstr);

	CheckRadioPresetMode();

	cstr = i18n(L"Menu", L"SETTINGS") + L"\tCtrl + Q";
	menu->ModifyMenu(ID_SETTINGS_QUEUESTHREADS, MF_STRING, ID_SETTINGS_QUEUESTHREADS, cstr);

	cstr = i18n(L"Menu", L"PROFILE_DEFAULT");
	menu->ModifyMenu(ID_PROFILE_DEFAULT, MF_STRING, ID_PROFILE_DEFAULT, cstr);
	cstr = i18n(L"Menu", L"PROFILE_PEAK");
	menu->ModifyMenu(ID_PROFILE_PEAK, MF_STRING, ID_PROFILE_PEAK, cstr);
	cstr = i18n(L"Menu", L"PROFILE_REAL");
	menu->ModifyMenu(ID_PROFILE_REAL, MF_STRING, ID_PROFILE_REAL, cstr);
	cstr = i18n(L"Menu", L"PROFILE_DEMO");
	menu->ModifyMenu(ID_PROFILE_DEMO, MF_STRING, ID_PROFILE_DEMO, cstr);

#ifdef MIX_MODE
	cstr = i18n(L"Menu", L"PROFILE_DEFAULT") + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_DEFAULT_MIX, MF_STRING, ID_PROFILE_DEFAULT_MIX, cstr);
	cstr = i18n(L"Menu", L"PROFILE_PEAK") + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_PEAK_MIX, MF_STRING, ID_PROFILE_PEAK_MIX, cstr);
	cstr = i18n(L"Menu", L"PROFILE_REAL") + L" [+Mix]";
	menu->ModifyMenu(ID_PROFILE_REAL_MIX, MF_STRING, ID_PROFILE_REAL_MIX, cstr);
#endif

	cstr = i18n(L"Menu", L"HELP") + L" [Web]" + L"\tF1";
	menu->ModifyMenu(ID_HELP, MF_STRING, ID_HELP, cstr);
	cstr = i18n(L"Menu", L"HELP_ABOUT");
	menu->ModifyMenu(ID_ABOUT, MF_STRING, ID_ABOUT, cstr);

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
	case PROFILE_DEMO:
		ProfileDemo();
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
		ProfileDefault();
		break;
	}

	switch (m_Benchmark)
	{
	case BENCHMARK_READ_WRITE:
		BenchmarkReadWrite();
		break;
	case BENCHMARK_READ:
		BenchmarkReadOnly();
		break;
	case BENCHMARK_WRITE:
		BenchmarkWriteOnly();
		break;
	default:
		BenchmarkReadWrite();
		break;
	}

	SetMenu(menu);

	m_MesStopBenchmark = i18n(L"Message", L"STOP_BENCHMARK");
	m_MesDiskCapacityError = i18n(L"Message", L"DISK_CAPACITY_ERROR");
	m_MesDiskCreateFileError = i18n(L"Message", L"DISK_CREATE_FILE_ERROR");
	m_MesDiskWriteError = i18n(L"Message", L"DISK_WRITE_ERROR");
	m_MesDiskReadError = i18n(L"Message", L"DISK_READ_ERROR");
	m_MesDiskSpdNotFound = i18n(L"Message", L"DISK_SPD_NOT_FOUND");

	UpdateDriveToolTip();

	WritePrivateProfileString(L"Setting", L"Language", LangName, m_Ini);
}

BOOL CDiskMarkDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// Select Theme
	if (WM_THEME_ID <= wParam && wParam < WM_THEME_ID + (UINT)m_MenuArrayTheme.GetSize())
	{
		CMenu menu;
		CMenu subMenu;
		menu.Attach(GetMenu()->GetSafeHmenu());
		subMenu.Attach(menu.GetSubMenu(MENU_THEME_INDEX)->GetSafeHmenu());

		m_CurrentTheme = m_MenuArrayTheme.GetAt(wParam - WM_THEME_ID);
		if (m_CurrentTheme.Compare(m_RandomThemeLabel) == 0)
		{
			m_CurrentTheme = GetRandomTheme();
			m_RandomThemeLabel = L"Random";
			m_RandomThemeName = L" (" + m_CurrentTheme + L")";

			// ChangeTheme save the theme configuration to profile; so if we are on
			// Random, then save Random to profile.
			ChangeTheme(m_RandomThemeLabel);
		}
		else
		{
			ChangeTheme(m_MenuArrayTheme.GetAt(wParam - WM_THEME_ID));
			m_RandomThemeName = L"";
		}

		subMenu.ModifyMenu(WM_THEME_ID, MF_STRING, WM_THEME_ID, m_RandomThemeLabel + m_RandomThemeName);
		subMenu.CheckMenuRadioItem(WM_THEME_ID, WM_THEME_ID + (UINT)m_MenuArrayTheme.GetSize(),
			(UINT)wParam, MF_BYCOMMAND);
		subMenu.Detach();
		menu.Detach();

		if (m_Profile == PROFILE_DEMO && IsFileExist(m_ThemeDir + m_CurrentTheme + L"\\BackgroundDemo-300.png"))
		{
			m_BackgroundName = L"BackgroundDemo";
		}
		else
		{
			m_BackgroundName = L"Background";
		}

		UpdateThemeInfo();
		UpdateDialogSize();

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
	path.Format(L"%s_%04d%02d%02d%02d%02d%02d", PRODUCT_NAME, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

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

CString CDiskMarkDlg::GetResultString(int type, double score, double latency, int size, int queues, int threads)
{
	CString result;
	double iops = 0.0;

	iops = score * 1000 * 1000 / ((double)size * 1024);
	if (latency < 0.0)
	{
		latency = 0.0;
	}

	if (type == BENCH_RND)
	{
		if (size >= 1024)
		{
			result.Format(L"  RND %4dMiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size / 1024, queues, threads, score, iops, latency);
		}
		else
		{
			result.Format(L"  RND %4dKiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size, queues, threads, score, iops, latency);
		}
	}
	else
	{
		if (size >= 1024)
		{
			result.Format(L"  SEQ %4dMiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size / 1024, queues, threads, score, iops, latency);
		}
		else
		{
			result.Format(L"  SEQ %4dKiB (Q=%3d, T=%2d): %9.3f MB/s [%9.1f IOPS] <%9.2f us>", size, queues, threads, score, iops, latency);
		}
	}

	return result;
}

void CDiskMarkDlg::SaveText(CString fileName)
{
	CString cstr, clip;

	UpdateData(TRUE);

	if (m_Profile == PROFILE_DEMO)
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
%BenchRead1%\r\n\
\r\n\
[Write]\r\n\
%BenchWrite1%\r\n\
\r\n\
Profile: Demo\r\n\
   Test: %TestSize% (x%TestCount%)%Capacity%\r\n\
   Mode:%TestMode%\r\n\
   Time: Measure %MeasureTime% / Interval %IntervalTime% \r\n\
   Date: %Date%\r\n\
     OS: %OS%\r\n\
%Comment%";
	}
	else if (m_Profile == PROFILE_DEFAULT || m_Profile == PROFILE_DEFAULT_MIX)
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
%BenchRead1%\r\n\
%BenchRead2%\r\n\
%BenchRead3%\r\n\
%BenchRead4%\r\n\
\r\n\
[Write]\r\n\
%BenchWrite1%\r\n\
%BenchWrite2%\r\n\
%BenchWrite3%\r\n\
%BenchWrite4%\r\n\
\r\n\
";

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip += L"\
[Mix] %MixRatio%\r\n\
%BenchMix1%\r\n\
%BenchMix2%\r\n\
%BenchMix3%\r\n\
%BenchMix4%\r\n\
\r\n\
";
		}
#endif

		clip += L"\
Profile: Default\r\n\
   Test: %TestSize% (x%TestCount%)%Capacity%\r\n\
   Mode:%TestMode%\r\n\
   Time: Measure %MeasureTime% / Interval %IntervalTime% \r\n\
   Date: %Date%\r\n\
     OS: %OS%\r\n\
%Comment%";
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
   Test: %TestSize% (x%TestCount%)%Capacity%\r\n\
   Mode:%TestMode%\r\n\
   Time: Measure %MeasureTime% / Interval %IntervalTime% \r\n\
   Date: %Date%\r\n\
     OS: %OS%\r\n\
%Comment%";
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

	if (m_Profile == PROFILE_DEMO)
	{
		clip.Replace(L"%BenchRead1%", GetResultString(m_BenchType[8], m_ReadScore[8], m_ReadLatency[8], m_BenchSize[8], m_BenchQueues[8], m_BenchThreads[8]));
		clip.Replace(L"%BenchWrite1%", GetResultString(m_BenchType[8], m_WriteScore[8], m_WriteLatency[8], m_BenchSize[8], m_BenchQueues[8], m_BenchThreads[8]));

	}
	else if (m_Profile == PROFILE_PEAK || m_Profile == PROFILE_PEAK_MIX)
	{
		clip.Replace(L"%SequentialRead1%", GetResultString(BENCH_SEQ, m_ReadScore[4], m_ReadLatency[4], m_BenchSize[4], m_BenchQueues[4], m_BenchThreads[4]));
		clip.Replace(L"%SequentialWrite1%", GetResultString(BENCH_SEQ, m_WriteScore[4], m_WriteLatency[4], m_BenchSize[4], m_BenchQueues[4], m_BenchThreads[4]));
		clip.Replace(L"%RandomRead1%", GetResultString(BENCH_RND, m_ReadScore[5], m_ReadLatency[5], m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5]));
		clip.Replace(L"%RandomWrite1%", GetResultString(BENCH_RND, m_WriteScore[5], m_WriteLatency[5], m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5]));

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip.Replace(L"%SequentialMix1%", GetResultString(BENCH_SEQ, m_MixScore[4], m_MixLatency[4], m_BenchSize[4], m_BenchQueues[4], m_BenchThreads[4]));
			clip.Replace(L"%RandomMix1%", GetResultString(BENCH_RND, m_MixScore[5], m_MixLatency[5], m_BenchSize[5], m_BenchQueues[5], m_BenchThreads[5]));
			cstr.Format(L"Read %d%%/Write %d%%", 100 - m_MixRatio, m_MixRatio);
			clip.Replace(L"%MixRatio%", cstr);
		}
#endif
	}
	else if (m_Profile == PROFILE_REAL || m_Profile == PROFILE_REAL_MIX)
	{
		clip.Replace(L"%SequentialRead1%", GetResultString(BENCH_SEQ, m_ReadScore[6], m_ReadLatency[6], 1024, 1, 1));
		clip.Replace(L"%SequentialWrite1%", GetResultString(BENCH_SEQ, m_WriteScore[6], m_WriteLatency[6], 1024, 1, 1));
		clip.Replace(L"%RandomRead1%", GetResultString(BENCH_RND, m_ReadScore[7], m_ReadLatency[7], 4, 1, 1));
		clip.Replace(L"%RandomWrite1%", GetResultString(BENCH_RND, m_WriteScore[7], m_WriteLatency[7], 4, 1, 1));

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip.Replace(L"%SequentialMix1%", GetResultString(BENCH_SEQ, m_MixScore[6], m_MixLatency[6], 1024, 1, 1));
			clip.Replace(L"%RandomMix1%", GetResultString(BENCH_RND, m_MixScore[7], m_MixLatency[7], 4, 1, 1));
			cstr.Format(L"Read %d%%/Write %d%%", 100 - m_MixRatio, m_MixRatio);
			clip.Replace(L"%MixRatio%", cstr);
		}
#endif
	}

	else 
	{
		clip.Replace(L"%BenchRead1%", GetResultString(m_BenchType[0], m_ReadScore[0], m_ReadLatency[0], m_BenchSize[0], m_BenchQueues[0], m_BenchThreads[0]));
		clip.Replace(L"%BenchRead2%", GetResultString(m_BenchType[1], m_ReadScore[1], m_ReadLatency[1], m_BenchSize[1], m_BenchQueues[1], m_BenchThreads[1]));
		clip.Replace(L"%BenchRead3%", GetResultString(m_BenchType[2], m_ReadScore[2], m_ReadLatency[2], m_BenchSize[2], m_BenchQueues[2], m_BenchThreads[2]));
		clip.Replace(L"%BenchRead4%", GetResultString(m_BenchType[3], m_ReadScore[3], m_ReadLatency[3], m_BenchSize[3], m_BenchQueues[3], m_BenchThreads[3]));

		clip.Replace(L"%BenchWrite1%", GetResultString(m_BenchType[0], m_WriteScore[0], m_WriteLatency[0], m_BenchSize[0], m_BenchQueues[0], m_BenchThreads[0]));
		clip.Replace(L"%BenchWrite2%", GetResultString(m_BenchType[1], m_WriteScore[1], m_WriteLatency[1], m_BenchSize[1], m_BenchQueues[1], m_BenchThreads[1]));
		clip.Replace(L"%BenchWrite3%", GetResultString(m_BenchType[2], m_WriteScore[2], m_WriteLatency[2], m_BenchSize[2], m_BenchQueues[2], m_BenchThreads[2]));
		clip.Replace(L"%BenchWrite4%", GetResultString(m_BenchType[3], m_WriteScore[3], m_WriteLatency[3], m_BenchSize[3], m_BenchQueues[3], m_BenchThreads[3]));

#ifdef MIX_MODE
		if (m_MixMode)
		{
			clip.Replace(L"%BenchMix1%", GetResultString(m_BenchType[0], m_MixScore[0], m_MixLatency[0], m_BenchSize[0], m_BenchQueues[0], m_BenchThreads[0]));
			clip.Replace(L"%BenchMix2%", GetResultString(m_BenchType[1], m_MixScore[1], m_MixLatency[1], m_BenchSize[1], m_BenchQueues[1], m_BenchThreads[1]));
			clip.Replace(L"%BenchMix3%", GetResultString(m_BenchType[2], m_MixScore[2], m_MixLatency[2], m_BenchSize[2], m_BenchQueues[2], m_BenchThreads[2]));
			clip.Replace(L"%BenchMix4%", GetResultString(m_BenchType[3], m_MixScore[3], m_MixLatency[3], m_BenchSize[3], m_BenchQueues[3], m_BenchThreads[3]));

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

	cstr = L"";
	if (m_AdminMode){ cstr += L" [Admin]"; }
	if (m_TestData) { cstr += L" <0Fill>"; }
	clip.Replace(L"%TestMode%", cstr);

	m_Comment.GetWindowText(cstr);
	if (cstr.IsEmpty())
	{
		clip.Replace(L"%Comment%", L"");
	}else
	{
		clip.Replace(L"%Comment%", L"Comment: " + cstr + L"\r\n");
	}

	cstr.Format(L"%d sec", m_IntervalTime);
	clip.Replace(L"%IntervalTime%", cstr);
	cstr.Format(L"%d sec", m_MeasureTime);
	clip.Replace(L"%MeasureTime%", cstr);

	GetOsName(cstr);
	clip.Replace(L"%OS%", cstr);

	SYSTEMTIME st;
	GetLocalTime(&st);
	cstr.Format(L"%04d/%02d/%02d %d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	clip.Replace(L"%Date%", cstr);

	if (m_ValueTestDrive.FindOneOf(L":") != -1)
	{
		clip.Replace(L"%Capacity%", L" [" + m_ValueTestDrive + L"]");
	}
	else
	{
		clip.Replace(L"%Capacity%", L"");
	}

	if (fileName.IsEmpty())
	{
		if (OpenClipboard())
		{
			HGLOBAL clipbuffer;
			TCHAR* buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, sizeof(TCHAR) * (clip.GetLength() + 1));
			if (clipbuffer != NULL)
			{
				buffer = (TCHAR*)GlobalLock(clipbuffer);
				_tcscpy_s(buffer, clip.GetLength() + 1, LPCTSTR(clip));
				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_UNICODETEXT, clipbuffer);
			}
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

void CDiskMarkDlg::CheckRadioPresetMode()
{
	if (IsDefaultMode())
	{
		CMenu* menu = GetMenu();
		menu->CheckMenuRadioItem(ID_SETTING_DEFAULT, ID_SETTING_NVME_8, ID_SETTING_DEFAULT, MF_BYCOMMAND);
		SetMenu(menu);
		DrawMenuBar();
	}
	else if (IsNVMe8Mode())
	{
		CMenu* menu = GetMenu();
		menu->CheckMenuRadioItem(ID_SETTING_DEFAULT, ID_SETTING_NVME_8, ID_SETTING_NVME_8, MF_BYCOMMAND);
		SetMenu(menu);
		DrawMenuBar();
	}
	/*
	else if (IsNVMe9Mode())
	{
		CMenu* menu = GetMenu();
		menu->CheckMenuRadioItem(ID_SETTING_DEFAULT, ID_SETTING_NVME_9, ID_SETTING_NVME_9, MF_BYCOMMAND);
		SetMenu(menu);
		DrawMenuBar();
	}
	*/
	else
	{
		CMenu* menu = GetMenu();
		menu->CheckMenuRadioItem(ID_SETTING_DEFAULT, ID_SETTING_NVME_8, 0, MF_BYCOMMAND);
		SetMenu(menu);
		DrawMenuBar();
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
	default:  id = ID_ZOOM_AUTO;break;
	}

	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_ZOOM_100, ID_ZOOM_AUTO, id, MF_BYCOMMAND);
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
		OpenUrl(URL_MAIN_JA);
	}
	else // Other Language
	{
		OpenUrl(URL_MAIN_EN);
	}
}

#ifdef MIX_MODE
	#define ID_PROFILE_MAX ID_PROFILE_REAL_MIX
#else
	#define ID_PROFILE_MAX ID_PROFILE_DEMO
#endif

void CDiskMarkDlg::OnSettingDefault()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_SETTING_DEFAULT, ID_SETTING_NVME_8, ID_SETTING_DEFAULT, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	SettingsQueuesThreads(0);
}

void CDiskMarkDlg::OnSettingNVMe8()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_SETTING_DEFAULT, ID_SETTING_NVME_8, ID_SETTING_NVME_8, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	SettingsQueuesThreads(1);
}

/*
void CDiskMarkDlg::OnSettingNVMe9()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_SETTING_DEFAULT, ID_SETTING_NVME_9, ID_SETTING_NVME_9, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	SettingsQueuesThreads(2);
}
*/

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
	ShowWindow(SW_HIDE);
	ProfileDefault();
	UpdateUnitLabel();
	InitScore();
	UpdateDialogSize();
	ChangeButtonStatus(TRUE);
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfileDefault()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_MAX, ID_PROFILE_DEFAULT, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_DEFAULT;
	m_MixMode = FALSE;
	WritePrivateProfileString(L"Setting", L"Profile", L"0", m_Ini);
	m_BackgroundName = L"Background";
}

void CDiskMarkDlg::OnProfilePeak()
{
	ShowWindow(SW_HIDE);
	ProfilePeak();
	UpdateUnitLabel();
	InitScore();
	UpdateDialogSize();
	ChangeButtonStatus(TRUE);
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfilePeak()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_MAX, ID_PROFILE_PEAK, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_PEAK;
	m_MixMode = FALSE;
	WritePrivateProfileString(L"Setting", L"Profile", L"1", m_Ini);
	m_BackgroundName = L"Background";
}

void CDiskMarkDlg::OnProfileReal()
{
	ShowWindow(SW_HIDE);
	ProfileReal();
	UpdateUnitLabel();
	InitScore();
	UpdateDialogSize();
	ChangeButtonStatus(TRUE);
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfileReal()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_MAX, ID_PROFILE_REAL, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_REAL;
	m_MixMode = FALSE;
	WritePrivateProfileString(L"Setting", L"Profile", L"2", m_Ini);
	m_BackgroundName = L"Background";
}

void CDiskMarkDlg::OnProfileDemo()
{
	ShowWindow(SW_HIDE);
	ProfileDemo();
	UpdateUnitLabel();
	InitScore();
	UpdateDialogSize();
	ChangeButtonStatus(TRUE);
	SetWindowTitle(L"");
}

void CDiskMarkDlg::ProfileDemo()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_PROFILE_DEFAULT, ID_PROFILE_MAX, ID_PROFILE_DEMO, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Profile = PROFILE_DEMO;
	m_MixMode = FALSE;
	WritePrivateProfileString(L"Setting", L"Profile", L"3", m_Ini);

	if (IsFileExist(m_ThemeDir + m_CurrentTheme + L"\\BackgroundDemo-300.png"))
	{
		m_BackgroundName = L"BackgroundDemo";
	}
	else
	{
		m_BackgroundName = L"Background";
	}
}

#ifdef MIX_MODE
void CDiskMarkDlg::OnProfileDefaultMix()
{
	ProfileDefaultMix();
	UpdateUnitLabel();
	InitScore();
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
	WritePrivateProfileString(L"Setting", L"Profile", L"4", m_Ini);
	m_BackgroundName = L"Background";
}

void CDiskMarkDlg::OnProfilePeakMix()
{
	ProfilePeakMix();
	ChangeButtonStatus(TRUE);
	UpdateUnitLabel();
	InitScore();
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
	WritePrivateProfileString(L"Setting", L"Profile", L"5", m_Ini);
	m_BackgroundName = L"Background";
}

void CDiskMarkDlg::OnProfileRealMix()
{
	ProfileRealMix();
	ChangeButtonStatus(TRUE);
	UpdateUnitLabel();
	InitScore();
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
	WritePrivateProfileString(L"Setting", L"Profile", L"6", m_Ini);
	m_BackgroundName = L"Background";
}
#endif

void CDiskMarkDlg::OnBenchmarkReadWrite()
{
	BenchmarkReadWrite();
}

void CDiskMarkDlg::BenchmarkReadWrite()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_BENCHMARK_READ_WRITE, ID_BENCHMARK_WRITE_ONLY, ID_BENCHMARK_READ_WRITE, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Benchmark = BENCHMARK_READ_WRITE;
	WritePrivateProfileString(L"Setting", L"Benchmark", L"3", m_Ini);
}

void CDiskMarkDlg::OnBenchmarkReadOnly()
{
	BenchmarkReadOnly();
}

void CDiskMarkDlg::BenchmarkReadOnly()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_BENCHMARK_READ_WRITE, ID_BENCHMARK_WRITE_ONLY, ID_BENCHMARK_READ_ONLY, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Benchmark = BENCHMARK_READ;
	WritePrivateProfileString(L"Setting", L"Benchmark", L"1", m_Ini);
}

void CDiskMarkDlg::OnBenchmarkWriteOnly()
{
	BenchmarkWriteOnly();
}

void CDiskMarkDlg::BenchmarkWriteOnly()
{
	CMenu* menu = GetMenu();
	menu->CheckMenuRadioItem(ID_BENCHMARK_READ_WRITE, ID_BENCHMARK_WRITE_ONLY, ID_BENCHMARK_WRITE_ONLY, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_Benchmark = BENCHMARK_WRITE;
	WritePrivateProfileString(L"Setting", L"Benchmark", L"2", m_Ini);
}

void CDiskMarkDlg::OnSettingsQueuesThreads()
{
	if (! m_DiskBenchStatus)
	{
		CSettingsDlg* dlg = new CSettingsDlg(this);
		dlg->DoModal();
		UpdateQueuesThreads();
		ChangeButtonStatus(TRUE);

		UpdateData(FALSE);

		delete dlg;
	}
}

void CDiskMarkDlg::OnFontSetting()
{
	CFontSelectionDlg fontSelection(this);
	if (fontSelection.DoModal() == IDOK)
	{
		m_FontFace = fontSelection.GetFontFace();
		m_FontScale = fontSelection.GetFontScale();
		m_FontRatio = m_FontScale / 100.0;
		m_FontRender = fontSelection.GetFontRender();

		CString cstr;
		WritePrivateProfileString(L"Setting", L"FontFace", L"\"" + m_FontFace + L"\"", m_Ini);
		cstr.Format(L"%d", m_FontScale);
		WritePrivateProfileString(L"Setting", L"FontScale", cstr, m_Ini);
		cstr.Format(L"%d", m_FontRender);
		WritePrivateProfileString(L"Setting", L"FontRender", cstr, m_Ini);

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

void CDiskMarkDlg::UpdateUnitLabel()
{
	if (m_Profile == PROFILE_DEMO)
	{
		if (m_IndexTestUnit == SCORE_UNIT::SCORE_IOPS)
		{
			m_TestRead0.SetLabelUnit(L"Read", L"IOPS");
			m_TestWrite0.SetLabelUnit(L"Write", L"IOPS");
		}
		else if (m_IndexTestUnit == SCORE_UNIT::SCORE_US)
		{
			if (m_ReadLatency[8] >= 1000000)
			{
				m_TestRead0.SetLabelUnit(L"Read", L"ms");
			}
			else
			{
				m_TestRead0.SetLabelUnit(L"Read", L"μs");
			}

			if (m_WriteLatency[8] >= 1000000)
			{
				m_TestWrite0.SetLabelUnit(L"Write", L"ms");
			}
			else
			{
				m_TestWrite0.SetLabelUnit(L"Write", L"μs");
			}
		}
		else if (m_IndexTestUnit == SCORE_UNIT::SCORE_GBS)
		{
			m_TestRead0.SetLabelUnit(L"Read", L"GB/s");
			m_TestWrite0.SetLabelUnit(L"Write", L"GB/s");
		}
		else
		{
			m_TestRead0.SetLabelUnit(L"Read", L"MB/s");
			m_TestWrite0.SetLabelUnit(L"Write", L"MB/s");
		}
	}
	else
	{
		m_TestRead0.SetLabelUnit(L"", L"");
		m_TestWrite0.SetLabelUnit(L"", L"");
	}

	m_TestRead0.Invalidate();
	m_TestWrite0.Invalidate();

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
		m_ReadUnit.SetWindowTextW(L"Read (IOPS)");
		m_WriteUnit.SetWindowTextW(L"Write (IOPS)");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix (IOPS)");
#endif
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_US)
	{
		m_ReadUnit.SetWindowTextW(L"Read (μs)");
		m_WriteUnit.SetWindowTextW(L"Write (μs)");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix (μs)");
#endif
	}
	else if (m_IndexTestUnit == SCORE_UNIT::SCORE_GBS)
	{
		m_ReadUnit.SetWindowTextW(L"Read (GB/s)");
		m_WriteUnit.SetWindowTextW(L"Write (GB/s)");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix (GB/s)");
#endif
	}
	else
	{
		m_ReadUnit.SetWindowTextW(L"Read (MB/s)");
		m_WriteUnit.SetWindowTextW(L"Write (MB/s)");
#ifdef MIX_MODE
		m_MixUnit.SetWindowTextW(L"Mix (MB/s)");
#endif
	}
}

void CDiskMarkDlg::SetWindowTitle(CString message)
{
	CString title;

	if (!message.IsEmpty())
	{
		title.Format(L"%s", message.GetString());
	}
	else
	{
		title.Format(L"%s %s %s", PRODUCT_NAME, PRODUCT_VERSION, PRODUCT_EDITION);
	}

	if (m_AdminMode)
	{
		title += L" [Admin]";
	}

	if (m_TestData == TEST_DATA_ALL0X00)
	{
		title += L" <0Fill>";
	}

	SetWindowText(title);
}

void CDiskMarkDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Move Focus to Hide Control
	GetDlgItem(IDC_HIDE)->SetFocus();

	CMainDialogFx::OnLButtonDown(nFlags, point);
}