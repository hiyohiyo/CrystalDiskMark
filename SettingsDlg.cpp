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


// CSettingsDlg ダイアログ

IMPLEMENT_DYNCREATE(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogCx(CSettingsDlg::IDD, pParent)
{
	m_CurrentLangPath = ((CMainDialog*) pParent)->m_CurrentLangPath;
	m_DefaultLangPath = ((CMainDialog*)pParent)->m_DefaultLangPath;
	m_ZoomType = ((CMainDialog*)pParent)->GetZoomType();
	m_FontFace = ((CMainDialog*)pParent)->m_FontFace;

	_tcscpy_s(m_Ini, MAX_PATH, ((CDiskMarkApp*) AfxGetApp())->m_Ini);
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogCx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LABEL_BLOCK_SIZE, m_LabelBlockSize);
	DDX_Control(pDX, IDC_LABEL_QUEUES, m_LabelQueues);
	DDX_Control(pDX, IDC_LABEL_THREADS, m_LabelThreads);

	DDX_Control(pDX, IDC_SEQUENTIAL_LABEL_1, m_SequentialLabel1);
	DDX_Control(pDX, IDC_SEQUENTIAL_LABEL_2, m_SequentialLabel2);
	DDX_Control(pDX, IDC_RANDOM_LABEL_1, m_RandomLabel1);
	DDX_Control(pDX, IDC_RANDOM_LABEL_2, m_RandomLabel2);
	DDX_Control(pDX, IDC_RANDOM_LABEL_3, m_RandomLabel3);
	DDX_Control(pDX, IDC_AFFINITY_LABEL, m_LabelAffinity);

	DDX_Control(pDX, IDC_SET_DEFAULT, m_ButtonSetDefault);

	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_SIZE_1, m_ComboSequentialSize1);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_SIZE_2, m_ComboSequentialSize2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_SIZE_1, m_ComboRandomSize1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_SIZE_2, m_ComboRandomSize2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_SIZE_3, m_ComboRandomSize3);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_1, m_ComboSequentialQueues1);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_2, m_ComboSequentialQueues2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_QUEUE_1, m_ComboRandomQueues1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_QUEUE_2, m_ComboRandomQueues2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_QUEUE_3, m_ComboRandomQueues3);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_THREAD_1, m_ComboSequentialThreads1);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_THREAD_2, m_ComboSequentialThreads2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_THREAD_1, m_ComboRandomThreads1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_THREAD_2, m_ComboRandomThreads2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_THREAD_3, m_ComboRandomThreads3);

	DDX_Control(pDX, IDC_COMBO_AFFINITY, m_ComboAffinity);

}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogCx)
	ON_BN_CLICKED(IDC_SET_DEFAULT, &CSettingsDlg::OnSetDefault)
END_MESSAGE_MAP()

void CSettingsDlg::OnSetDefault()
{
	m_SequentialSize1 = 8;
	m_SequentialQueues1 = 1;
	m_SequentialThreads1 = 1;
	m_SequentialQueues2 = 32;
	m_SequentialThreads2 = 1;
	m_RandomQueues1 = 32;
	m_RandomThreads1 = 16;
	m_RandomQueues2 = 32;
	m_RandomThreads2 = 1;
	m_RandomQueues3 = 1;
	m_RandomThreads3 = 1;
	m_Affinity = 0;
	InitComboBox();
}

