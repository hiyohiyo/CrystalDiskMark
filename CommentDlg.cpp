/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskMark.h"
#include "DiskMarkDlg.h"
#include "CommentDlg.h"

IMPLEMENT_DYNCREATE(CCommentDlg, CDialog)

CCommentDlg::CCommentDlg(CWnd* pParent /*=NULL*/)
	: CDialogCx(CCommentDlg::IDD, pParent)
{
	m_CurrentLangPath = ((CMainDialog*)pParent)->m_CurrentLangPath;
	m_DefaultLangPath = ((CMainDialog*)pParent)->m_DefaultLangPath;
	m_ZoomType = ((CMainDialog*)pParent)->GetZoomType();
	m_MixMode = ((CDiskMarkDlg*)pParent)->m_MixMode;
	m_FontFace = ((CMainDialog*)pParent)->m_FontFace;
	m_FontScale = ((CDiskMarkDlg*)pParent)->m_FontScale;
}

CCommentDlg::~CCommentDlg()
{
}

void CCommentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogCx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMENT, m_Comment);
}

BOOL CCommentDlg::OnInitDialog()
{
	CDialogCx::OnInitDialog();

	SetWindowText(i18n(_T("WindowTitle"), _T("COMMENT")));

	m_Comment.SetWindowTextW(((CDiskMarkDlg*)GetParent())->m_CommentExchange);

	UpdateDialogSize();

	CenterWindow();
	CRect parentRect;
	CRect commentRect;
	((CDiskMarkDlg*)GetParent())->GetWindowRect(&parentRect);
	GetWindowRect(&commentRect);
	commentRect.MoveToXY(parentRect.right - commentRect.Width(), parentRect.bottom - commentRect.Height());

	MoveWindow(&commentRect, TRUE);
	
	ShowWindow(SW_SHOW);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCommentDlg, CDialogCx)
	ON_EN_UPDATE(IDC_COMMENT, &CCommentDlg::OnChangeComment)
END_MESSAGE_MAP()

void CCommentDlg::UpdateDialogSize()
{
	double scale = m_FontScale / 100.0;

	ChangeZoomType(m_ZoomType);

#ifdef MIX_MODE
	if (m_MixMode)
	{
		SetClientRect((DWORD)((SIZE_X + 200)* m_ZoomRatio), (DWORD)(SIZE_Y * m_ZoomRatio), 0);
		m_Comment.MoveWindow((int)((8) * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(664 * m_ZoomRatio), (int)(24 * m_ZoomRatio));
	}
	else
	{
		SetClientRect((DWORD)(SIZE_X * m_ZoomRatio), (DWORD)(SIZE_Y * m_ZoomRatio), 0);
		m_Comment.MoveWindow((int)(8 * m_ZoomRatio), (int)(8 * m_ZoomRatio), (int)(464 * m_ZoomRatio), (int)(24 * m_ZoomRatio));
	}
	m_Comment.SetFontEx(m_FontFace, (int)(16 * scale), m_ZoomRatio, FW_BOLD);
#else
	SetClientRect((DWORD)(SIZE_X * m_ZoomRatio), (DWORD)(SIZE_Y * m_ZoomRatio), 0);
	m_Comment.MoveWindow((int)(12 * m_ZoomRatio), (int)(12 * m_ZoomRatio), (int)(776 * m_ZoomRatio), (int)(40 * m_ZoomRatio));
	m_Comment.SetFontEx(m_FontFace, (int)(28 * scale), m_ZoomRatio, FW_BOLD);
#endif


	UpdateBackground(true);

	Invalidate();
}

void CCommentDlg::OnChangeComment()
{
	static CString cstr;
	m_Comment.GetWindowTextW(cstr);
	
	::PostMessage(GetParent()->GetSafeHwnd(), WM_USER_UPDATE_COMMENT, (WPARAM)&cstr, 0);
}