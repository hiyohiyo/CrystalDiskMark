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
//	DDX_Control(pDX, IDC_SEQUENTIAL_LABEL_2, m_SequentialLabel2);
	DDX_Control(pDX, IDC_RANDOM_LABEL_1, m_RandomLabel1);
	DDX_Control(pDX, IDC_RANDOM_LABEL_2, m_RandomLabel2);
	DDX_Control(pDX, IDC_RANDOM_LABEL_3, m_RandomLabel3);

	DDX_Control(pDX, IDC_SET_DEFAULT, m_ButtonSetDefault);

	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_SIZE_1, m_ComboSequentialSize1);
//	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_SIZE_2, m_ComboSequentialSize2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_SIZE_1, m_ComboRandomSize1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_SIZE_2, m_ComboRandomSize2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_SIZE_3, m_ComboRandomSize3);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_1, m_ComboSequentialQueues1);
//	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_2, m_ComboSequentialQueues2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_QUEUE_1, m_ComboRandomQueues1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_QUEUE_2, m_ComboRandomQueues2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_QUEUE_3, m_ComboRandomQueues3);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_THREAD_1, m_ComboSequentialThreads1);
//	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_THREAD_2, m_ComboSequentialThreads2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_THREAD_1, m_ComboRandomThreads1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_THREAD_2, m_ComboRandomThreads2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_THREAD_3, m_ComboRandomThreads3);
	
	DDX_CBIndex(pDX, IDC_COMBO_SEQUENTIAL_SIZE_1, m_IndexSequentialSize1);
//	DDX_CBIndex(pDX, IDC_COMBO_SEQUENTIAL_SIZE_2, m_IndexSequentialSize2);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_SIZE_1, m_IndexRandomSize1);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_SIZE_2, m_IndexRandomSize2);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_SIZE_3, m_IndexRandomSize3);
	DDX_CBIndex(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_1, m_IndexSequentialQueues1);
//	DDX_CBIndex(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_2, m_IndexSequentialQueues2);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_QUEUE_1, m_IndexRandomQueues1);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_QUEUE_2, m_IndexRandomQueues2);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_QUEUE_3, m_IndexRandomQueues3);
	DDX_CBIndex(pDX, IDC_COMBO_SEQUENTIAL_THREAD_1, m_IndexSequentialThreads1);
//	DDX_CBIndex(pDX, IDC_COMBO_SEQUENTIAL_THREAD_2, m_IndexSequentialThreads2);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_THREAD_1, m_IndexRandomThreads1);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_THREAD_2, m_IndexRandomThreads2);
	DDX_CBIndex(pDX, IDC_COMBO_RANDOM_THREAD_3, m_IndexRandomThreads3);

	DDX_Text(pDX, IDC_COMBO_SEQUENTIAL_SIZE_1, m_ValueSequentialSize1);
//	DDX_Text(pDX, IDC_COMBO_SEQUENTIAL_SIZE_2, m_ValueSequentialSize2);
	DDX_Text(pDX, IDC_COMBO_RANDOM_SIZE_1, m_ValueRandomSize1);
	DDX_Text(pDX, IDC_COMBO_RANDOM_SIZE_2, m_ValueRandomSize2);
	DDX_Text(pDX, IDC_COMBO_RANDOM_SIZE_3, m_ValueRandomSize3);
	DDX_Text(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_1, m_ValueSequentialQueues1);
//	DDX_Text(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_2, m_ValueSequentialQueues2);
	DDX_Text(pDX, IDC_COMBO_RANDOM_QUEUE_1, m_ValueRandomQueues1);
	DDX_Text(pDX, IDC_COMBO_RANDOM_QUEUE_2, m_ValueRandomQueues2);
	DDX_Text(pDX, IDC_COMBO_RANDOM_QUEUE_3, m_ValueRandomQueues3);
	DDX_Text(pDX, IDC_COMBO_SEQUENTIAL_THREAD_1, m_ValueSequentialThreads1);
