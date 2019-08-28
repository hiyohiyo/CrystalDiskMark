/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskMark.h"
#include "DiskMarkDlg.h"
#include "FontSelection.h"

int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam);

IMPLEMENT_DYNAMIC(CFontSelection, CDialog)

static CDiskMarkDlg *p;

CFontSelection::CFontSelection(CWnd* pParent)
	: CDialogCx(CFontSelection::IDD, pParent)
{
	p = (CDiskMarkDlg*)pParent;
	_tcscpy_s(m_Ini, MAX_PATH, ((CDiskMarkApp*)AfxGetApp())->m_Ini);

	m_CurrentLangPath = ((CMainDialog*)pParent)->m_CurrentLangPath;
	m_DefaultLangPath = ((CMainDialog*)pParent)->m_DefaultLangPath;
	m_ZoomType = ((CMainDialog*)pParent)->GetZoomType();
	m_FontFace = ((CMainDialog*)pParent)->m_FontFace;
	m_FontType = ((CMainDialog*)pParent)->m_FontType;

	m_CxThemeDir = ((CDiskMarkApp*)AfxGetApp())->m_ThemeDir;
	m_CxCurrentTheme = ((CMainDialog*)pParent)->m_CurrentTheme;
	m_CxDefaultTheme = ((CMainDialog*)pParent)->m_DefaultTheme;

	m_FontScale = ((CDiskMarkDlg*)pParent)->m_FontScale;
}

CFontSelection::~CFontSelection()
{
}
CStaticCx m_LabelFontFace;
CStaticCx m_LabelFontScale;
CStaticCx m_LabelFontType;
void CFontSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_OK, m_CtrlOk);
	DDX_Control(pDX, IDC_FONT_FACE, m_LabelFontFace);
	DDX_Control(pDX, IDC_FONT_SCALE, m_LabelFontScale);
	DDX_Control(pDX, IDC_FONT_TYPE, m_LabelFontType);
	DDX_Control(pDX, IDC_FONT_COMBO, m_FontComboBox);
	DDX_Control(pDX, IDC_FONT_SCALE_COMBO, m_FontScaleComboBox);
	DDX_Control(pDX, IDC_FONT_TYPE_COMBO, m_FontTypeComboBox);
	DDX_Control(pDX, IDC_SET_DEFAULT, m_ButtonSetDefault);
}


BEGIN_MESSAGE_MAP(CFontSelection, CDialogCx)
	ON_BN_CLICKED(ID_OK, &CFontSelection::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SET_DEFAULT, &CFontSelection::OnSetDefault)
END_MESSAGE_MAP()


BOOL CFontSelection::OnInitDialog()
{
	CDialogCx::OnInitDialog();

	SetWindowText(i18n(_T("WindowTitle"), _T("FONT_SETTING")));

    CClientDC dc(this);
    LOGFONT logfont; 
    ZeroMemory(&logfont, sizeof(LOGFONT)); 
    logfont.lfCharSet = ANSI_CHARSET;

    ::EnumFontFamiliesExW(dc.m_hDC, &logfont, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)&m_FontComboBox, 0);

	int no = m_FontComboBox.FindStringExact(0, m_FontFace);
	if(no >= 0)
	{
		m_FontComboBox.SetCurSel(no);
	}
	else
	{
		no = m_FontComboBox.FindStringExact(0, _T("Segoe UI"));
		if(no >= 0)
		{
			m_FontComboBox.SetCurSel(no);
		}
		else
		{
			no = m_FontComboBox.FindStringExact(0, _T("Tahoma"));
			if(no >= 0)
			{
				m_FontComboBox.SetCurSel(no);
			}
			else
			{
				m_FontComboBox.SetCurSel(0);
			}
		}
	}

	m_FontTypeComboBox.AddString(i18n(_T("Dialog"), _T("Auto")));
	m_FontTypeComboBox.AddString(L"GDI");
	m_FontTypeComboBox.AddString(L"GDI+");
	m_FontTypeComboBox.AddString(L"GDI+ w/o Descent");

//	m_FontTypeComboBox.AddString(L"DirectWrite");
	if (0 <= m_FontType && m_FontType <= 3)
	{
		m_FontTypeComboBox.SetCurSel(m_FontType);
	}
	else
	{
		m_FontTypeComboBox.SetCurSel(0);
	}

	CString cstr;

	for (int i = 50; i <= 150; i += 10)
	{
		cstr.Format(L"%d", i);
		m_FontScaleComboBox.AddString(cstr);
		if (m_FontScale == i) { m_FontScaleComboBox.SetCurSel(m_FontScaleComboBox.GetCount() - 1);  }
	}

	m_LabelFontFace.SetWindowTextW(i18n(L"Dialog", L"FONT_FACE"));
	m_LabelFontScale.SetWindowTextW(i18n(L"Dialog", L"FONT_SCALE"));
	m_LabelFontType.SetWindowTextW(i18n(L"Dialog", L"FONT_RENDER_METHOD"));

	m_ButtonSetDefault.SetWindowTextW(i18n(L"Dialog", L"DEFAULT"));

	UpdateDialogSize();

	return TRUE;
}

