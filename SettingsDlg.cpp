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
	m_CurrentLangPath = p->GetCurrentLangPath();
	m_DefaultLangPath = p->GetDefaultLangPath();
	m_ThemeDir = p->GetThemeDir();
	m_CurrentTheme = p->GetCurrentTheme();
	m_DefaultTheme = p->GetDefaultTheme();
	m_Ini = p->GetIniPath();

	m_Profile = ((CDiskMarkDlg*)pParent)->m_Profile;
}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogFx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LABEL_BLOCK_SIZE, m_LabelBlockSize);
	DDX_Control(pDX, IDC_LABEL_QUEUES, m_LabelQueues);
	DDX_Control(pDX, IDC_LABEL_THREADS, m_LabelThreads);

	DDX_Control(pDX, IDC_SEQUENTIAL_LABEL_1, m_SequentialLabel1);
	DDX_Control(pDX, IDC_SEQUENTIAL_LABEL_2, m_SequentialLabel2);
	DDX_Control(pDX, IDC_RANDOM_LABEL_1, m_RandomLabel1);
	DDX_Control(pDX, IDC_RANDOM_LABEL_2, m_RandomLabel2);
	DDX_Control(pDX, IDC_AFFINITY_LABEL, m_LabelAffinity);

	DDX_Control(pDX, IDC_SET_DEFAULT, m_ButtonSetDefault);

	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_SIZE_1, m_ComboSequentialSize1);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_SIZE_2, m_ComboSequentialSize2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_SIZE_1, m_ComboRandomSize1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_SIZE_2, m_ComboRandomSize2);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_1, m_ComboSequentialQueues1);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_QUEUE_2, m_ComboSequentialQueues2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_QUEUE_1, m_ComboRandomQueues1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_QUEUE_2, m_ComboRandomQueues2);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_THREAD_1, m_ComboSequentialThreads1);
	DDX_Control(pDX, IDC_COMBO_SEQUENTIAL_THREAD_2, m_ComboSequentialThreads2);
	DDX_Control(pDX, IDC_COMBO_RANDOM_THREAD_1, m_ComboRandomThreads1);
	DDX_Control(pDX, IDC_COMBO_RANDOM_THREAD_2, m_ComboRandomThreads2);

	DDX_Control(pDX, IDC_COMBO_AFFINITY, m_ComboAffinity);
	DDX_Control(pDX, ID_OK, m_ButtonOk);

}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogFx)
	ON_BN_CLICKED(IDC_SET_DEFAULT, &CSettingsDlg::OnSetDefault)
	ON_BN_CLICKED(ID_OK, &CSettingsDlg::OnOk)
END_MESSAGE_MAP()

void CSettingsDlg::OnSetDefault()
{
	m_SequentialSize1 = 1;
	m_SequentialQueues1 = 8;
	m_SequentialThreads1 = 1;
	m_SequentialSize2 = 1;
	m_SequentialQueues2 = 1;
	m_SequentialThreads2 = 1;
	m_RandomSize1 = 4;
	m_RandomQueues1 = 32;
	m_RandomThreads1 = 16;
	m_RandomSize2 = 4;
	m_RandomQueues2 = 1;
	m_RandomThreads2 = 1;
	m_Affinity = 0;
	InitComboBox();
}

