/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskMark.h"
#include "DiskMarkDlg.h"
#include "SettingsDlg.h"

IMPLEMENT_DYNCREATE(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogFx(CSettingsDlg::IDD, pParent)
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

	m_Profile = ((CDiskMarkDlg*)pParent)->m_Profile;
	m_MeasureTime = ((CDiskMarkDlg*)pParent)->m_MeasureTime;
	m_IntervalTime = ((CDiskMarkDlg*)pParent)->m_IntervalTime;
	m_TestData = ((CDiskMarkDlg*)pParent)->m_TestData;
	m_Affinity = ((CDiskMarkDlg*)pParent)->m_Affinity;
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogFx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LABEL_TYPE, m_LabelType);
	DDX_Control(pDX, IDC_LABEL_SIZE, m_LabelSize);
	DDX_Control(pDX, IDC_LABEL_QUEUES, m_LabelQueues);
	DDX_Control(pDX, IDC_LABEL_THREADS, m_LabelThreads);
	DDX_Control(pDX, IDC_LABEL_DEFAULT, m_LabelDefault);
	DDX_Control(pDX, IDC_LABEL_PEAK, m_LabelPeak);
	DDX_Control(pDX, IDC_LABEL_DEMO, m_LabelDemo);

	DDX_Control(pDX, IDC_LABEL_MEASURE_TIME, m_LabelMeasureTime);
	DDX_Control(pDX, IDC_LABEL_INTERVAL_TIME, m_LabelIntervalTime);
	DDX_Control(pDX, IDC_LABEL_AFFINITY, m_LabelAffinity);
	DDX_Control(pDX, IDC_LABEL_DATA, m_LabelData);

	DDX_Control(pDX, IDC_SET_NORM, m_ButtonSetNorm);
	DDX_Control(pDX, IDC_SET_NVME, m_ButtonSetNVMe);
	DDX_Control(pDX, IDC_SET_ATA, m_ButtonSetAta);

	DDX_Control(pDX, IDC_COMBO_BENCH_TYPE_0, m_ComboBenchType0);
	DDX_Control(pDX, IDC_COMBO_BENCH_TYPE_1, m_ComboBenchType1);
	DDX_Control(pDX, IDC_COMBO_BENCH_TYPE_2, m_ComboBenchType2);
	DDX_Control(pDX, IDC_COMBO_BENCH_TYPE_3, m_ComboBenchType3);
	DDX_Control(pDX, IDC_COMBO_BENCH_TYPE_4, m_ComboBenchType4);
	DDX_Control(pDX, IDC_COMBO_BENCH_TYPE_5, m_ComboBenchType5);
	DDX_Control(pDX, IDC_COMBO_BENCH_TYPE_8, m_ComboBenchType8);

	DDX_Control(pDX, IDC_COMBO_BENCH_SIZE_0, m_ComboBenchSize0);
	DDX_Control(pDX, IDC_COMBO_BENCH_SIZE_1, m_ComboBenchSize1);
	DDX_Control(pDX, IDC_COMBO_BENCH_SIZE_2, m_ComboBenchSize2);
	DDX_Control(pDX, IDC_COMBO_BENCH_SIZE_3, m_ComboBenchSize3);
	DDX_Control(pDX, IDC_COMBO_BENCH_SIZE_4, m_ComboBenchSize4);
	DDX_Control(pDX, IDC_COMBO_BENCH_SIZE_5, m_ComboBenchSize5);
	DDX_Control(pDX, IDC_COMBO_BENCH_SIZE_8, m_ComboBenchSize8);

	DDX_Control(pDX, IDC_COMBO_BENCH_QUEUE_0, m_ComboBenchQueues0);
	DDX_Control(pDX, IDC_COMBO_BENCH_QUEUE_1, m_ComboBenchQueues1);
	DDX_Control(pDX, IDC_COMBO_BENCH_QUEUE_2, m_ComboBenchQueues2);
	DDX_Control(pDX, IDC_COMBO_BENCH_QUEUE_3, m_ComboBenchQueues3);
	DDX_Control(pDX, IDC_COMBO_BENCH_QUEUE_4, m_ComboBenchQueues4);
	DDX_Control(pDX, IDC_COMBO_BENCH_QUEUE_5, m_ComboBenchQueues5);
	DDX_Control(pDX, IDC_COMBO_BENCH_QUEUE_8, m_ComboBenchQueues8);

	DDX_Control(pDX, IDC_COMBO_BENCH_THREAD_0, m_ComboBenchThreads0);
	DDX_Control(pDX, IDC_COMBO_BENCH_THREAD_1, m_ComboBenchThreads1);
	DDX_Control(pDX, IDC_COMBO_BENCH_THREAD_2, m_ComboBenchThreads2);
	DDX_Control(pDX, IDC_COMBO_BENCH_THREAD_3, m_ComboBenchThreads3);
	DDX_Control(pDX, IDC_COMBO_BENCH_THREAD_4, m_ComboBenchThreads4);
	DDX_Control(pDX, IDC_COMBO_BENCH_THREAD_5, m_ComboBenchThreads5);
	DDX_Control(pDX, IDC_COMBO_BENCH_THREAD_8, m_ComboBenchThreads8);

	DDX_Control(pDX, IDC_COMBO_DATA, m_ComboData);
	DDX_Control(pDX, IDC_COMBO_AFFINITY, m_ComboAffinity);
	DDX_Control(pDX, IDC_COMBO_MEASURE_TIME, m_ComboMeasureTime);
	DDX_Control(pDX, IDC_COMBO_INTERVAL_TIME, m_ComboIntervalTime);
	DDX_Control(pDX, IDC_OK, m_ButtonOk);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogFx)
	ON_BN_CLICKED(IDC_SET_NORM, &CSettingsDlg::OnSetNorm)
	ON_BN_CLICKED(IDC_SET_NVME, &CSettingsDlg::OnSetNVMe)
	ON_BN_CLICKED(IDC_SET_ATA, &CSettingsDlg::OnSetAta)
	ON_BN_CLICKED(IDC_OK, &CSettingsDlg::OnOk)
