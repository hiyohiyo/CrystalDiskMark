// Settings.cpp : 実装ファイル
//

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

	_tcscpy_s(m_Ini, MAX_PATH, ((CDiskMarkApp*) AfxGetApp())->m_Ini);
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogCx::DoDataExchange(pDX);
	/*
	DDX_DHtml_SelectValue(pDX, _T("SequentialQueues1"), m_ValueSequentialQueues1);
	DDX_DHtml_SelectIndex(pDX, _T("SequentialQueues1"), m_IndexSequentialQueues1);
	DDX_DHtml_SelectValue(pDX, _T("SequentialThreads1"), m_ValueSequentialThreads1);
	DDX_DHtml_SelectIndex(pDX, _T("SequentialThreads1"), m_IndexSequentialThreads1);
#ifdef SEQUENTIAL2
	DDX_DHtml_SelectValue(pDX, _T("SequentialQueues2"), m_ValueSequentialQueues2);
	DDX_DHtml_SelectIndex(pDX, _T("SequentialQueues2"), m_IndexSequentialQueues2);
	DDX_DHtml_SelectValue(pDX, _T("SequentialThreads2"), m_ValueSequentialThreads2);
	DDX_DHtml_SelectIndex(pDX, _T("SequentialThreads2"), m_IndexSequentialThreads2);
#endif
	DDX_DHtml_SelectValue(pDX, _T("RandomQueues1"), m_ValueRandomQueues1);
	DDX_DHtml_SelectIndex(pDX, _T("RandomQueues1"), m_IndexRandomQueues1);
	DDX_DHtml_SelectValue(pDX, _T("RandomThreads1"), m_ValueRandomThreads1);
	DDX_DHtml_SelectIndex(pDX, _T("RandomThreads1"), m_IndexRandomThreads1);
	DDX_DHtml_SelectValue(pDX, _T("RandomQueues2"), m_ValueRandomQueues2);
	DDX_DHtml_SelectIndex(pDX, _T("RandomQueues2"), m_IndexRandomQueues2);
	DDX_DHtml_SelectValue(pDX, _T("RandomThreads2"), m_ValueRandomThreads2);
	DDX_DHtml_SelectIndex(pDX, _T("RandomThreads2"), m_IndexRandomThreads2);
	DDX_DHtml_SelectValue(pDX, _T("RandomQueues3"), m_ValueRandomQueues3);
	DDX_DHtml_SelectIndex(pDX, _T("RandomQueues3"), m_IndexRandomQueues3);
	DDX_DHtml_SelectValue(pDX, _T("RandomThreads3"), m_ValueRandomThreads3);
	DDX_DHtml_SelectIndex(pDX, _T("RandomThreads3"), m_IndexRandomThreads3);
	*/
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogCx)
END_MESSAGE_MAP()

HRESULT CSettingsDlg::OnDefaultSetting(IHTMLElement *pElement)
{
	m_SequentialQueues1 = 32;
	m_SequentialThreads1 = 1;
#ifdef SEQUENTIAL2
	m_SequentialQueues2 = 8;
	m_SequentialThreads2 = 1;
#endif
	m_RandomQueues1 = 8;
	m_RandomThreads1 = 8;
	m_RandomQueues2 = 32;
	m_RandomThreads2 = 1;
	m_RandomQueues3 = 1;
	m_RandomThreads3 = 1;
	UpdateData(FALSE);

	InitSelectBoxQ(L"SequentialQueues1", m_SequentialQueues1, MAX_QUEUES, &m_IndexSequentialQueues1);
	InitSelectBoxT(L"SequentialThreads1", m_SequentialThreads1, MAX_THREADS, &m_IndexSequentialThreads1);
#ifdef SEQUENTIAL2
	InitSelectBoxQ(L"SequentialQueues2", m_SequentialQueues2, MAX_QUEUES, &m_IndexSequentialQueues2);
	InitSelectBoxT(L"SequentialThreads2", m_SequentialThreads2, MAX_THREADS, &m_IndexSequentialThreads2);
#endif
	InitSelectBoxQ(L"RandomQueues1", m_RandomQueues1, MAX_QUEUES, &m_IndexRandomQueues1);
	InitSelectBoxT(L"RandomThreads1", m_RandomThreads1, MAX_THREADS, &m_IndexRandomThreads1);
	InitSelectBoxQ(L"RandomQueues2", m_RandomQueues2, MAX_QUEUES, &m_IndexRandomQueues2);
	InitSelectBoxT(L"RandomThreads2", m_RandomThreads2, MAX_THREADS, &m_IndexRandomThreads2);
	InitSelectBoxQ(L"RandomQueues3", m_RandomQueues3, MAX_QUEUES, &m_IndexRandomQueues3);
	InitSelectBoxT(L"RandomThreads3", m_RandomThreads3, MAX_THREADS, &m_IndexRandomThreads3);

	return TRUE;
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

#ifdef SEQUENTIAL2
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
#endif

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
	UpdateData(FALSE);

	SetWindowText(i18n(_T("WindowTitle"), _T("QUEUES_THREADS")));

	return TRUE;
}