BOOL CSettingsDlg::OnInitDialog()
{
	CDialogFx::OnInitDialog();

	m_Affinity = GetPrivateProfileInt(L"Setting", L"Affinity", 0, m_Ini);
	if (m_Affinity < 0 || m_Affinity > 1)
	{
		m_Affinity = 0;
	}

	m_SequentialSize1 = GetPrivateProfileInt(L"Setting", L"SequentialSize1", 1, m_Ini);
	if (m_SequentialSize1 <= 0 || m_SequentialSize1 > 8)
	{
		m_SequentialSize1 = 1;
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

	m_SequentialSize2 = GetPrivateProfileInt(L"Setting", L"SequentialSize2", 1, m_Ini);
	if (m_SequentialSize2 <= 0 || m_SequentialSize2 > 8)
	{
		m_SequentialSize2 = 1;
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
	if (m_RandomSize1 <= 0 || m_RandomSize1 > 512)
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
	if (m_RandomSize2 <= 0 || m_RandomSize2 > 512)
	{
		m_RandomSize2 = 4;
	}
	m_RandomQueues2 = GetPrivateProfileInt(L"Setting", L"RandomQueues2", 32, m_Ini);
	if (m_RandomQueues2 <= 0 || m_RandomQueues2 > MAX_QUEUES)
	{
		m_RandomQueues2 = 32;
	}
	m_RandomThreads2 = GetPrivateProfileInt(L"Setting", L"RandomThreads2", 1, m_Ini);
	if (m_RandomThreads2 <= 0 || m_RandomThreads2 > MAX_THREADS)
	{
		m_RandomThreads2 = 1;
	}

	InitComboBox();

	m_LabelBlockSize.SetWindowTextW(i18n(L"Dialog", L"BLOCK_SIZE"));
	m_LabelQueues.SetWindowTextW(i18n(L"Dialog", L"QUEUES"));
	m_LabelThreads.SetWindowTextW(i18n(L"Dialog", L"THREADS"));
	m_LabelAffinity.SetWindowTextW(i18n(L"Dialog", L"DEFAULT_AFFINITY"));

	m_SequentialLabel1.SetWindowTextW(i18n(L"Dialog", L"SEQUENTIAL_1"));
	m_SequentialLabel2.SetWindowTextW(i18n(L"Dialog", L"SEQUENTIAL_2"));
	m_RandomLabel1.SetWindowTextW(i18n(L"Dialog", L"RANDOM_1"));
	m_RandomLabel2.SetWindowTextW(i18n(L"Dialog", L"RANDOM_2"));

	SetWindowText(i18n(L"WindowTitle", L"QUEUES_THREADS"));

	UpdateDialogSize();

	return TRUE;
}

void CSettingsDlg::InitComboBox()
{
	m_ComboSequentialSize1.ResetContent();
	m_ComboSequentialSize2.ResetContent();
	m_ComboRandomSize1.ResetContent();
	m_ComboRandomSize2.ResetContent();

	{
		int blocksize[] = { 1, 2, 4, 8 };
		for (int i = 0; i < 4; i++)
		{
			CString cstr;
			cstr.Format(L"%dMiB", blocksize[i]);
			m_ComboSequentialSize1.AddString(cstr);
			if (m_SequentialSize1 == blocksize[i]) { m_ComboSequentialSize1.SetCurSel(i); }
			m_ComboSequentialSize2.AddString(cstr);
			if (m_SequentialSize2 == blocksize[i]) { m_ComboSequentialSize2.SetCurSel(i); }
		}
	}

	{
		int blocksize[] = { 4, 8, 16, 32, 64, 128, 256, 512 };
		for (int i = 0; i < 8; i++)
		{
			CString cstr;
			cstr.Format(L"%dKiB", blocksize[i]);
			m_ComboRandomSize1.AddString(cstr);
			if (m_RandomSize1 == blocksize[i]) { m_ComboRandomSize1.SetCurSel(i); }
			m_ComboRandomSize2.AddString(cstr);
			if (m_RandomSize2 == blocksize[i]) { m_ComboRandomSize2.SetCurSel(i); }
		}
	}


	// Queues
	m_ComboSequentialQueues1.ResetContent();
	m_ComboSequentialQueues2.ResetContent();
	m_ComboRandomQueues1.ResetContent();
	m_ComboRandomQueues2.ResetContent();

	int queues[10] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 };
	// Sequential
	for (int i = 0; i < 5; i++)
	{
		CString cstr;
		cstr.Format(L"%d", queues[i]);
		m_ComboSequentialQueues1.AddString(cstr); if (m_SequentialQueues1 == queues[i]) { m_ComboSequentialQueues1.SetCurSel(i); }
		m_ComboSequentialQueues2.AddString(cstr); if (m_SequentialQueues2 == queues[i]) { m_ComboSequentialQueues2.SetCurSel(i); }
	}
	// Random
	for (int i = 0; i < 10; i++)
	{
		CString cstr;
		cstr.Format(L"%d", queues[i]);
		m_ComboRandomQueues1.AddString(cstr);     if (m_RandomQueues1 == queues[i]) { m_ComboRandomQueues1.SetCurSel(i); }
		m_ComboRandomQueues2.AddString(cstr);     if (m_RandomQueues2 == queues[i]) { m_ComboRandomQueues2.SetCurSel(i); }
	}

	// Threads
	m_ComboSequentialThreads1.ResetContent();
	m_ComboSequentialThreads2.ResetContent();
	m_ComboRandomThreads1.ResetContent();
	m_ComboRandomThreads2.ResetContent();

	for (int i = 1; i <= 64; i++)
	{
		CString cstr;
		cstr.Format(L"%d", i);
		m_ComboSequentialThreads1.AddString(cstr); if (m_SequentialThreads1 == i) { m_ComboSequentialThreads1.SetCurSel(i - 1); }
		m_ComboSequentialThreads2.AddString(cstr); if (m_SequentialThreads2 == i) { m_ComboSequentialThreads2.SetCurSel(i - 1); }
		m_ComboRandomThreads1.AddString(cstr);     if (m_RandomThreads1 == i) { m_ComboRandomThreads1.SetCurSel(i - 1); }
		m_ComboRandomThreads2.AddString(cstr);     if (m_RandomThreads2 == i) { m_ComboRandomThreads2.SetCurSel(i - 1); }
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


	if (m_Profile == 0/*PROFILE_DEFAULT*/ || m_Profile == 3/*PROFILE_DEFAULT_MIX*/)
	{

	}
	else if (m_Profile == 1/*PROFILE_PEAK*/ || m_Profile == 4/*PROFILE_PEAK_MIX*/)
	{
		m_ComboSequentialSize2.EnableWindow(FALSE);
		m_ComboRandomSize2.EnableWindow(FALSE);
		m_ComboSequentialQueues2.EnableWindow(FALSE);
		m_ComboRandomQueues2.EnableWindow(FALSE);
		m_ComboSequentialThreads2.EnableWindow(FALSE);
		m_ComboRandomThreads2.EnableWindow(FALSE);
	}
	else
	{
		m_ComboSequentialSize1.EnableWindow(FALSE);
		m_ComboSequentialSize2.EnableWindow(FALSE);
		m_ComboRandomSize1.EnableWindow(FALSE);
		m_ComboRandomSize2.EnableWindow(FALSE);
		m_ComboSequentialQueues1.EnableWindow(FALSE);
		m_ComboSequentialQueues2.EnableWindow(FALSE);
		m_ComboRandomQueues1.EnableWindow(FALSE);
		m_ComboRandomQueues2.EnableWindow(FALSE);
		m_ComboSequentialThreads1.EnableWindow(FALSE);
		m_ComboSequentialThreads2.EnableWindow(FALSE);
		m_ComboRandomThreads1.EnableWindow(FALSE);
		m_ComboRandomThreads2.EnableWindow(FALSE);
	}
}

void CSettingsDlg::OnOk()
{
	CString cstr;
	m_ComboSequentialSize1.GetWindowTextW(cstr);
	cstr.Format(L"%d", _wtoi(cstr.GetString()));
	WritePrivateProfileString(L"Setting", L"SequentialSize1", cstr, m_Ini);
	m_ComboSequentialQueues1.GetWindowTextW(cstr);
	WritePrivateProfileString(L"Setting", L"SequentialQueues1", cstr, m_Ini);
	m_ComboSequentialThreads1.GetWindowTextW(cstr);
	WritePrivateProfileString(L"Setting", L"SequentialThreads1", cstr, m_Ini);
	m_ComboSequentialSize2.GetWindowTextW(cstr);
	cstr.Format(L"%d", _wtoi(cstr.GetString()));
	WritePrivateProfileString(L"Setting", L"SequentialSize2", cstr, m_Ini);
	m_ComboSequentialQueues2.GetWindowTextW(cstr);
	WritePrivateProfileString(L"Setting", L"SequentialQueues2", cstr, m_Ini);
	m_ComboSequentialThreads2.GetWindowTextW(cstr);
	WritePrivateProfileString(L"Setting", L"SequentialThreads2", cstr, m_Ini);
	m_ComboRandomSize1.GetWindowTextW(cstr);
	cstr.Format(L"%d", _wtoi(cstr.GetString()));
	WritePrivateProfileString(L"Setting", L"RandomSize1", cstr, m_Ini);
	m_ComboRandomQueues1.GetWindowTextW(cstr);
	WritePrivateProfileString(L"Setting", L"RandomQueues1", cstr, m_Ini);
	m_ComboRandomThreads1.GetWindowTextW(cstr);
	WritePrivateProfileString(L"Setting", L"RandomThreads1", cstr, m_Ini);
	m_ComboRandomSize2.GetWindowTextW(cstr);
	cstr.Format(L"%d", _wtoi(cstr.GetString()));
	WritePrivateProfileString(L"Setting", L"RandomSize2", cstr, m_Ini);
	m_ComboRandomQueues2.GetWindowTextW(cstr);
	WritePrivateProfileString(L"Setting", L"RandomQueues2", cstr, m_Ini);
	m_ComboRandomThreads2.GetWindowTextW(cstr);
	WritePrivateProfileString(L"Setting", L"RandomThreads2", cstr, m_Ini);

	cstr.Format(L"%d", m_ComboAffinity.GetCurSel());
	WritePrivateProfileString(L"Setting", L"Affinity", cstr, m_Ini);

	CDialogFx::OnCancel();
}

void CSettingsDlg::OnCancel()
{
	CDialogFx::OnCancel();
}

void CSettingsDlg::UpdateDialogSize()
{
	ChangeZoomType(m_ZoomType);
	SetClientSize((int)(SIZE_X * m_ZoomRatio), (int)(SIZE_Y * m_ZoomRatio), 0);

	UpdateBackground();

	m_LabelBlockSize.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_LabelQueues.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_LabelThreads.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_LabelAffinity.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);

	m_SequentialLabel1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_SequentialLabel2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_RandomLabel1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_RandomLabel2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboSequentialSize1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboSequentialSize2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboSequentialQueues1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboSequentialQueues2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboSequentialThreads1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboSequentialThreads2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboRandomSize1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboRandomSize2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboRandomQueues1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboRandomQueues2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboRandomThreads1.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboRandomThreads2.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ComboAffinity.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ButtonSetDefault.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);
	m_ButtonOk.SetFontEx(m_FontFace, 16, 16, m_ZoomRatio, m_FontRatio);

	m_LabelBlockSize.InitControl(176, 12, 120, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_LabelQueues.InitControl(304, 12, 120, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);
	m_LabelThreads.InitControl(432, 12, 120, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent | m_bHighContrast);

	m_SequentialLabel1.InitControl(0, 40, 172, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_RIGHT, OwnerDrawTransparent | m_bHighContrast);
	m_SequentialLabel2.InitControl(0, 80, 172, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_RIGHT, OwnerDrawTransparent | m_bHighContrast);
	m_RandomLabel1.InitControl(0, 120, 172, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_RIGHT, OwnerDrawTransparent | m_bHighContrast);
	m_RandomLabel2.InitControl(0, 160, 172, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_RIGHT, OwnerDrawTransparent | m_bHighContrast);
	m_LabelAffinity.InitControl(0, 200, 172, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_RIGHT, OwnerDrawTransparent | m_bHighContrast);

	m_ComboSequentialSize1.InitControl(176, 40, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboSequentialSize2.InitControl(176, 80, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboRandomSize1.InitControl(176, 120, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboRandomSize2.InitControl(176, 160, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboSequentialQueues1.InitControl(304, 40, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboSequentialQueues2.InitControl(304, 80, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboRandomQueues1.InitControl(304, 120, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboRandomQueues2.InitControl(304, 160, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboSequentialThreads1.InitControl(432, 40, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboSequentialThreads2.InitControl(432, 80, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboRandomThreads1.InitControl(432, 120, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);
	m_ComboRandomThreads2.InitControl(432, 160, 120, 200, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, SystemDraw | m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);

	m_ComboAffinity.InitControl(176, 200, 200, 100, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, m_bHighContrast, m_ComboBk, m_ComboBkSelected, m_Glass, m_GlassAlpha);

	m_ButtonSetDefault.InitControl(120, 240, 120, 32, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw | m_bHighContrast);
	m_ButtonSetDefault.SetHandCursor();
	m_ButtonSetDefault.SetWindowTextW(i18n(L"Dialog", L"DEFAULT"));

	m_ButtonOk.InitControl(320, 240, 120, 32, m_ZoomRatio, &m_BkDC, NULL, 0, BS_CENTER, SystemDraw | m_bHighContrast);
	m_ButtonOk.SetHandCursor();
	
	Invalidate();
}