END_MESSAGE_MAP()

void CSettingsDlg::OnSetNorm()
{
	int type[9] =    {    0,    0,  1, 1,    0,  1,    0, 1,    0 };
	int size[9] =    { 1024, 1024,  4, 4, 1024,  4, 1024, 4, 1024 };
	int queues[9] =  {    8,    1, 32, 1,    8, 32,    1, 1,    8 };
	int threads[9] = {    1,    1,  1, 1,    1,  1,    1, 1,    1 };

	for (int i = 0; i < 9; i++)
	{
		m_BenchType[i] = type[i];
		m_BenchSize[i] = size[i];
		m_BenchQueues[i] = queues[i];
		m_BenchThreads[i] = threads[i];
	}

	m_TestData = 0;
	m_Affinity = 0;
	m_MeasureTime = 5;
	m_IntervalTime = 0;
	InitComboBox();
}

void CSettingsDlg::OnSetNVMe()
{
	int type[9] =    {    0,    0,  1, 1,    0,  1,    0, 1,    0 };
	int size[9] =    { 1024, 1024,  4, 4, 1024,  4, 1024, 4, 1024 };
	int queues[9] =  {    8,    1, 32, 1,    8, 32,    1, 1,    8 };
	int threads[9] = {    1,    1, 16, 1,    1, 16,    1, 1,    1 };

	for (int i = 0; i < 9; i++)
	{
		m_BenchType[i] = type[i];
		m_BenchSize[i] = size[i];
		m_BenchQueues[i] = queues[i];
		m_BenchThreads[i] = threads[i];
	}

	m_TestData = 0;
	m_Affinity = 0;
	m_MeasureTime = 5;
	m_IntervalTime = 0;

	InitComboBox();
}

void CSettingsDlg::OnSetAta()
{
	int type[9] =    {   0,   0,  1, 1,   0,  1,   0, 1,   0 };
	int size[9] =    { 128, 128,  4, 4, 128,  4, 128, 4, 128 };
	int queues[9] =  {  32,   1, 32, 1,  32, 32,   1, 1,  32 };
	int threads[9] = {   1,   1,  1, 1,   1,  1,   1, 1,   1 };

	for (int i = 0; i < 9; i++)
	{
		m_BenchType[i] = type[i];
		m_BenchSize[i] = size[i];
		m_BenchQueues[i] = queues[i];
		m_BenchThreads[i] = threads[i];
	}

	m_TestData = 0;
	m_Affinity = 0;
	m_MeasureTime = 5;
	m_IntervalTime = 0;

	InitComboBox();
}