//	DDX_Text(pDX, IDC_COMBO_SEQUENTIAL_THREAD_2, m_ValueSequentialThreads2);
	DDX_Text(pDX, IDC_COMBO_RANDOM_THREAD_1, m_ValueRandomThreads1);
	DDX_Text(pDX, IDC_COMBO_RANDOM_THREAD_2, m_ValueRandomThreads2);
	DDX_Text(pDX, IDC_COMBO_RANDOM_THREAD_3, m_ValueRandomThreads3);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogCx)
	ON_BN_CLICKED(IDC_SET_DEFAULT, &CSettingsDlg::OnSetDefault)
END_MESSAGE_MAP()

void CSettingsDlg::OnSetDefault()
{
	m_SequentialQueues1 = 32;
	m_SequentialThreads1 = 1;
//	m_SequentialQueues2 = 8;
//	m_SequentialThreads2 = 1;
	m_RandomQueues1 = 8;
	m_RandomThreads1 = 8;
	m_RandomQueues2 = 32;
	m_RandomThreads2 = 1;
	m_RandomQueues3 = 1;
	m_RandomThreads3 = 1;
	InitComboBox();
}

BOOL CSettingsDlg::OnInitDialog()
{
	CDialogCx::OnInitDialog();

	m_SequentialQueues1 = GetPrivateProfileInt(_T("Settings"), _T("SequentialMultiQueues1"), 32, m_Ini);
	if (m_SequentialQueues1 <= 0 || m_SequentialQueues1 > MAX_QUEUES)
	{
		m_SequentialQueues1 = 32;
	}
	m_SequentialThreads1 = GetPrivateProfileInt(_T("Settings"), _T("SequentialMultiThreads1"), 1, m_Ini);
	if (m_SequentialThreads1 <= 0 || m_SequentialThreads1 > MAX_THREADS)
	{
		m_SequentialThreads1 = 1;
	}
	/*
	m_SequentialQueues2 = GetPrivateProfileInt(_T("Settings"), _T("SequentialMultiQueues2"), 8, m_Ini);
	if (m_SequentialQueues2 <= 0 || m_SequentialQueues2 > MAX_QUEUES)
	{
		m_SequentialQueues2 = 8;
	}
	m_SequentialThreads2 = GetPrivateProfileInt(_T("Settings"), _T("SequentialMultiThreads2"), 1, m_Ini);
	if (m_SequentialThreads2 <= 0 || m_SequentialThreads2 > MAX_THREADS)
	{
		m_SequentialThreads2 = 1;
	}
	*/
	m_RandomQueues1 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiQueues1"), 8, m_Ini);
	if (m_RandomQueues1 <= 0 || m_RandomQueues1 > MAX_QUEUES)
	{
		m_RandomQueues1 = 8;
	}
	m_RandomThreads1 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiThreads1"), 8, m_Ini);
	if (m_RandomThreads1 <= 0 || m_RandomThreads1 > MAX_THREADS)
	{
		m_RandomThreads1 = 8;
	}
	m_RandomQueues2 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiQueues2"), 32, m_Ini);
	if (m_RandomQueues2 <= 0 || m_RandomQueues2 > MAX_QUEUES)
	{
		m_RandomQueues2 = 32;
	}
	m_RandomThreads2 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiThreads2"), 1, m_Ini);
	if (m_RandomThreads2 <= 0 || m_RandomThreads2 > MAX_THREADS)
	{
		m_RandomThreads2 = 1;
	}
	m_RandomQueues3 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiQueues3"), 1, m_Ini);
	if (m_RandomQueues3 <= 0 || m_RandomQueues3 > MAX_QUEUES)
	{
		m_RandomQueues3 = 1;
	}
	m_RandomThreads3 = GetPrivateProfileInt(_T("Settings"), _T("RandomMultiThreads3"), 1, m_Ini);
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

	m_ComboSequentialSize1.AddString(L"128KiB");
	m_ComboSequentialSize1.SetCurSel(0);
	m_ComboRandomSize1.AddString(L"4KiB");
	m_ComboRandomSize1.SetCurSel(0);
	m_ComboRandomSize2.AddString(L"4KiB");
	m_ComboRandomSize2.SetCurSel(0);
	m_ComboRandomSize3.AddString(L"4KiB");
	m_ComboRandomSize3.SetCurSel(0);

	// Queues
	m_ComboSequentialSize1.EnableWindow(FALSE);
//	m_ComboSequentialSize2.EnableWindow(FALSE);
	m_ComboRandomSize1.EnableWindow(FALSE);
	m_ComboRandomSize2.EnableWindow(FALSE);
	m_ComboRandomSize3.EnableWindow(FALSE);

	m_ComboSequentialQueues1.ResetContent();
//	m_ComboSequentialQueues2.ResetContent();
	m_ComboRandomQueues1.ResetContent();
	m_ComboRandomQueues2.ResetContent();
	m_ComboRandomQueues3.ResetContent();

	TCHAR queues[10][8] = { L"1", L"2", L"4", L"8", L"16", L"32", L"64", L"128", L"256", L"512" };
	for (int i = 0; i < 10; i++)
	{
		CString cstr;
		cstr.Format(L"%s", queues[i]);
		m_ComboSequentialQueues1.AddString(cstr); if (_ttoi(cstr.GetString()) == m_SequentialQueues1) { m_ComboSequentialQueues1.SetCurSel(i); }
		//	m_ComboSequentialQueues2.AddString(cstr); if (_ttoi(cstr.GetString()) == m_SequentialQueues2) { m_ComboSequentialQueues2.SetCurSel(i); }
		m_ComboRandomQueues1.AddString(cstr);     if (_ttoi(cstr.GetString()) == m_RandomQueues1) { m_ComboRandomQueues1.SetCurSel(i); }
		m_ComboRandomQueues2.AddString(cstr);     if (_ttoi(cstr.GetString()) == m_RandomQueues2) { m_ComboRandomQueues2.SetCurSel(i); }
		m_ComboRandomQueues3.AddString(cstr);     if (_ttoi(cstr.GetString()) == m_RandomQueues3) { m_ComboRandomQueues3.SetCurSel(i); }
	}
//	m_ComboSequentialQueues2.EnableWindow(FALSE);

	// Threads
	m_ComboSequentialThreads1.ResetContent();
//	m_ComboSequentialThreads2.ResetContent();
	m_ComboRandomThreads1.ResetContent();
	m_ComboRandomThreads2.ResetContent();
	m_ComboRandomThreads3.ResetContent();
	for (int i = 1; i <= 64; i++)
	{
		CString cstr;
		cstr.Format(L"%d", i);
		m_ComboSequentialThreads1.AddString(cstr); if (_ttoi(cstr.GetString()) == m_SequentialThreads1) { m_ComboSequentialThreads1.SetCurSel(i - 1); }
		//	m_ComboSequentialThreads2.AddString(cstr); if (_ttoi(cstr.GetString()) == m_SequentialThreads2) { m_ComboSequentialThreads2.SetCurSel(i - 1); }
		m_ComboRandomThreads1.AddString(cstr);     if (_ttoi(cstr.GetString()) == m_RandomThreads1) { m_ComboRandomThreads1.SetCurSel(i - 1); }
		m_ComboRandomThreads2.AddString(cstr);     if (_ttoi(cstr.GetString()) == m_RandomThreads2) { m_ComboRandomThreads2.SetCurSel(i - 1); }
		m_ComboRandomThreads3.AddString(cstr);     if (_ttoi(cstr.GetString()) == m_RandomThreads3) { m_ComboRandomThreads3.SetCurSel(i - 1); }
	}
//	m_ComboSequentialThreads2.EnableWindow(FALSE);
	UpdateData(FALSE);

}

