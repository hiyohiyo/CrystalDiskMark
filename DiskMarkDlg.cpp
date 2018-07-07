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

#define MAX_METER_LENGTH	150

#ifdef SUISHO_SHIZUKU_SUPPORT
#define SIZE_X		1000
#define SIZE_Y		584
#define SIZE_MIN_Y	584
#define OFFSET_X    328
//#define MAIN_CSS_FILE_NAME	_T("MainS.css")
#else
#define SIZE_X		672
#define SIZE_Y		584
#define SIZE_MIN_Y	584
#define OFFSET_X    0
//#define MAIN_CSS_FILE_NAME	_T("Main.css")
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

	DDX_Control(pDX, IDC_COMMENT, m_Comment);

	DDX_Control(pDX, IDC_COMBO_COUNT, m_ComboCount);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_ComboSize);
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_ComboDrive);

	DDX_Control(pDX, IDC_READ_MBS, m_ReadMbps);
	DDX_Control(pDX, IDC_WRITE_MBS, m_WriteMbps);
}

BEGIN_MESSAGE_MAP(CDiskMarkDlg, CMainDialog)
	//}}AFX_MSG_MAP
#ifdef SUISHO_SHIZUKU_SUPPORT
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
#endif
	ON_COMMAND(ID_FILE_EXIT, OnExit)
	ON_COMMAND(ID_HELP_ABOUT, OnAbout)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_MESSAGE(WM_USER_UPDATE_SCORE, OnUpdateScore)
	ON_MESSAGE(WM_USER_UPDATE_MESSAGE, OnUpdateMessage)
	ON_MESSAGE(WM_USER_EXIT_BENCHMARK, OnExitBenchmark)
	ON_COMMAND(ID_ZOOM_75,  &CDiskMarkDlg::OnZoom75)
	ON_COMMAND(ID_ZOOM_100, &CDiskMarkDlg::OnZoom100)
	ON_COMMAND(ID_ZOOM_125, &CDiskMarkDlg::OnZoom125)
	ON_COMMAND(ID_ZOOM_150, &CDiskMarkDlg::OnZoom150)
	ON_COMMAND(ID_ZOOM_200, &CDiskMarkDlg::OnZoom200)
	ON_COMMAND(ID_ZOOM_250, &CDiskMarkDlg::OnZoom250)
	ON_COMMAND(ID_ZOOM_300, &CDiskMarkDlg::OnZoom300)
	ON_COMMAND(ID_ZOOM_400, &CDiskMarkDlg::OnZoom400)
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

	ON_BN_CLICKED(IDC_BUTTON_ALL, &CDiskMarkDlg::OnBnClickedAll)
END_MESSAGE_MAP()