BOOL CSettingsDlg::OnInitDialog()
{
	CDialogCx::OnInitDialog();

	m_Affinity = GetPrivateProfileInt(_T("Setting"), _T("Affinity"), 0, m_Ini);
	if (m_Affinity < 0 || m_Affinity > 1)
	{
		m_Affinity = 0;
	}

	m_SequentialSize1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiSize1"), 8, m_Ini);
	if (m_SequentialSize1 <= 0 || m_SequentialSize1 > 8)
	{
		m_SequentialSize1 = 8;
	}
	
	m_SequentialQueues1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiQueues1"), 1, m_Ini);
	// if (m_SequentialQueues1 <= 0 || m_SequentialQueues1 > MAX_QUEUES)
	{
		m_SequentialQueues1 = 1;
	}
	m_SequentialThreads1 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiThreads1"), 1, m_Ini);
	if (m_SequentialThreads1 <= 0 || m_SequentialThreads1 > MAX_THREADS)
	{
		m_SequentialThreads1 = 1;
	}

	m_SequentialQueues2 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiQueues2"), 32, m_Ini);
	if (m_SequentialQueues2 <= 0 || m_SequentialQueues2 > MAX_QUEUES)
	{
		m_SequentialQueues2 = 32;
	}
	m_SequentialThreads2 = GetPrivateProfileInt(_T("Setting"), _T("SequentialMultiThreads2"), 1, m_Ini);
	if (m_SequentialThreads2 <= 0 || m_SequentialThreads2 > MAX_THREADS)
	{
		m_SequentialThreads2 = 1;
	}

	m_RandomQueues1 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiQueues1"), 32, m_Ini);
	if (m_RandomQueues1 <= 0 || m_RandomQueues1 > MAX_QUEUES)
	{
		m_RandomQueues1 = 32;
	}
	m_RandomThreads1 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiThreads1"), 16, m_Ini);
	if (m_RandomThreads1 <= 0 || m_RandomThreads1 > MAX_THREADS)
	{
		m_RandomThreads1 = 16;
	}
	m_RandomQueues2 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiQueues2"), 32, m_Ini);
	if (m_RandomQueues2 <= 0 || m_RandomQueues2 > MAX_QUEUES)
	{
		m_RandomQueues2 = 32;
	}
	m_RandomThreads2 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiThreads2"), 1, m_Ini);
	if (m_RandomThreads2 <= 0 || m_RandomThreads2 > MAX_THREADS)
	{
		m_RandomThreads2 = 1;
	}
	m_RandomQueues3 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiQueues3"), 1, m_Ini);
	if (m_RandomQueues3 <= 0 || m_RandomQueues3 > MAX_QUEUES)
	{
		m_RandomQueues3 = 1;
	}
	m_RandomThreads3 = GetPrivateProfileInt(_T("Setting"), _T("RandomMultiThreads3"), 1, m_Ini);
	if (m_RandomThreads3 <= 0 || m_RandomThreads3 > MAX_THREADS)
	{
		m_RandomThreads3 = 1;
	}

	InitComboBox();

	SetWindowText(i18n(_T("WindowTitle"), _T("QUEUES_THREADS")));

	UpdateDialogSize();

	return TRUE;
}