void CSettingsDlg::OnCancel()
{
	UpdateData(TRUE);

	WritePrivateProfileString(_T("Settings"), _T("SequentialMultiQueues1"), m_ValueSequentialQueues1, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("SequentialMultiThreads1"), m_ValueSequentialThreads1, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("SequentialMultiQueues2"), m_ValueSequentialQueues2, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("SequentialMultiThreads2"), m_ValueSequentialThreads2, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiQueues1"), m_ValueRandomQueues1, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiThreads1"), m_ValueRandomThreads1, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiQueues2"), m_ValueRandomQueues2, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiThreads2"), m_ValueRandomThreads2, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiQueues3"), m_ValueRandomQueues3, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiThreads3"), m_ValueRandomThreads3, m_Ini);

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
	m_SequentialLabel1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
//	m_SequentialLabel2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_RandomLabel1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_RandomLabel2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_RandomLabel3.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialSize1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
//	m_ComboSequentialSize2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialQueues1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
//	m_ComboSequentialQueues2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboSequentialThreads1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
//	m_ComboSequentialThreads2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomSize1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomSize2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomSize3.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomQueues1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomQueues2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomQueues3.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomThreads1.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomThreads2.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ComboRandomThreads3.SetFontEx(m_FontFace, 16, m_ZoomRatio);
	m_ButtonSetDefault.SetFontEx(m_FontFace, 16, m_ZoomRatio);

	m_LabelBlockSize.InitControl(176, 8, 120, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_LabelQueues.InitControl(304, 8, 120, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_LabelThreads.InitControl(432, 8, 120, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_SequentialLabel1.InitControl(8, 48, 160, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
//	m_SequentialLabel2.InitControl(8, 88, 160, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_RandomLabel1.InitControl(8, 88, 160, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_RandomLabel2.InitControl(8, 128, 160, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_RandomLabel3.InitControl(8, 168, 160, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);

	m_ComboSequentialSize1.InitControl(176, 48, 120, 32, m_ZoomRatio);
//	m_ComboSequentialSize2.InitControl(176, 88, 120, 32, m_ZoomRatio);
	m_ComboRandomSize1.InitControl(176, 88, 120, 32, m_ZoomRatio);
	m_ComboRandomSize2.InitControl(176, 128, 120, 32, m_ZoomRatio);
	m_ComboRandomSize3.InitControl(176, 168, 120, 32, m_ZoomRatio);
	m_ComboSequentialQueues1.InitControl(304, 48, 120, 32, m_ZoomRatio);
//	m_ComboSequentialQueues2.InitControl(304, 88, 120, 32, m_ZoomRatio);
	m_ComboRandomQueues1.InitControl(304, 88, 120, 32, m_ZoomRatio);
	m_ComboRandomQueues2.InitControl(304, 128, 120, 32, m_ZoomRatio);
	m_ComboRandomQueues3.InitControl(304, 168, 120, 32, m_ZoomRatio);
	m_ComboSequentialThreads1.InitControl(432, 48, 120, 32, m_ZoomRatio);
//	m_ComboSequentialThreads2.InitControl(432, 88, 120, 32, m_ZoomRatio);
	m_ComboRandomThreads1.InitControl(432, 88, 120, 32, m_ZoomRatio);
	m_ComboRandomThreads2.InitControl(432, 128, 120, 32, m_ZoomRatio);
	m_ComboRandomThreads3.InitControl(432, 168, 120, 32, m_ZoomRatio);

	m_ButtonSetDefault.InitControl(432, 208, 120, 32, m_ZoomRatio, NULL, 0, SS_CENTER, CButtonCx::OwnerDrawGlass | m_IsHighContrast);
	m_ButtonSetDefault.SetHandCursor(TRUE);
	m_ButtonSetDefault.SetWindowTextW(i18n(_T("Dialog"), _T("DEFAULT")));
	
	Invalidate();
}