void CFontSelection::UpdateDialogSize()
{
	ChangeZoomType(m_ZoomType);
	SetClientRect((DWORD)(SIZE_X * m_ZoomRatio), (DWORD)(SIZE_Y * m_ZoomRatio), 0);

	UpdateBackground();
	
	m_LabelFontFace.SetFontEx(m_FontFace, 20, m_ZoomRatio);
	m_LabelFontScale.SetFontEx(m_FontFace, 20, m_ZoomRatio);
	m_LabelFontType.SetFontEx(m_FontFace, 20, m_ZoomRatio);

	m_FontComboBox.SetFontHeight(20, m_ZoomRatio);
	m_FontComboBox.SetFontEx(m_FontFace, 20, m_ZoomRatio);
	m_FontComboBox.SetItemHeight(-1, (UINT)(24 * m_ZoomRatio));
	for (int i = 0; i < m_FontComboBox.GetCount(); i++)
	{
		m_FontComboBox.SetItemHeight(i, (UINT)(20 * m_ZoomRatio));
	}

	m_FontScaleComboBox.SetFontEx(m_FontFace, 20, m_ZoomRatio);
	m_FontScaleComboBox.SetItemHeight(-1, (UINT)(24 * m_ZoomRatio));
	for (int i = 0; i < m_FontScaleComboBox.GetCount(); i++)
	{
		m_FontScaleComboBox.SetItemHeight(i, (UINT)(20 * m_ZoomRatio));
	}

	m_FontTypeComboBox.SetFontEx(m_FontFace, 20, m_ZoomRatio);
	m_FontTypeComboBox.SetItemHeight(-1, (UINT) (24 * m_ZoomRatio));
	for (int i = 0; i < m_FontTypeComboBox.GetCount(); i++)
	{
		m_FontTypeComboBox.SetItemHeight(i, (UINT) (20 * m_ZoomRatio));
	}

	m_ButtonSetDefault.SetFontEx(m_FontFace, 20, m_ZoomRatio);
	m_CtrlOk.SetFontEx(m_FontFace, 20, m_ZoomRatio);

	
	m_LabelFontFace.InitControl(8, 8, 160, 28, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_LabelFontScale.InitControl(8, 44, 160, 28, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_LabelFontType.InitControl(8, 80, 160, 28, m_ZoomRatio, NULL, 0, SS_RIGHT, CStaticCx::OwnerDrawTransparent | m_IsHighContrast);
	m_FontComboBox.MoveWindow((DWORD)(176 * m_ZoomRatio), (DWORD)(8 * m_ZoomRatio), (DWORD)(320 * m_ZoomRatio), (DWORD)(200 * m_ZoomRatio));
	m_FontScaleComboBox.MoveWindow((DWORD)(176 * m_ZoomRatio), (DWORD)(44 * m_ZoomRatio), (DWORD)(320 * m_ZoomRatio), (DWORD)(200 * m_ZoomRatio));
	m_FontTypeComboBox.MoveWindow((DWORD)(176 * m_ZoomRatio), (DWORD)(80 * m_ZoomRatio), (DWORD)(320 * m_ZoomRatio), (DWORD)(200 * m_ZoomRatio));

	m_ButtonSetDefault.InitControl(72, 120, 160, 28, m_ZoomRatio, NULL, 0, SS_CENTER, CButtonCx::OwnerDrawGlass | m_IsHighContrast);
	m_CtrlOk.InitControl(272, 120, 160, 28, m_ZoomRatio, NULL, 0, SS_CENTER, CButtonCx::OwnerDrawGlass | m_IsHighContrast);

	m_ButtonSetDefault.SetHandCursor();
	m_CtrlOk.SetHandCursor();

	Invalidate();
}

CString CFontSelection::GetFontFace()
{
	return m_FontFace;
}

INT CFontSelection::GetFontType()
{
	return m_FontType;
}

INT CFontSelection::GetFontScale()
{
	return m_FontScale;
}

int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam)
{
	CFontComboBox* pFontComboBox = (CFontComboBox*)lParam;
//	CComboBox* pFontComboBox = (CComboBox*)lParam;
	if(pFontComboBox->FindStringExact(0, (TCHAR*)lpelfe->elfLogFont.lfFaceName) == CB_ERR
	&& _tcschr((TCHAR*)lpelfe->elfLogFont.lfFaceName, _T('@')) == NULL
	)
	{
		pFontComboBox->AddString((TCHAR*)lpelfe->elfLogFont.lfFaceName);
	}
    return TRUE;
}

void CFontSelection::OnBnClickedOk()
{
	CString cstr;

	m_FontComboBox.GetLBText(m_FontComboBox.GetCurSel(), m_FontFace);
	m_FontType = m_FontTypeComboBox.GetCurSel();
	m_FontScaleComboBox.GetLBText(m_FontScaleComboBox.GetCurSel(), cstr);
	m_FontScale = _wtoi(cstr);

	CDialog::OnOK();
}


void CFontSelection::OnSetDefault()
{
	m_FontComboBox.ResetContent();

	CClientDC dc(this);
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = ANSI_CHARSET;

	::EnumFontFamiliesExW(dc.m_hDC, &logfont, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)& m_FontComboBox, 0);

	int no = m_FontComboBox.FindStringExact(0, _T("Segoe UI"));
	if (no >= 0)
	{
		m_FontComboBox.SetCurSel(no);
	}
	else
	{
		no = m_FontComboBox.FindStringExact(0, _T("Tahoma"));
		if (no >= 0)
		{
			m_FontComboBox.SetCurSel(no);
		}
		else
		{
			m_FontComboBox.SetCurSel(0);
		}
	}

	m_FontTypeComboBox.SetCurSel(0);
	m_FontScaleComboBox.SetCurSel(5);
}