void CSettingsDlg::InitComboBox()
{
	// Size
	/*
	TCHAR blocksize[13][8] = { L"4KiB", L"8KiB", L"16KiB", L"32KiB", L"64KiB", L"128KiB",  L"256KiB", L"512KiB", L"1MiB", L"2MiB", L"4MiB", L"8MiB", L"16MiB" };
	for (int i = 0; i < 13; i++)
	{
		CString cstr;
		cstr.Format(L"%s", blocksize[i]);
	//	m_ComboSequentialSize1.AddString(cstr);
	//	m_ComboSequentialSize2.AddString(cstr);
	//	if (_ttoi(cstr.GetString()) == m_SequentialQueues1) { m_ComboSequentialSize1.SetCurSel(i); }
	//	m_ComboRandomSize1.AddString(cstr);
	//	m_ComboRandomSize2.AddString(cstr);
	//	m_ComboRandomSize3.AddString(cstr);
	}
	*/

	int blocksize[] = { 1, 2, 4, 8 };
	for (int i = 0; i < 4; i++)
	{
		CString cstr;
		cstr.Format(L"%dMiB", blocksize[i]);
		m_ComboSequentialSize1.AddString(cstr);
		if (m_SequentialSize1 == blocksize[i]) { m_ComboSequentialSize1.SetCurSel(i); }
	}
	
	m_ComboSequentialSize2.AddString(L"128KiB");
	m_ComboSequentialSize2.SetCurSel(0);
	m_ComboRandomSize1.AddString(L"4KiB");
	m_ComboRandomSize1.SetCurSel(0);
	m_ComboRandomSize2.AddString(L"4KiB");
	m_ComboRandomSize2.SetCurSel(0);
	m_ComboRandomSize3.AddString(L"4KiB");
	m_ComboRandomSize3.SetCurSel(0);

//	m_ComboSequentialSize1.EnableWindow(FALSE);
	m_ComboSequentialSize2.EnableWindow(FALSE);
	m_ComboRandomSize1.EnableWindow(FALSE);
	m_ComboRandomSize2.EnableWindow(FALSE);
	m_ComboRandomSize3.EnableWindow(FALSE);

	// Queues
	m_ComboSequentialQueues1.ResetContent();
	m_ComboSequentialQueues2.ResetContent();
	m_ComboRandomQueues1.ResetContent();
	m_ComboRandomQueues2.ResetContent();
	m_ComboRandomQueues3.ResetContent();

	int queues[10] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 };
	for (int i = 0; i < 10; i++)
	{
		CString cstr;
		cstr.Format(L"%d", queues[i]);
		m_ComboSequentialQueues2.AddString(cstr); if (m_SequentialQueues2 == queues[i]) { m_ComboSequentialQueues2.SetCurSel(i); }
		m_ComboRandomQueues1.AddString(cstr);     if (m_RandomQueues1 == queues[i]) { m_ComboRandomQueues1.SetCurSel(i); }
		m_ComboRandomQueues2.AddString(cstr);     if (m_RandomQueues2 == queues[i]) { m_ComboRandomQueues2.SetCurSel(i); }
		m_ComboRandomQueues3.AddString(cstr);     if (m_RandomQueues3 == queues[i]) { m_ComboRandomQueues3.SetCurSel(i); }
	}

	m_ComboSequentialQueues1.AddString(L"1");
	m_ComboSequentialQueues1.SetCurSel(0);

	m_ComboSequentialQueues1.EnableWindow(FALSE);

	// Threads
	m_ComboSequentialThreads1.ResetContent();
	m_ComboSequentialThreads2.ResetContent();
	m_ComboRandomThreads1.ResetContent();
	m_ComboRandomThreads2.ResetContent();
	m_ComboRandomThreads3.ResetContent();

	for (int i = 1; i <= 64; i++)
	{
		CString cstr;
		cstr.Format(L"%d", i);
		m_ComboSequentialThreads1.AddString(cstr); if (m_SequentialThreads1 == i) { m_ComboSequentialThreads1.SetCurSel(i - 1); }
		m_ComboSequentialThreads2.AddString(cstr); if (m_SequentialThreads2 == i) { m_ComboSequentialThreads2.SetCurSel(i - 1); }
		m_ComboRandomThreads1.AddString(cstr);     if (m_RandomThreads1 == i) { m_ComboRandomThreads1.SetCurSel(i - 1); }
		m_ComboRandomThreads2.AddString(cstr);     if (m_RandomThreads2 == i) { m_ComboRandomThreads2.SetCurSel(i - 1); }
		m_ComboRandomThreads3.AddString(cstr);     if (m_RandomThreads3 == i) { m_ComboRandomThreads3.SetCurSel(i - 1); }
	}

	m_ComboAffinity.AddString(L"OFF (-n)");
	m_ComboAffinity.AddString(L"ON (-ag)");

	if (m_Affinity == 1)
	{
		m_ComboAffinity.SetCurSel(1);
	}
	else
	{
		m_ComboAffinity.SetCurSel(0);
	}
}

void CSettingsDlg::OnCancel()
{
	CString cstr;
	m_ComboSequentialSize1.GetWindowTextW(cstr);
	cstr.Format(L"%d", _wtoi64(cstr.GetString()));
	WritePrivateProfileString(_T("Setting"), _T("SequentialMultiSize1"), cstr, m_Ini);
	m_ComboSequentialQueues1.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("SequentialMultiQueues1"), cstr, m_Ini);
	m_ComboSequentialThreads1.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("SequentialMultiThreads1"), cstr, m_Ini);
	m_ComboSequentialQueues2.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("SequentialMultiQueues2"), cstr, m_Ini);
	m_ComboSequentialThreads2.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("SequentialMultiThreads2"), cstr, m_Ini);
	m_ComboRandomQueues1.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("RandomMultiQueues1"), cstr, m_Ini);
	m_ComboRandomThreads1.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("RandomMultiThreads1"), cstr, m_Ini);
	m_ComboRandomQueues2.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("RandomMultiQueues2"), cstr, m_Ini);
	m_ComboRandomThreads2.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("RandomMultiThreads2"), cstr, m_Ini);
	m_ComboRandomQueues3.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("RandomMultiQueues3"), cstr, m_Ini);
	m_ComboRandomThreads3.GetWindowTextW(cstr);
	WritePrivateProfileString(_T("Setting"), _T("RandomMultiThreads3"), cstr, m_Ini);

	cstr.Format(L"%d", m_ComboAffinity.GetCurSel());
	WritePrivateProfileString(_T("Setting"), _T("Affinity"), cstr, m_Ini);

	CDialogCx::OnCancel();
}