LRESULT CDiskMarkDlg::OnQueryEndSession(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

int CALLBACK EnumFontFamExProcMeiryo(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam)
{
	if (_tcscmp(lpelfe->elfLogFont.lfFaceName, _T("メイリオ")) == 0)
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
	m_BackgroundName = L"mainBackground";
#endif

	TCHAR str[256];

	CClientDC dc(this);
	LOGFONT logfont;
	CString defaultFontFace;
	BOOL hasMeiryo = FALSE;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = ANSI_CHARSET;
	::EnumFontFamiliesExW(dc.m_hDC, &logfont, (FONTENUMPROC)EnumFontFamExProcMeiryo, (INT_PTR)(&hasMeiryo), 0);

	if (hasMeiryo)
	{
		defaultFontFace = _T("メイリオ");
	}
	else
	{
		defaultFontFace = _T("Tahoma");
	}

	GetPrivateProfileString(_T("Setting"), _T("FontFace"), defaultFontFace, str, 256, m_Ini);
	m_FontFace = str;

	m_IndexTestDrive = 0;	// default value may be "C:\".
	m_MaxIndexTestDrive = 0;

	m_TestDriveLetter = GetPrivateProfileInt(_T("Settings"), _T("DriveLetter"), 2, m_Ini);
	if(m_TestDriveLetter != 99 && (m_TestDriveLetter < 0 || m_TestDriveLetter > 'Z' - 'A'))
	{
		m_TestDriveLetter = 'C' - 'A'; // default value is "C:\"
	}

	if (m_TestDriveLetter == 99)
	{
		TCHAR targetPath[MAX_PATH];
		GetPrivateProfileStringW(_T("Settings"), _T("TargetPath"), L"", targetPath, MAX_PATH, m_Ini);
		m_TestTargetPath = targetPath;
	}

	m_IndexTestCount = GetPrivateProfileInt(_T("Settings"), _T("TestCount"), 4, m_Ini);
	if (m_IndexTestCount < 0 || m_IndexTestCount >= 9)
	{
		m_IndexTestCount = 4;	// default value is 5.
	}

	m_IndexTestSize = GetPrivateProfileInt(_T("Settings"), _T("TestSize"), 3, m_Ini);
	if(m_IndexTestSize < 0 || m_IndexTestSize > 9)
	{
		m_IndexTestSize = 3;	// default value is 1000 MiB;
	}

	m_IntervalTime = GetPrivateProfileInt(_T("Settings"), _T("IntervalTime"), 5, m_Ini);
	if (m_IntervalTime < 0)
	{
		m_IntervalTime = 5;	// default value is 5;
	}

	UpdateQueuesThreads();

	m_WinThread = NULL;
	m_DiskBenchStatus = FALSE;
	m_FragmenteCounts = -1;

	InitThemeLang();
	InitMenu();

	switch(GetPrivateProfileInt(_T("Settings"), _T("ZoomType"), 0, m_Ini))
	{
	case  75:  CheckRadioZoomType(ID_ZOOM_75,   75); break;
	case 100:  CheckRadioZoomType(ID_ZOOM_100, 100); break;
	case 125:  CheckRadioZoomType(ID_ZOOM_125, 125); break;
	case 150:  CheckRadioZoomType(ID_ZOOM_150, 150); break;
	case 200:  CheckRadioZoomType(ID_ZOOM_200, 200); break;
	case 250:  CheckRadioZoomType(ID_ZOOM_250, 250); break;
	case 300:  CheckRadioZoomType(ID_ZOOM_300, 300); break;
	case 400:  CheckRadioZoomType(ID_ZOOM_400, 400); break;
	default:   CheckRadioZoomType(ID_ZOOM_AUTO, 0); break;
	}

	m_TestData = (BOOL)GetPrivateProfileInt(_T("Settings"), _T("TestData"), TEST_DATA_RANDOM, m_Ini);
	m_SizeX = SIZE_X;
#ifdef SUISHO_SHIZUKU_SUPPORT
	int sizeY = GetPrivateProfileInt(_T("Settings"), _T("Height"), 0, m_Ini);
	if (sizeY == 0)
	{
		m_SizeY = SIZE_Y;
	}
	else if (sizeY <= SIZE_MIN_Y)
	{
		m_SizeY = SIZE_MIN_Y;
	}
	else if (SIZE_Y <= sizeY)
	{
		m_SizeY = SIZE_Y;
	}
	else
	{
		m_SizeY = sizeY;
	}
#else
	m_SizeY = SIZE_Y;
#endif

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
//	ChangeTheme(m_CurrentTheme);
//	ChangeButtonStatus(TRUE);
	UpdateDialogSize();

	CenterWindow();
	ShowWindow(SW_SHOW);
	m_FlagInitializing = FALSE;

	ChangeLang(m_CurrentLang);


	return TRUE;
}

void CDiskMarkDlg::UpdateDialogSize()
{
	ShowWindow(SW_HIDE);

	UpdateBackground(true);

	m_SizeX = SIZE_X;
	if (GetPrivateProfileInt(_T("Setting"), _T("Height"), 0, m_Ini) > 0)
	{
		m_SizeY = GetPrivateProfileInt(_T("Setting"), _T("Height"), 0, m_Ini);
	}
	else
	{
		m_SizeY = SIZE_Y;
	}
	SetClientRect((DWORD)(m_SizeX * m_ZoomRatio), (DWORD)(m_SizeY * m_ZoomRatio), 1);


	SetControlFont();

	m_ButtonAll.InitControl(        8 + OFFSET_X,  8, 160, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::SystemDraw | m_IsHighContrast);
	m_ButtonSequential1.InitControl(8 + OFFSET_X, 96, 160, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonSequential2.InitControl(8 + OFFSET_X,184, 160, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom1.InitControl(    8 + OFFSET_X,272, 160, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom2.InitControl(    8 + OFFSET_X,360, 160, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawImage | m_IsHighContrast);
	m_ButtonRandom3.InitControl(    8 + OFFSET_X,448, 160, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawImage | m_IsHighContrast);

	m_ButtonAll.SetHandCursor(TRUE);
	m_ButtonSequential1.SetHandCursor(TRUE);
	m_ButtonSequential2.SetHandCursor(TRUE);
	m_ButtonRandom1.SetHandCursor(TRUE);
	m_ButtonRandom2.SetHandCursor(TRUE);
	m_ButtonRandom3.SetHandCursor(TRUE);

	m_SequentialRead1.InitControl(176 + OFFSET_X,  96, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_SequentialRead2.InitControl(176 + OFFSET_X, 184, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomRead1.InitControl(    176 + OFFSET_X, 272, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomRead2.InitControl(    176 + OFFSET_X, 360, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomRead3.InitControl(    176 + OFFSET_X, 448, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);

	m_SequentialWrite1.InitControl(424 + OFFSET_X,  96, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_SequentialWrite2.InitControl(424 + OFFSET_X, 184, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomWrite1.InitControl(    424 + OFFSET_X, 272, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomWrite2.InitControl(    424 + OFFSET_X, 360, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);
	m_RandomWrite3.InitControl(    424 + OFFSET_X, 448, 240, 80, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawGlass | m_IsHighContrast);

	m_Comment.MoveWindow((int)((8 + OFFSET_X) * m_ZoomRatio), (int)(536 * m_ZoomRatio), (int)(656 * m_ZoomRatio), (int)(40 * m_ZoomRatio));

	m_ReadMbps.InitControl( 176 + OFFSET_X, 48, 240, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_WriteMbps.InitControl(424 + OFFSET_X, 48, 240, 40, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_ComboCount.MoveWindow((int)((176 + OFFSET_X) * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(80 * m_ZoomRatio), (int)(40 * m_ZoomRatio));
	m_ComboSize.MoveWindow ((int)((264 + OFFSET_X) * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(120 * m_ZoomRatio), (int)(40 * m_ZoomRatio));
	m_ComboDrive.MoveWindow((int)((392 + OFFSET_X) * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(272 * m_ZoomRatio), (int)(40 * m_ZoomRatio));


	::SetWindowLong(m_Comment.m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_Comment.m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
	::SetWindowLong(m_Comment.m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_Comment.m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	if (m_IsHighContrast)
	{
		::SetLayeredWindowAttributes(m_Comment.m_hWnd, 0, 255, LWA_ALPHA);
	}
	else
	{
		::SetLayeredWindowAttributes(m_Comment.m_hWnd, 0, 192, LWA_ALPHA);
	}
	Invalidate();
	ShowWindow(SW_SHOW);
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

	m_ButtonAll.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonSequential1.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonSequential2.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonRandom1.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonRandom2.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_ButtonRandom3.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_SequentialRead1.SetFontEx(m_FontFace, 56, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_SequentialRead2.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomRead1.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomRead2.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomRead3.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_SequentialWrite1.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_SequentialWrite2.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomWrite1.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomWrite2.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_RandomWrite3.SetFontEx(m_FontFace, 18, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_Comment.SetFontEx(m_FontFace, 24, m_ZoomRatio, FW_BOLD);

	m_ReadMbps.SetFontEx(m_FontFace, 24, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);
	m_WriteMbps.SetFontEx(m_FontFace, 24, m_ZoomRatio, textAlpha, textColor, FW_BOLD, m_FontType);

	m_ComboCount.SetFontEx(m_FontFace, 24, m_ZoomRatio);
	m_ComboSize.SetFontEx(m_FontFace, 24, m_ZoomRatio);
	m_ComboDrive.SetFontEx(m_FontFace, 24, m_ZoomRatio);

}

CString CDiskMarkDlg::IP(CString imageName)
{
	CString imagePath;
	imagePath.Format(L"%s%s\\%s-%3d.png", m_ThemeDir, m_CurrentTheme, imageName, (DWORD)(m_ZoomRatio * 100));
	if (IsFileExist(imagePath))
	{
		return imagePath;
	}
#ifdef SUISHO_SHIZUKU_SUPPORT
	if (m_CurrentTheme.Find(L"ShizukuMikoNight") == 0)
	{
		imagePath.Format(L"%s%s\\%s-%3d.png", m_ThemeDir, L"ShizukuMiko", imageName, (DWORD)(m_ZoomRatio * 100));
		if (IsFileExist(imagePath))
		{
			return imagePath;
		}
	}
	if (m_CurrentTheme.Find(L"ShizukuHotaru") == 0)
	{
		imagePath.Format(L"%s%s\\%s-%3d.png", m_ThemeDir, L"ShizukuHeianKomachi", imageName, (DWORD)(m_ZoomRatio * 100));
		if (IsFileExist(imagePath))
		{
			return imagePath;
		}
	}
#endif
	imagePath.Format(L"%s%s\\%s-%3d.png", m_ThemeDir, m_DefaultTheme, imageName, (DWORD)(m_ZoomRatio * 100));
	if (IsFileExist(imagePath))
	{
		return imagePath;
	}
	return L"";
}





void CDiskMarkDlg::UpdateQueuesThreads()
{
	CString cstr;
	m_SequentialMultiQueues1 = GetPrivateProfileInt(_T("Settings"), _T("SequentialMultiQueues1"), 32, m_Ini);
	if (m_SequentialMultiQueues1 <= 0 || m_SequentialMultiQueues1 > MAX_QUEUES)
	{
		m_SequentialMultiQueues1 = 32;
	}

	m_SequentialMultiThreads1 = GetPrivateProfileInt(_T("Settings"), _T("SequentialMultiThreads1"), 1, m_Ini);
	if (m_SequentialMultiThreads1 <= 0 || m_SequentialMultiThreads1 > MAX_THREADS)
	{
		m_SequentialMultiThreads1 = 1;
	}

#ifdef SEQUENTIAL2
	m_SequentialMultiQueues2 = GetPrivateProfileInt(_T("Settings"), _T("SequentialMultiQueues2"), 8, m_Ini);
	if (m_SequentialMultiQueues2 <= 0 || m_SequentialMultiQueues2 > MAX_QUEUES)
	{
		m_SequentialMultiQueues2 = 8;
	}

	m_SequentialMultiThreads2 = GetPrivateProfileInt(_T("Settings"), _T("SequentialMultiThreads2"), 1, m_Ini);
	if (m_SequentialMultiThreads2 <= 0 || m_SequentialMultiThreads2 > MAX_THREADS)
	{
		m_SequentialMultiThreads2 = 1;
	}
#endif

	m_RandomMultiQueues1 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiQueues1"), 8, m_Ini);
	if (m_RandomMultiQueues1 <= 0 || m_RandomMultiQueues1 > MAX_QUEUES)
	{
		m_RandomMultiQueues1 = 8;
	}

	m_RandomMultiThreads1 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiThreads1"), 8, m_Ini);
	if (m_RandomMultiThreads1 <= 0 || m_RandomMultiThreads1 > MAX_THREADS)
	{
		m_RandomMultiThreads1 = 8;
	}

	m_RandomMultiQueues2 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiQueues2"), 32, m_Ini);
	if (m_RandomMultiQueues2 <= 0 || m_RandomMultiQueues2 > MAX_QUEUES)
	{
		m_RandomMultiQueues2 = 32;
	}

	m_RandomMultiThreads2 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiThreads2"), 1, m_Ini);
	if (m_RandomMultiThreads2 <= 0 || m_RandomMultiThreads2 > MAX_THREADS)
	{
		m_RandomMultiThreads2 = 1;
	}

	m_RandomMultiQueues3 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiQueues3"), 1, m_Ini);
	if (m_RandomMultiQueues3 <= 0 || m_RandomMultiQueues3 > MAX_QUEUES)
	{
		m_RandomMultiQueues3 = 1;
	}

	m_RandomMultiThreads3 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiThreads3"), 1, m_Ini);
	if (m_RandomMultiThreads3 <= 0 || m_RandomMultiThreads3 > MAX_THREADS)
	{
		m_RandomMultiThreads3 = 1;
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

HRESULT CDiskMarkDlg::OnSelectDrive()
{
	if (m_DiskBenchStatus)
	{
		return FALSE;
	}

	static DWORD preTime = 0;
	DWORD currentTime = GetTickCount();

	if (currentTime - preTime < 100)
	{
		preTime = currentTime;
		return FALSE;
	}

	UpdateData(TRUE);

	if (m_IndexTestDrive == m_MaxIndexTestDrive)
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
				WritePrivateProfileString(_T("Settings"), _T("TargetPath"), szTmp, m_Ini);
				m_TestTargetPath = szTmp;
			}
			g_pMalloc->Free(idl);
			g_pMalloc->Release();
		}
		ChangeSelectTitle(_T("TestDrive"), m_TitleTestDrive + L"(" + m_TestTargetPath + L")");
	}
	else
	{
		ChangeSelectTitle(_T("TestDrive"), m_TitleTestDrive);
	}
	preTime = GetTickCount();

	return TRUE;
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
	cstr.Format(_T("%d"), m_IndexTestSize);
	WritePrivateProfileString(_T("Settings"), _T("TestSize"), cstr, m_Ini);
	cstr.Format(_T("%d"), m_IndexTestCount);
	WritePrivateProfileString(_T("Settings"), _T("TestCount"), cstr, m_Ini);

	if(m_IndexTestDrive != m_MaxIndexTestDrive)
	{ 
		cstr.Format(_T("%d"), m_ValueTestDrive.GetAt(0) - 'A');
	}
	else
	{
		cstr.Format(_T("%d"), 99);
	}
	WritePrivateProfileString(_T("Settings"), _T("DriveLetter"), cstr, m_Ini);

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
	UpdateScore();
}

void CDiskMarkDlg::UpdateScore()
{
	UpdateData(TRUE); // Hold Comment
	SetMeter(_T("SequentialRead1"), m_SequentialReadScore1);
	SetMeter(_T("SequentialWrite1"), m_SequentialWriteScore1);
#ifdef SEQUENTIAL2
	SetMeter(_T("SequentialRead2"), m_SequentialReadScore2);
	SetMeter(_T("SequentialWrite2"), m_SequentialWriteScore2);
#endif
	SetMeter(_T("RandomRead4KB1"), m_RandomRead4KBScore1);
	SetMeter(_T("RandomWrite4KB1"), m_RandomWrite4KBScore1);
	SetMeter(_T("RandomRead4KB2"), m_RandomRead4KBScore2);
	SetMeter(_T("RandomWrite4KB2"), m_RandomWrite4KBScore2);
	SetMeter(_T("RandomRead4KB3"), m_RandomRead4KBScore3);
	SetMeter(_T("RandomWrite4KB3"), m_RandomWrite4KBScore3);

	// Set IOPS value as title
	CString cstr;
	cstr.Format(_T("%8.1f IOPS"), m_RandomRead4KBScore1 * 1000 * 1000 / 4096);
//	SetElementPropertyEx(_T("RandomRead4KB1"), DISPID_IHTMLELEMENT_TITLE, cstr);
	cstr.Format(_T("%8.1f IOPS"), m_RandomWrite4KBScore1 * 1000 * 1000 / 4096);
//	SetElementPropertyEx(_T("RandomWrite4KB1"), DISPID_IHTMLELEMENT_TITLE, cstr);
	cstr.Format(_T("%8.1f IOPS"), m_RandomRead4KBScore2 * 1000 * 1000 / 4096);
//	SetElementPropertyEx(_T("RandomRead4KB2"), DISPID_IHTMLELEMENT_TITLE, cstr);
	cstr.Format(_T("%8.1f IOPS"), m_RandomWrite4KBScore2 * 1000 * 1000 / 4096);
//	SetElementPropertyEx(_T("RandomWrite4KB2"), DISPID_IHTMLELEMENT_TITLE, cstr);
	cstr.Format(_T("%8.1f IOPS"), m_RandomRead4KBScore3 * 1000 * 1000 / 4096);
//	SetElementPropertyEx(_T("RandomRead4KB3"), DISPID_IHTMLELEMENT_TITLE, cstr);
	cstr.Format(_T("%8.1f IOPS"), m_RandomWrite4KBScore3 * 1000 * 1000 / 4096);
//	SetElementPropertyEx(_T("RandomWrite4KB3"), DISPID_IHTMLELEMENT_TITLE, cstr);
}

HRESULT CDiskMarkDlg::OnSequential1()
{
	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_SequentialReadScore1 = 0.0;
		m_SequentialWriteScore1 = 0.0;
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
	return S_FALSE;
}

#ifdef SEQUENTIAL2
HRESULT CDiskMarkDlg::OnSequential2()
{
	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_SequentialReadScore2 = 0.0;
		m_SequentialWriteScore2 = 0.0;
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
	return S_FALSE;
}
#endif

HRESULT CDiskMarkDlg::OnRandom4KB1()
{
	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomRead4KBScore1 = 0.0;
		m_RandomWrite4KBScore1 = 0.0;
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
	return S_FALSE;
}

HRESULT CDiskMarkDlg::OnRandom4KB2()
{
	if(m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomRead4KBScore2 = 0.0;
		m_RandomWrite4KBScore2 = 0.0;
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
	return S_FALSE;
}

HRESULT CDiskMarkDlg::OnRandom4KB3()
{
	if (m_WinThread == NULL)
	{
		UpdateData(TRUE);
		m_RandomRead4KBScore3 = 0.0;
		m_RandomWrite4KBScore3 = 0.0;
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
	return S_FALSE;
}


HRESULT CDiskMarkDlg::OnAll()
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
	return S_FALSE;
}

void CDiskMarkDlg::Stop()
{
	if(m_DiskBenchStatus)
	{
		m_DiskBenchStatus = FALSE;
		UpdateMessage(_T("Message"), _T("Stopping..."));
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
		CString title;
		CString toolTip;

		ChangeButton(_T("All"),			_T("button1"),	_T("All"),						_T("All"));

		toolTip.Format(L"Sequential, Queues=%d, Threads=%d", m_SequentialMultiQueues1, m_SequentialMultiThreads1);
		title.Format(L"Seq<br>Q%dT%d", m_SequentialMultiQueues1, m_SequentialMultiThreads1);
		ChangeButton(_T("Sequential1"), _T("button2"), toolTip, title);
#ifdef SEQUENTIAL2
		toolTip.Format(L"Sequential, Queues=%d, Threads=%d", m_SequentialMultiQueues2, m_SequentialMultiThreads2);
		title.Format(L"Seq<br>Q%dT%d", m_SequentialMultiQueues2, m_SequentialMultiThreads2);
		ChangeButton(_T("Sequential2"), _T("button2"), toolTip, title);
#endif
		toolTip.Format(L"Random 4KiB, Queues=%d, Threads=%d", m_RandomMultiQueues1, m_RandomMultiThreads1);
		title.Format(L"4KiB<br>Q%dT%d", m_RandomMultiQueues1, m_RandomMultiThreads1);
		ChangeButton(_T("Random4KB1"), _T("button2"), toolTip, title);
		
		toolTip.Format(L"Random 4KiB, Queues=%d, Threads=%d", m_RandomMultiQueues2, m_RandomMultiThreads2);
		title.Format(L"4KiB<br>Q%dT%d", m_RandomMultiQueues2, m_RandomMultiThreads2);
		ChangeButton(_T("Random4KB2"), _T("button2"), toolTip, title);
	
		toolTip.Format(L"Random 4KiB, Queues=%d, Threads=%d", m_RandomMultiQueues3, m_RandomMultiThreads3);
		title.Format(L"4KiB<br>Q%dT%d", m_RandomMultiQueues3, m_RandomMultiThreads3);
		ChangeButton(_T("Random4KB3"), _T("button2"), toolTip, title);

		ChangeSelectStatus(_T("TestDrive"),	VARIANT_FALSE);
		ChangeSelectStatus(_T("TestCount"),VARIANT_FALSE);
		ChangeSelectStatus(_T("TestSize"),	VARIANT_FALSE);
	}
	else
	{
		ChangeButton(_T("All"),        _T("button1"), _T("Stop"), _T("Stop"));
		ChangeButton(_T("Sequential1"),_T("button1"), _T("Stop"), _T("Stop"));
#ifdef SEQUENTIAL2
		ChangeButton(_T("Sequential2"),_T("button1"), _T("Stop"), _T("Stop"));
#endif
		ChangeButton(_T("Random4KB1"), _T("button1"), _T("Stop"), _T("Stop"));
		ChangeButton(_T("Random4KB2"), _T("button1"), _T("Stop"), _T("Stop"));
		ChangeButton(_T("Random4KB3"), _T("button1"), _T("Stop"), _T("Stop"));
		ChangeSelectStatus(_T("TestDrive"),	VARIANT_TRUE);
		ChangeSelectStatus(_T("TestCount"),VARIANT_TRUE);
		ChangeSelectStatus(_T("TestSize"),	VARIANT_TRUE);
	}
}

void CDiskMarkDlg::ChangeButton(CString elementName, CString className, CString title, CString innerHtml)
{
//	SetElementPropertyEx(elementName, DISPID_IHTMLELEMENT_CLASSNAME, className);
//	SetElementPropertyEx(elementName, DISPID_IHTMLELEMENT_TITLE, title);
//	SetElementInnerHtmlEx(elementName, innerHtml);
}

void CDiskMarkDlg::ChangeSelectStatus(CString ElementName, VARIANT_BOOL status)
{

}

void CDiskMarkDlg::ChangeSelectTitle(CString ElementName, CString title)
{

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

void CDiskMarkDlg::UpdateMessage(CString ElementName, CString message)
{
	CComBSTR bstr;
	bstr = _T("&nbsp;") + message;
//	SetElementHtml(ElementName, bstr);
}

void CDiskMarkDlg::SetMeter(CString ElementName, double Score)
{
	CString cstr;

	int meterLength;
	if(Score > 0.1)
	{
		meterLength = (int)(MAX_METER_LENGTH / 5 * log10(Score * 10));
	}
	else
	{
		meterLength = 0;
	}

	if(meterLength > MAX_METER_LENGTH)
	{
		meterLength = MAX_METER_LENGTH;
	}
	else if(meterLength < 1)
	{
		meterLength = 0;
	}

	cstr.Format(_T("%dpx"), -1 * (MAX_METER_LENGTH - meterLength));

	if(Score >= 1000000.0)
	{
		cstr.Format(_T("999999"));
	}
	else if (Score >= 10000.0)
	{
		cstr.Format(_T("%d"), (int)Score);
	}
	else if (Score < 10.0)
	{
		cstr.Format(_T("%.3f"), Score);
	}
	else if (Score < 100.0)
	{
		cstr.Format(_T("%.2f"), Score);
	}
	else
	{
		cstr.Format(_T("%.1f"), Score);
	}

	UpdateData(FALSE);

	if (Score >= 100000.0)
	{
	//	SetElementPropertyEx(ElementName, DISPID_IHTMLELEMENT_CLASSNAME, _T("meter1HighScore"));
	}
	else if(Score > 0.0)
	{
	//	SetElementPropertyEx(ElementName, DISPID_IHTMLELEMENT_CLASSNAME, _T("meter1"));
	}
	else
	{
	//	SetElementPropertyEx(ElementName, DISPID_IHTMLELEMENT_CLASSNAME, _T("meter0"));
	}
}

void CDiskMarkDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CString cstr;
	cstr = szUrl;
	static BOOL once = FALSE;
	if(cstr.Find(_T("html")) != -1 || cstr.Find(_T("dlg")) != -1)
	{
		if(! once)
		{
			UpdateMessage(_T("Message"), _T(""));
			InitDrive(_T("TestDrive"));
			InitScore();
			ChangeLang(m_CurrentLang);
			ChangeZoomType(m_ZoomType);
			UpdateData(TRUE);

			SetClientRect((DWORD) (m_SizeX * m_ZoomRatio), (DWORD) (m_SizeY * m_ZoomRatio));

			m_FlagShowWindow = TRUE;
			ChangeTheme(m_CurrentTheme);
			ChangeButtonStatus(TRUE);

			CenterWindow();
			ShowWindow(SW_SHOW);
			m_FlagInitializing = FALSE;
			once = TRUE;
		}
		else
		{
			InitDrive(_T("TestDrive"));
			InitScore();
			ChangeTheme(m_CurrentTheme);
		}
	}
}

void CDiskMarkDlg::InitDrive(CString ElementName)
{
	CString cstr;
	CString select;

	// list up drive
	TCHAR szDrives[256] = {0};
	LPTSTR pDrive = szDrives;
	TCHAR rootPath[4] = {0};
	TCHAR fileSystem[32] = {0};
	int count = 0;
	GetLogicalDriveStrings(255, szDrives);

	while( pDrive[0] != _T('\0') )
	{
		ULARGE_INTEGER freeBytesAvailableToCaller = {0};
		ULARGE_INTEGER totalNumberOfBytes = {0};
		ULARGE_INTEGER totalNumberOfFreeBytes = {0};

	//	_tcsupr_s(pDrive, sizeof(TCHAR) * 4);
		int result = GetDriveType(pDrive);
		
		/*
		if((result == DRIVE_REMOVABLE) && (pDrive[0] == 'A' || pDrive[0] == 'B'))
		{
			pDrive += _tcslen(pDrive) + 1;
			continue;
		}
		*/

		int forward = (int)_tcslen( pDrive );

		if(result == DRIVE_FIXED || result == DRIVE_REMOTE || result == DRIVE_REMOVABLE || result == DRIVE_RAMDISK)
		{
			pDrive[1] = _T('\0');
			cstr.Format(_T("%C: "), pDrive[0]);
			if(GetDiskFreeSpaceEx(cstr, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes) != 0)
			{
				cstr = _T("<option value=\"");
				cstr += pDrive;
				cstr += _T("\">");
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
				select += _T("</option>\n");

				if(m_TestDriveLetter == pDrive[0] - 'A')
				{
					m_IndexTestDrive = count;
				}

				count++;
			}
		}
		pDrive += forward + 1;
	}

	if(m_TestDriveLetter == 99)
	{
		m_IndexTestDrive = count;
	}

	m_MaxIndexTestDrive = count;

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
	cstr = i18n(_T("Menu"), _T("SETTINGS"));
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

	cstr = i18n(_T("Menu"), _T("FONT_SETTING"));
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

#ifdef _M_X64
	m_MesDiskSpdNotFound = L"Not found diskspd64.exe";//i18n(_T("Message"), _T("DISK_SPD_NOT_FOUND"));
#else
	m_MesDiskSpdNotFound = L"Not found diskspd32.exe";//i18n(_T("Message"), _T("DISK_SPD_NOT_FOUND"));
#endif

	InitDrive(_T("TestDrive"));

	m_TitleTestDrive = i18n(_T("Title"), _T("TEST_DRIVE"));
	m_TitleTestSize = i18n(_T("Title"), _T("TEST_SIZE"));
	m_TitleTestCount = i18n(_T("Title"), _T("TEST_COUNT"));
	if (m_MaxIndexTestDrive == m_IndexTestDrive)
	{
		ChangeSelectTitle(_T("TestDrive"), m_TitleTestDrive + L"(" + m_TestTargetPath + L")");
	}
	else
	{ 
		ChangeSelectTitle(_T("TestDrive"),	m_TitleTestDrive);
	}

	ChangeSelectTitle(_T("TestCount"), m_TitleTestCount);
	ChangeSelectTitle(_T("TestSize"),	m_TitleTestSize);

	WritePrivateProfileString(_T("Settings"), _T("Language"), LangName, m_Ini);

	SetClientRect((DWORD)(m_SizeX * m_ZoomRatio), (DWORD)(m_SizeY * m_ZoomRatio), 1);
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

	clip = _T("\
-----------------------------------------------------------------------\r\n\
%PRODUCT% %VERSION%%EDITION% (C) %COPY_YEAR% hiyohiyo\r\n\
                          Crystal Dew World : https://crystalmark.info/\r\n\
-----------------------------------------------------------------------\r\n\
* MB/s = 1,000,000 bytes/s [SATA/600 = 600,000,000 bytes/s]\r\n\
* KB = 1000 bytes, KiB = 1024 bytes\r\n\
\r\n\
%SequentialRead1%\r\n\
%SequentialWrite1%\r\n\
%RandomRead4KB1%\r\n\
%RandomWrite4KB1%\r\n\
%RandomRead4KB2%\r\n\
%RandomWrite4KB2%\r\n\
%RandomRead4KB3%\r\n\
%RandomWrite4KB3%\r\n\
\r\n\
  Test : %TestSize% (x%TestCount%) %TestMode% [%IntervalTime%]\r\n\
  Date : %Date%\r\n\
    OS : %OS%\r\n\
  %Comment%\
");

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

	cstr.Format(_T("   Sequential Read (Q=%3d,T=%2d) : %9.3f MB/s"), m_SequentialMultiQueues1, m_SequentialMultiThreads1, m_SequentialReadScore1);
	clip.Replace(_T("%SequentialRead1%"), cstr);
	cstr.Format(_T("  Sequential Write (Q=%3d,T=%2d) : %9.3f MB/s"), m_SequentialMultiQueues1, m_SequentialMultiThreads1, m_SequentialWriteScore1);
	clip.Replace(_T("%SequentialWrite1%"), cstr);
#ifdef SEQUENTIAL2
	cstr.Format(_T("   Sequential Read (Q=%3d,T=%2d) : %9.3f MB/s"), m_SequentialMultiQueues2, m_SequentialMultiThreads2, m_SequentialReadScore2);
	clip.Replace(_T("%SequentialRead2%"), cstr);
	cstr.Format(_T("  Sequential Write (Q=%3d,T=%2d) : %9.3f MB/s"), m_SequentialMultiQueues2, m_SequentialMultiThreads2, m_SequentialWriteScore2);
	clip.Replace(_T("%SequentialWrite2%"), cstr);
#endif
	cstr.Format(_T("  Random Read 4KiB (Q=%3d,T=%2d) : %9.3f MB/s [%9.1f IOPS]"), m_RandomMultiQueues1, m_RandomMultiThreads1, m_RandomRead4KBScore1, m_RandomRead4KBScore1 * 1000 * 1000 / 4096);
	clip.Replace(_T("%RandomRead4KB1%"), cstr);
	cstr.Format(_T(" Random Write 4KiB (Q=%3d,T=%2d) : %9.3f MB/s [%9.1f IOPS]"), m_RandomMultiQueues1, m_RandomMultiThreads1, m_RandomWrite4KBScore1, m_RandomWrite4KBScore1 * 1000 * 1000 / 4096);
	clip.Replace(_T("%RandomWrite4KB1%"), cstr);

	cstr.Format(_T("  Random Read 4KiB (Q=%3d,T=%2d) : %9.3f MB/s [%9.1f IOPS]"), m_RandomMultiQueues2, m_RandomMultiThreads2, m_RandomRead4KBScore2, m_RandomRead4KBScore2 * 1000 * 1000 / 4096);
	clip.Replace(_T("%RandomRead4KB2%"), cstr);
	cstr.Format(_T(" Random Write 4KiB (Q=%3d,T=%2d) : %9.3f MB/s [%9.1f IOPS]"), m_RandomMultiQueues2, m_RandomMultiThreads2, m_RandomWrite4KBScore2, m_RandomWrite4KBScore2 * 1000 * 1000 / 4096);
	clip.Replace(_T("%RandomWrite4KB2%"), cstr);

	cstr.Format(_T("  Random Read 4KiB (Q=%3d,T=%2d) : %9.3f MB/s [%9.1f IOPS]"), m_RandomMultiQueues3, m_RandomMultiThreads3, m_RandomRead4KBScore3, m_RandomRead4KBScore3 * 1000 * 1000 / 4096);
	clip.Replace(_T("%RandomRead4KB3%"), cstr);
	cstr.Format(_T(" Random Write 4KiB (Q=%3d,T=%2d) : %9.3f MB/s [%9.1f IOPS]"), m_RandomMultiQueues3, m_RandomMultiThreads3, m_RandomWrite4KBScore3, m_RandomWrite4KBScore3 * 1000 * 1000 / 4096);
	clip.Replace(_T("%RandomWrite4KB3%"), cstr);

	cstr.Format(_T("%d MiB [%s]"), _tstoi(m_ValueTestSize), m_TestDriveInfo);
	clip.Replace(_T("%TestSize%"), cstr);
	cstr.Format(_T("%d"), _tstoi(m_ValueTestCount));
	clip.Replace(_T("%TestCount%"), cstr);

	m_Comment.GetWindowText(cstr);
	if (cstr.IsEmpty())
	{
		clip.Replace(_T("%Comment%"), _T(""));
	}else
	{
		clip.Replace(_T("%Comment%"), _T("  ") + cstr + _T("\r\n"));
	}

	if(m_TestData == TEST_DATA_ALL0X00)
	{
		clip.Replace(_T("%TestMode%"), _T("") ALL_0X00_0FILL);
	}
	else
	{
		clip.Replace(_T("%TestMode%"), _T(""));
	}
	cstr.Format(L"Interval=%d sec", m_IntervalTime);
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
			fwrite(bom, sizeof(UCHAR), sizeof(bom), pFile);
			fwprintf(pFile, L"%s", (LPCTSTR)clip);
			fclose(pFile);
		}
	}
}

void CDiskMarkDlg::OnZoom75()
{
	if (CheckRadioZoomType(ID_ZOOM_75, 75))
	{
		UpdateDialogSize();
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

void CDiskMarkDlg::OnZoom400()
{
	if (CheckRadioZoomType(ID_ZOOM_400, 400))
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
	menu->CheckMenuRadioItem(ID_ZOOM_75, ID_ZOOM_AUTO, id, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_ZoomType = value;

	CString cstr;
	cstr.Format(_T("%d"), value);
	WritePrivateProfileString(_T("Settings"), _T("ZoomType"), cstr, m_Ini);

	ChangeZoomType(m_ZoomType);

	return TRUE;
}

void CDiskMarkDlg::CheckRadioZoomType()
{
	int id = ID_ZOOM_AUTO;

	switch(m_ZoomType)
	{
	case 75:  id = ID_ZOOM_75;	break;
	case 100: id = ID_ZOOM_100;	break;
	case 125: id = ID_ZOOM_125;	break;
	case 150: id = ID_ZOOM_150;	break;
	case 200: id = ID_ZOOM_200;	break;
	case 250: id = ID_ZOOM_250;	break;
	case 300: id = ID_ZOOM_300;	break;
	case 400: id = ID_ZOOM_400;	break;
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
	WritePrivateProfileString(_T("Settings"), _T("IntervalTime"), cstr, m_Ini);

	return TRUE;
}

void CDiskMarkDlg::CheckIntervalTime()
{
	int id = ID_INTERVAL_TIME_5;

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
	default:  id = ID_INTERVAL_TIME_5;	break;
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
	WritePrivateProfileString(_T("Settings"), _T("TestData"), _T("0"), m_Ini);
	SetWindowTitle(_T(""), _T(""));
}

void CDiskMarkDlg::OnModeAll0x00()
{
	CMenu *menu = GetMenu();
	menu->CheckMenuRadioItem(ID_MODE_DEFAULT, ID_MODE_ALL0X00, ID_MODE_ALL0X00, MF_BYCOMMAND);
	SetMenu(menu);
	DrawMenuBar();

	m_TestData = TEST_DATA_ALL0X00;
	WritePrivateProfileString(_T("Settings"), _T("TestData"), _T("1"), m_Ini);
	SetWindowTitle(_T(""), ALL_0X00_0FILL);
}

#ifdef SUISHO_SHIZUKU_SUPPORT
void CDiskMarkDlg::OnSize(UINT nType, int cx, int cy)
{
	CMainDialog::OnSize(nType, cx, cy);

	if(! m_FlagInitializing)
	{
		RECT rect;
		CString cstr;
		GetClientRect(&rect);
		if(rect.bottom - rect.top > 0)
		{
			m_SizeY = (DWORD)((rect.bottom - rect.top) / m_ZoomRatio);
			cstr.Format(_T("%d"), m_SizeY);
			WritePrivateProfileString(_T("Settings"), _T("Height"), cstr, m_Ini);
		}
	}
}

void CDiskMarkDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = (LONG) (SIZE_X * m_ZoomRatio + (GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(92/*SM_CXPADDEDBORDER*/)) * 2);
	lpMMI->ptMinTrackSize.y = (LONG)(SIZE_MIN_Y * m_ZoomRatio + GetSystemMetrics(SM_CYMENU)
							+ GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION));

	lpMMI->ptMaxTrackSize.x = (LONG) (SIZE_X * m_ZoomRatio + (GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(92/*SM_CXPADDEDBORDER*/)) * 2);
	lpMMI->ptMaxTrackSize.y = (LONG)(SIZE_Y * m_ZoomRatio + GetSystemMetrics(SM_CYMENU)
							+ GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION));

	CMainDialog::OnGetMinMaxInfo(lpMMI);
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
		SetControlFont();
		Invalidate();
		CString cstr;
		cstr.Format(L"%d", m_FontType);
		WritePrivateProfileString(_T("Setting"), _T("FontFace"), _T("\"") + m_FontFace + _T("\""), m_Ini);
		WritePrivateProfileString(_T("Setting"), _T("FontType"), cstr, m_Ini);
	}
}




void CDiskMarkDlg::OnBnClickedAll()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