BOOL CSettingsDlg::OnInitDialog()
{
	CDialogFx::OnInitDialog();

	CString cstr;

	int type[9] =    {    0,    0,  1, 1,    0,  1,    0, 1,    0 };
	int size[9] =    { 1024, 1024,  4, 4, 1024,  4, 1024, 4, 1024 };
	int queues[9] =  {    8,    1, 32, 1,    8, 32,    1, 1,    8 };
	int threads[9] = {    1,    1,  1, 1,    1,  1,    1, 1,    1 };

	for (int i = 0; i < 9; i++)
	{
		cstr.Format(L"BenchType%d", i);
		m_BenchType[i] = GetPrivateProfileInt(L"Setting", cstr, type[i], m_Ini);
		if (m_BenchType[i] < 0 || m_BenchSize[i] > 1) { m_BenchSize[i] = type[i]; }

		cstr.Format(L"BenchSize%d", i);
		m_BenchSize[i] = GetPrivateProfileInt(L"Setting", cstr, size[i], m_Ini);
		if (m_BenchSize[i] <= 0 || m_BenchSize[i] > 8192) { m_BenchSize[i] = size[i]; }

		cstr.Format(L"BenchQueues%d", i);
		m_BenchQueues[i] = GetPrivateProfileInt(L"Setting", cstr, queues[i], m_Ini);
		if (m_BenchQueues[i] <= 0 || m_BenchQueues[i] > MAX_QUEUES) { m_BenchQueues[i] = queues[i]; }

		cstr.Format(L"BenchThreads%d", i);
		m_BenchThreads[i] = GetPrivateProfileInt(L"Setting", cstr, threads[i], m_Ini);
		if (m_BenchThreads[i] <= 0 || m_BenchThreads[i] > MAX_THREADS) { m_BenchThreads[i] = threads[i]; }
	}

	m_Affinity = GetPrivateProfileInt(L"Setting", L"Affinity", 0, m_Ini);
	if (m_Affinity < 0 || m_Affinity > 1)
	{
		m_Affinity = 0;
	}

	m_TestData = GetPrivateProfileInt(L"Setting", L"TestData", 0, m_Ini);
	if (m_TestData < 0 || m_TestData > 1)
	{
		m_TestData = 0;
	}

	InitComboBox();

	m_LabelType.SetWindowTextW(i18n(L"Dialog", L"TYPE"));
	m_LabelSize.SetWindowTextW(i18n(L"Dialog", L"BLOCK_SIZE"));
	m_LabelQueues.SetWindowTextW(i18n(L"Dialog", L"QUEUES"));
	m_LabelThreads.SetWindowTextW(i18n(L"Dialog", L"THREADS"));
	m_LabelDefault.SetWindowTextW(L" " + i18n(L"Dialog", L"PROFILE_DEFAULT"));
	m_LabelPeak.SetWindowTextW(L" " + i18n(L"Dialog", L"PROFILE_PEAK_PERFORMANCE"));
	m_LabelDemo.SetWindowTextW(L" " + i18n(L"Dialog", L"PROFILE_DEMO"));
	m_LabelAffinity.SetWindowTextW(L" " + i18n(L"Dialog", L"DEFAULT_AFFINITY"));
	m_LabelData.SetWindowTextW(L" " + i18n(L"Menu", L"TEST_DATA"));
	m_LabelMeasureTime.SetWindowTextW(L" " + i18n(L"Dialog", L"MEASURE_TIME"));
	m_LabelIntervalTime.SetWindowTextW(L" " + i18n(L"Dialog", L"INTERVAL_TIME"));

	SetWindowText(i18n(L"WindowTitle", L"SETTINGS"));

	UpdateDialogSize();

	return TRUE;
}