void CSettingsDlg::InitSelectBoxQ(CString ElementName, int currentValue, int maxValue, long *index)
{
}

void CSettingsDlg::InitSelectBoxT(CString ElementName, int currentValue, int maxValue, long *index)
{

}
/*
void CSettingsDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CString cstr;
	cstr = szUrl;
	if (cstr.Find(_T("html")) != -1 || cstr.Find(_T("dlg")) != -1)
	{
		m_FlagShowWindow = TRUE;

		ChangeZoomType(m_ZoomType);
		SetClientRect((DWORD) (SIZE_X * m_ZoomRatio), (DWORD) (SIZE_Y * m_ZoomRatio), 0);

		InitSelectBoxQ(L"SequentialQueues1", m_SequentialQueues1, MAX_QUEUES, &m_IndexSequentialQueues1);
		InitSelectBoxT(L"SequentialThreads1", m_SequentialThreads1, MAX_THREADS, &m_IndexSequentialThreads1);
#ifdef SEQUENTIAL2
		InitSelectBoxQ(L"SequentialQueues2", m_SequentialQueues2, MAX_QUEUES, &m_IndexSequentialQueues2);
		InitSelectBoxT(L"SequentialThreads2", m_SequentialThreads2, MAX_THREADS, &m_IndexSequentialThreads2);
#endif
		InitSelectBoxQ(L"RandomQueues1", m_RandomQueues1, MAX_QUEUES, &m_IndexRandomQueues1);
		InitSelectBoxT(L"RandomThreads1", m_RandomThreads1, MAX_THREADS, &m_IndexRandomThreads1);
		InitSelectBoxQ(L"RandomQueues2", m_RandomQueues2, MAX_QUEUES, &m_IndexRandomQueues2);
		InitSelectBoxT(L"RandomThreads2", m_RandomThreads2, MAX_THREADS, &m_IndexRandomThreads2);
		InitSelectBoxQ(L"RandomQueues3", m_RandomQueues3, MAX_QUEUES, &m_IndexRandomQueues3);
		InitSelectBoxT(L"RandomThreads3", m_RandomThreads3, MAX_THREADS, &m_IndexRandomThreads3);

		UpdateData(FALSE);
		CenterWindow();
		ShowWindow(SW_SHOW);
	}
}
*/

void CSettingsDlg::OnCancel()
{
	UpdateData(TRUE);

	WritePrivateProfileString(_T("Settings"), _T("SequentialMultiQueues1"), m_ValueSequentialQueues1, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("SequentialMultiThreads1"), m_ValueSequentialThreads1, m_Ini);
#ifdef SEQUENTIAL2
	WritePrivateProfileString(_T("Settings"), _T("SequentialMultiQueues2"), m_ValueSequentialQueues2, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("SequentialMultiThreads2"), m_ValueSequentialThreads2, m_Ini);
#endif
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiQueues1"), m_ValueRandomQueues1, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiThreads1"), m_ValueRandomThreads1, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiQueues2"), m_ValueRandomQueues2, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiThreads2"), m_ValueRandomThreads2, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiQueues3"), m_ValueRandomQueues3, m_Ini);
	WritePrivateProfileString(_T("Settings"), _T("RandomMultiThreads3"), m_ValueRandomThreads3, m_Ini);

	CDialogCx::OnCancel();
}


