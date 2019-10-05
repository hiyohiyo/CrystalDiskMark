/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once
#include "DialogCx.h"
#include "EditEx.h"

#define WM_USER_UPDATE_COMMENT	(WM_USER+4)

class CCommentDlg : public CDialogCx
{
	DECLARE_DYNCREATE(CCommentDlg)

#ifdef MIX_MODE
	static const int SIZE_X = 480;
	static const int SIZE_Y = 40;
#else
	static const int SIZE_X = 800;
	static const int SIZE_Y = 64;
#endif

public:
	CCommentDlg(CWnd* pParent = NULL);
	virtual ~CCommentDlg();

	enum { IDD = IDD_COMMENT };

	BOOL m_MixMode;
	int m_FontScale;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void UpdateDialogSize();

	DECLARE_MESSAGE_MAP()

public:
	CEditEx m_Comment;
	afx_msg void OnChangeComment();
};