void CSettingsDlg::InitComboBox()
{
	m_ComboBenchType0.ResetContent();
	m_ComboBenchType1.ResetContent();
	m_ComboBenchType2.ResetContent();
	m_ComboBenchType3.ResetContent();
	m_ComboBenchType4.ResetContent();
	m_ComboBenchType5.ResetContent();
	m_ComboBenchType8.ResetContent();

	for (int i = 0; i < 2; i++)
	{
		CString cstr;
		if (i == 0)
		{
			cstr.Format(L"SEQ");
		}
		else
		{
			cstr.Format(L"RND");
		}
		m_ComboBenchType0.AddString(cstr); if (m_BenchType[0] == i) { m_ComboBenchType0.SetCurSel(i); }
		m_ComboBenchType1.AddString(cstr); if (m_BenchType[1] == i) { m_ComboBenchType1.SetCurSel(i); }
		m_ComboBenchType2.AddString(cstr); if (m_BenchType[2] == i) { m_ComboBenchType2.SetCurSel(i); }
		m_ComboBenchType3.AddString(cstr); if (m_BenchType[3] == i) { m_ComboBenchType3.SetCurSel(i); }
		m_ComboBenchType4.AddString(cstr); if (m_BenchType[4] == i) { m_ComboBenchType4.SetCurSel(i); }
		m_ComboBenchType5.AddString(cstr); if (m_BenchType[5] == i) { m_ComboBenchType5.SetCurSel(i); }
		m_ComboBenchType8.AddString(cstr); if (m_BenchType[8] == i) { m_ComboBenchType8.SetCurSel(i); }
	}

	m_ComboBenchSize0.ResetContent();
	m_ComboBenchSize1.ResetContent();
	m_ComboBenchSize2.ResetContent();
	m_ComboBenchSize3.ResetContent();
	m_ComboBenchSize4.ResetContent();
	m_ComboBenchSize5.ResetContent();
	m_ComboBenchSize8.ResetContent();

	int blockSize[] = { 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
	for (int i = 0; i < 12; i++)
	{
		CString cstr;
		if (blockSize[i] >= 1024)
		{
			cstr.Format(L"%dMiB", blockSize[i] / 1024);
		}
		else
		{
			cstr.Format(L"%dKiB", blockSize[i]);
		}
		m_ComboBenchSize0.AddString(cstr); if (m_BenchSize[0] == blockSize[i]) { m_ComboBenchSize0.SetCurSel(i); }
		m_ComboBenchSize1.AddString(cstr); if (m_BenchSize[1] == blockSize[i]) { m_ComboBenchSize1.SetCurSel(i); }
		m_ComboBenchSize2.AddString(cstr); if (m_BenchSize[2] == blockSize[i]) { m_ComboBenchSize2.SetCurSel(i); }
		m_ComboBenchSize3.AddString(cstr); if (m_BenchSize[3] == blockSize[i]) { m_ComboBenchSize3.SetCurSel(i); }
		m_ComboBenchSize4.AddString(cstr); if (m_BenchSize[4] == blockSize[i]) { m_ComboBenchSize4.SetCurSel(i); }
		m_ComboBenchSize5.AddString(cstr); if (m_BenchSize[5] == blockSize[i]) { m_ComboBenchSize5.SetCurSel(i); }
		m_ComboBenchSize8.AddString(cstr); if (m_BenchSize[8] == blockSize[i]) { m_ComboBenchSize8.SetCurSel(i); }
	}

	// Queues
	m_ComboBenchQueues0.ResetContent();
	m_ComboBenchQueues1.ResetContent();
	m_ComboBenchQueues2.ResetContent();
	m_ComboBenchQueues3.ResetContent();
	m_ComboBenchQueues4.ResetContent();
	m_ComboBenchQueues5.ResetContent();
	m_ComboBenchQueues8.ResetContent();

	int queues[10] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 };
	// Sequential
	for (int i = 0; i < 10; i++)
	{
		CString cstr;
		cstr.Format(L"%d", queues[i]);
		m_ComboBenchQueues0.AddString(cstr); if (m_BenchQueues[0] == queues[i]) { m_ComboBenchQueues0.SetCurSel(i); }
		m_ComboBenchQueues1.AddString(cstr); if (m_BenchQueues[1] == queues[i]) { m_ComboBenchQueues1.SetCurSel(i); }
		m_ComboBenchQueues2.AddString(cstr); if (m_BenchQueues[2] == queues[i]) { m_ComboBenchQueues2.SetCurSel(i); }
		m_ComboBenchQueues3.AddString(cstr); if (m_BenchQueues[3] == queues[i]) { m_ComboBenchQueues3.SetCurSel(i); }
		m_ComboBenchQueues4.AddString(cstr); if (m_BenchQueues[4] == queues[i]) { m_ComboBenchQueues4.SetCurSel(i); }
		m_ComboBenchQueues5.AddString(cstr); if (m_BenchQueues[5] == queues[i]) { m_ComboBenchQueues5.SetCurSel(i); }
		m_ComboBenchQueues8.AddString(cstr); if (m_BenchQueues[8] == queues[i]) { m_ComboBenchQueues8.SetCurSel(i); }
	}

	// Threads
	m_ComboBenchThreads0.ResetContent();
	m_ComboBenchThreads1.ResetContent();
	m_ComboBenchThreads2.ResetContent();
	m_ComboBenchThreads3.ResetContent();
	m_ComboBenchThreads4.ResetContent();
	m_ComboBenchThreads5.ResetContent();
	m_ComboBenchThreads8.ResetContent();

	for (int i = 1; i <= 64; i++)
	{
		CString cstr;
		cstr.Format(L"%d", i);
		m_ComboBenchThreads0.AddString(cstr); if (m_BenchThreads[0] == i) { m_ComboBenchThreads0.SetCurSel(i - 1); }
		m_ComboBenchThreads1.AddString(cstr); if (m_BenchThreads[1] == i) { m_ComboBenchThreads1.SetCurSel(i - 1); }
		m_ComboBenchThreads2.AddString(cstr); if (m_BenchThreads[2] == i) { m_ComboBenchThreads2.SetCurSel(i - 1); }
		m_ComboBenchThreads3.AddString(cstr); if (m_BenchThreads[3] == i) { m_ComboBenchThreads3.SetCurSel(i - 1); }
		m_ComboBenchThreads4.AddString(cstr); if (m_BenchThreads[4] == i) { m_ComboBenchThreads4.SetCurSel(i - 1); }
		m_ComboBenchThreads5.AddString(cstr); if (m_BenchThreads[5] == i) { m_ComboBenchThreads5.SetCurSel(i - 1); }
		m_ComboBenchThreads8.AddString(cstr); if (m_BenchThreads[8] == i) { m_ComboBenchThreads8.SetCurSel(i - 1); }
	}

	m_ComboAffinity.ResetContent();
	m_ComboAffinity.AddString(i18n(L"Dialog", L"DISABLED") + L" (-n)");
	m_ComboAffinity.AddString(i18n(L"Dialog", L"ENABLED") + L" (-ag)");

	if (m_Affinity == 1)
	{
		m_ComboAffinity.SetCurSel(1);
	}
	else
	{
		m_ComboAffinity.SetCurSel(0);
	}

	m_ComboData.ResetContent();
	m_ComboData.AddString(i18n(L"Menu", L"DEFAULT_RANDOM"));
	m_ComboData.AddString(i18n(L"Menu", L"ALL_ZERO"));

	if (m_TestData == 1)
	{
		m_ComboData.SetCurSel(1);
	}
	else
	{
		m_ComboData.SetCurSel(0);
	}

	m_ComboMeasureTime.ResetContent();
	int measureTimes[] = { 5, 10, 20, 30, 60 };
	for (int i = 0; i < 5; i++)
	{
		CString cstr;
		cstr.Format(L"%d", measureTimes[i]);
		m_ComboMeasureTime.AddString(cstr); if (m_MeasureTime == measureTimes[i]) { m_ComboMeasureTime.SetCurSel(i); }
	}

	m_ComboIntervalTime.ResetContent();
	int intervalTimes[] = { 0, 1, 3, 5, 10, 30, 60, 180, 300, 600 };
	for (int i = 0; i < 10; i++)
	{
		CString cstr;
		cstr.Format(L"%d", intervalTimes[i]);
		m_ComboIntervalTime.AddString(cstr); if (m_IntervalTime == intervalTimes[i]) { m_ComboIntervalTime.SetCurSel(i); }
	}
}