void CSettingsDlg::UpdateDialogSize()
{
	ChangeZoomType(m_ZoomType);
	SetClientRect((DWORD)(SIZE_X * m_ZoomRatio), (DWORD)(SIZE_Y * m_ZoomRatio), 0);

	UpdateBackground();

	m_LabelBlockSize.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_LabelQueues.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_LabelThreads.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_LabelAffinity.SetFontEx(m_FontFace, 16, m_ZoomRatio);

	m_SequentialLabel1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_SequentialLabel2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_RandomLabel1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_RandomLabel2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_RandomLabel3.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialSize1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialSize2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialQueues1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialQueues2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialThreads1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialThreads2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomSize1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomSize2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomSize3.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomQueues1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomQueues2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomQueues3.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomThreads1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomThreads2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomThreads3.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboAffinity.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ButtonSetDefault.SetFontEx(m_FontFace, 16, m_ZoomRatio);

	m_LabelBlockSize.InitControl(176, 8, 120, 24, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_LabelQueues.InitControl(304, 8, 120, 24, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_LabelThreads.InitControl(432, 8, 120, 24, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_SequentialLabel1.InitControl(8, 40, 160, 24, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_SequentialLabel2.InitControl(8, 80, 160, 24, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_RandomLabel1.InitControl(8, 120, 160, 24, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_RandomLabel2.InitControl(8, 160, 160, 24, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_RandomLabel3.InitControl(8, 200, 160, 24, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_LabelAffinity.InitControl(8, 240, 160, 24, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_ComboSequentialSize1.InitControl(176, 40, 120, 32, m_ZoomRatio);
	m_ComboSequentialSize2.InitControl(176, 80, 120, 32, m_ZoomRatio);
	m_ComboRandomSize1.InitControl(176, 120, 120, 32, m_ZoomRatio);
	m_ComboRandomSize2.InitControl(176, 160, 120, 32, m_ZoomRatio);
	m_ComboRandomSize3.InitControl(176, 200, 120, 32, m_ZoomRatio);
	m_ComboSequentialQueues1.InitControl(304, 40, 120, 32, m_ZoomRatio);
	m_ComboSequentialQueues2.InitControl(304, 80, 120, 32, m_ZoomRatio);
	m_ComboRandomQueues1.InitControl(304, 120, 120, 32, m_ZoomRatio);
	m_ComboRandomQueues2.InitControl(304, 160, 120, 32, m_ZoomRatio);
	m_ComboRandomQueues3.InitControl(304, 200, 120, 32, m_ZoomRatio);
	m_ComboSequentialThreads1.InitControl(432, 40, 120, 32, m_ZoomRatio);
	m_ComboSequentialThreads2.InitControl(432, 80, 120, 32, m_ZoomRatio);
	m_ComboRandomThreads1.InitControl(432, 120, 120, 32, m_ZoomRatio);
	m_ComboRandomThreads2.InitControl(432, 160, 120, 32, m_ZoomRatio);
	m_ComboRandomThreads3.InitControl(432, 200, 120, 32, m_ZoomRatio);

	m_ComboAffinity.InitControl(176, 240, 120, 32, m_ZoomRatio);

	m_ButtonSetDefault.InitControl(432, 240, 120, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CButtonCx::OwnerDrawGlass | m_IsHighContrast);
	m_ButtonSetDefault.SetHandCursor(TRUE);
	m_ButtonSetDefault.SetWindowTextW(i18n(_T("Dialog"), _T("DEFAULT")));
	
	Invalidate();
}