int CSettingsDlg::GetType(CString text)
{
	if (text.FindOneOf(L"SEQ") != -1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int CSettingsDlg::GetBlockSize(CString text)
{
	if(text.FindOneOf(L"M") != -1)
	{
		return _wtoi(text.GetString()) * 1024;
	}
	else
	{
		return _wtoi(text.GetString());
	}
}

void CSettingsDlg::OnOk()
{
	CString cstr;

	m_ComboBenchType0.GetWindowTextW(cstr);	cstr.Format(L"%d", GetType(cstr)); WritePrivateProfileString(L"Setting", L"BenchType0", cstr, m_Ini);
	m_ComboBenchType1.GetWindowTextW(cstr);	cstr.Format(L"%d", GetType(cstr)); WritePrivateProfileString(L"Setting", L"BenchType1", cstr, m_Ini);
	m_ComboBenchType2.GetWindowTextW(cstr);	cstr.Format(L"%d", GetType(cstr)); WritePrivateProfileString(L"Setting", L"BenchType2", cstr, m_Ini);
	m_ComboBenchType3.GetWindowTextW(cstr);	cstr.Format(L"%d", GetType(cstr)); WritePrivateProfileString(L"Setting", L"BenchType3", cstr, m_Ini);
	m_ComboBenchType4.GetWindowTextW(cstr);	cstr.Format(L"%d", GetType(cstr)); WritePrivateProfileString(L"Setting", L"BenchType4", cstr, m_Ini);
	m_ComboBenchType5.GetWindowTextW(cstr);	cstr.Format(L"%d", GetType(cstr)); WritePrivateProfileString(L"Setting", L"BenchType5", cstr, m_Ini);
	m_ComboBenchType8.GetWindowTextW(cstr);	cstr.Format(L"%d", GetType(cstr)); WritePrivateProfileString(L"Setting", L"BenchType8", cstr, m_Ini);

	m_ComboBenchSize0.GetWindowTextW(cstr);	cstr.Format(L"%d", GetBlockSize(cstr)); WritePrivateProfileString(L"Setting", L"BenchSize0", cstr, m_Ini);
	m_ComboBenchSize1.GetWindowTextW(cstr);	cstr.Format(L"%d", GetBlockSize(cstr)); WritePrivateProfileString(L"Setting", L"BenchSize1", cstr, m_Ini);
	m_ComboBenchSize2.GetWindowTextW(cstr);	cstr.Format(L"%d", GetBlockSize(cstr)); WritePrivateProfileString(L"Setting", L"BenchSize2", cstr, m_Ini);
	m_ComboBenchSize3.GetWindowTextW(cstr);	cstr.Format(L"%d", GetBlockSize(cstr)); WritePrivateProfileString(L"Setting", L"BenchSize3", cstr, m_Ini);
	m_ComboBenchSize4.GetWindowTextW(cstr);	cstr.Format(L"%d", GetBlockSize(cstr)); WritePrivateProfileString(L"Setting", L"BenchSize4", cstr, m_Ini);
	m_ComboBenchSize5.GetWindowTextW(cstr);	cstr.Format(L"%d", GetBlockSize(cstr)); WritePrivateProfileString(L"Setting", L"BenchSize5", cstr, m_Ini);
	m_ComboBenchSize8.GetWindowTextW(cstr);	cstr.Format(L"%d", GetBlockSize(cstr)); WritePrivateProfileString(L"Setting", L"BenchSize8", cstr, m_Ini);

	m_ComboBenchQueues0.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchQueues0", cstr, m_Ini);
	m_ComboBenchQueues1.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchQueues1", cstr, m_Ini);
	m_ComboBenchQueues2.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchQueues2", cstr, m_Ini);
	m_ComboBenchQueues3.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchQueues3", cstr, m_Ini);
	m_ComboBenchQueues4.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchQueues4", cstr, m_Ini);
	m_ComboBenchQueues5.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchQueues5", cstr, m_Ini);
	m_ComboBenchQueues8.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchQueues8", cstr, m_Ini);

	m_ComboBenchThreads0.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchThreads0", cstr, m_Ini);
	m_ComboBenchThreads1.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchThreads1", cstr, m_Ini);
	m_ComboBenchThreads2.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchThreads2", cstr, m_Ini);
	m_ComboBenchThreads3.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchThreads3", cstr, m_Ini);
	m_ComboBenchThreads4.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchThreads4", cstr, m_Ini);
	m_ComboBenchThreads5.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchThreads5", cstr, m_Ini);
	m_ComboBenchThreads8.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"BenchThreads8", cstr, m_Ini);

	cstr.Format(L"%d", m_ComboData.GetCurSel()); WritePrivateProfileString(L"Setting", L"TestData", cstr, m_Ini);
	cstr.Format(L"%d", m_ComboAffinity.GetCurSel()); WritePrivateProfileString(L"Setting", L"Affinity", cstr, m_Ini);
	m_ComboMeasureTime.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"MeasureTime", cstr, m_Ini);
	m_ComboIntervalTime.GetWindowTextW(cstr); WritePrivateProfileString(L"Setting", L"IntervalTime", cstr, m_Ini);

	CDialogFx::OnCancel();
}

void CSettingsDlg::OnCancel()
{
	CDialogFx::OnCancel();
}

void CSettingsDlg::UpdateDialogSize()
{
	CDialogFx::UpdateDialogSize();

	ChangeZoomType(m_ZoomType);
	SetClientSize(SIZE_X, SIZE_Y, m_ZoomRatio);
	UpdateBackground(FALSE, m_bDarkMode);

#ifdef SUISHO_SHIZUKU_SUPPORT
	int fontSize = 16;
	int comboHeight = 24;
#else
	int fontSize = 12;
	int comboHeight = 20;
#endif

	m_LabelType.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelSize.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelQueues.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelThreads.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelDefault.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelPeak.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelDemo.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelMeasureTime.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelIntervalTime.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelAffinity.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);
	m_LabelData.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_LabelText, FW_NORMAL, m_FontRender);

	m_ComboBenchType0.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchType1.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchType2.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchType3.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchType4.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchType5.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchType8.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);

	m_ComboBenchSize0.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchSize1.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchSize2.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchSize3.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchSize4.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchSize5.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchSize8.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);

	m_ComboBenchQueues0.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchQueues1.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchQueues2.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchQueues3.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchQueues4.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchQueues5.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchQueues8.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);

	m_ComboBenchThreads0.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchThreads1.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchThreads2.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchThreads3.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchThreads4.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchThreads5.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboBenchThreads8.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);

	m_ComboData.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboAffinity.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboMeasureTime.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ComboIntervalTime.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_ComboText, m_ComboTextSelected, FW_NORMAL, m_FontRender);
	m_ButtonSetNorm.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_ButtonSetNVMe.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_ButtonSetAta.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);
	m_ButtonOk.SetFontEx(m_FontFace, fontSize, fontSize, m_ZoomRatio, m_FontRatio, m_MeterText, FW_BOLD, m_FontRender);

#ifdef SUISHO_SHIZUKU_SUPPORT
	m_LabelType.InitControl(8, 8, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_LabelSize.InitControl(176, 8, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_LabelQueues.InitControl(344, 8, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_LabelThreads.InitControl(512, 8, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_LabelDefault.InitControl(8, 32, 664, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelPeak.InitControl(8, 172, 664, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelDemo.InitControl(8, 256, 664, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelData.InitControl(344, 316, 328, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelAffinity.InitControl(8, 316, 328, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelMeasureTime.InitControl(8, 372, 328, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelIntervalTime.InitControl(344, 372, 328, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);

	m_ComboBenchType0.InitControl(8,  60, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType1.InitControl(8,  88, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType2.InitControl(8, 116, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType3.InitControl(8, 144, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType4.InitControl(8, 200, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType5.InitControl(8, 228, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType8.InitControl(8, 284, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ComboBenchSize0.InitControl(176,  60, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize1.InitControl(176,  88, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize2.InitControl(176, 116, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize3.InitControl(176, 144, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize4.InitControl(176, 200, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize5.InitControl(176, 228, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize8.InitControl(176, 284, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ComboBenchQueues0.InitControl(344,  60, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues1.InitControl(344,  88, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues2.InitControl(344, 116, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues3.InitControl(344, 144, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues4.InitControl(344, 200, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues5.InitControl(344, 228, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues8.InitControl(344, 284, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ComboBenchThreads0.InitControl(512,  60, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads1.InitControl(512,  88, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads2.InitControl(512, 116, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads3.InitControl(512, 144, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads4.InitControl(512, 200, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads5.InitControl(512, 228, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads8.InitControl(512, 284, 160, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ComboData.InitControl(352, 344, 320, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboAffinity.InitControl(16, 344, 320, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboMeasureTime.InitControl(16, 400, 320, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboIntervalTime.InitControl(352, 400, 320, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ButtonSetNorm.InitControl(16, 440, 152, 32, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_ButtonSetNVMe.InitControl(184, 440, 152, 32, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_ButtonSetAta.InitControl(352, 440, 152, 32, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_ButtonOk.InitControl(512, 436, 160, 36, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
#else
	m_LabelType.InitControl(8, 8, 100, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_LabelSize.InitControl(116, 8, 100, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_LabelQueues.InitControl(224, 8, 100, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_LabelThreads.InitControl(332, 8, 100, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode);
	m_LabelDefault.InitControl(8, 28, 424, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelPeak.InitControl(8, 148, 424, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelDemo.InitControl(8, 220, 424, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelAffinity.InitControl(8, 272, 208, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelData.InitControl(224, 272, 208, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelMeasureTime.InitControl(8, 320, 208, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_LabelIntervalTime.InitControl(224, 320, 208, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, SystemDraw, m_bHighContrast, m_bDarkMode);

	m_ComboBenchType0.InitControl(8, 52, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType1.InitControl(8, 76, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType2.InitControl(8, 100, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType3.InitControl(8, 124, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType4.InitControl(8, 172, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType5.InitControl(8, 196, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchType8.InitControl(8, 244, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ComboBenchSize0.InitControl(116, 52, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize1.InitControl(116, 76, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize2.InitControl(116, 100, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize3.InitControl(116, 124, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize4.InitControl(116, 172, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize5.InitControl(116, 196, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchSize8.InitControl(116, 244, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ComboBenchQueues0.InitControl(224, 52, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues1.InitControl(224, 76, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues2.InitControl(224, 100, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues3.InitControl(224, 124, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues4.InitControl(224, 172, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues5.InitControl(224, 196, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchQueues8.InitControl(224, 244, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ComboBenchThreads0.InitControl(332, 52, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads1.InitControl(332, 76, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads2.InitControl(332, 100, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads3.InitControl(332, 124, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads4.InitControl(332, 172, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads5.InitControl(332, 196, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboBenchThreads8.InitControl(332, 244, 100, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ComboData.InitControl(232, 296, 200, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboAffinity.InitControl(16, 296, 200, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboMeasureTime.InitControl(16, 344, 200, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_ComboIntervalTime.InitControl(232, 344, 200, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);

	m_ButtonSetNorm.InitControl(12, 376, 96, 24, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_ButtonSetAta.InitControl(120, 376, 96, 24, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_ButtonSetNVMe.InitControl(228, 376, 96, 24, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
	m_ButtonOk.InitControl(332, 372, 100, 28, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode);
#endif

	m_ButtonSetNorm.SetHandCursor();
	m_ButtonSetNVMe.SetHandCursor();
	m_ButtonSetAta.SetHandCursor();
	m_ButtonOk.SetHandCursor();

	m_ButtonSetNorm.SetWindowTextW(L"NORM");
	m_ButtonSetNVMe.SetWindowTextW(L"NVMe");
	m_ButtonSetAta.SetWindowTextW(L"ATA");

	SetDarkModeControl(m_ButtonOk.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_ButtonSetNorm.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_ButtonSetNVMe.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_ButtonSetAta.GetSafeHwnd(), m_bDarkMode);

	m_ComboBenchType0.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchType1.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchType2.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchType3.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchType4.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchType5.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchType8.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);

	m_ComboBenchSize0.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchSize1.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchSize2.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchSize3.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchSize4.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchSize5.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchSize8.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);

	m_ComboBenchQueues0.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchQueues1.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchQueues2.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchQueues3.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchQueues4.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchQueues5.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchQueues8.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);

	m_ComboBenchThreads0.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchThreads1.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchThreads2.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchThreads3.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchThreads4.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchThreads5.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboBenchThreads8.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);

	m_ComboData.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboAffinity.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboMeasureTime.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);
	m_ComboIntervalTime.SetItemHeightAll(comboHeight, m_ZoomRatio, m_FontRatio);

	m_ComboBenchType0.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchType1.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchType2.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchType3.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchType4.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchType5.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchType8.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	m_ComboBenchSize0.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchSize1.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchSize2.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchSize3.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchSize4.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchSize5.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchSize8.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	m_ComboBenchQueues0.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchQueues1.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchQueues2.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchQueues3.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchQueues4.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchQueues5.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchQueues8.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	m_ComboBenchThreads0.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchThreads1.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchThreads2.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchThreads3.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchThreads4.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchThreads5.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboBenchThreads8.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	m_ComboData.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboAffinity.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboMeasureTime.SetMargin(0, 4, 0, 0, m_ZoomRatio);
	m_ComboIntervalTime.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	m_ComboBenchType4.EnableWindow(FALSE);
	m_ComboBenchType5.EnableWindow(FALSE);

	Invalidate();
}