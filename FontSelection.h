/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once
#include "FontComboBox.h"
#include "DialogCx.h"
#include "ButtonCx.h"
#include "ComboBoxCx.h"
#include "afxwin.h"

class CFontSelection : public CDialogCx
{
	DECLARE_DYNAMIC(CFontSelection)

	static const int SIZE_X = 600;
	static const int SIZE_Y = 196;

public:
	CFontSelection(CWnd* pParent = NULL);
	virtual ~CFontSelection();

	CString GetFontFace();
	INT GetFontType();
	INT GetFontScale();

	enum { IDD = IDD_FONT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

//	CString m_FontFace;
//	CString m_WindowTitle;

	int m_FontScale;

	CButtonCx m_CtrlOk;
	void OnSetDefault();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void UpdateDialogSize();

	CStaticCx m_LabelFontFace;
	CStaticCx m_LabelFontScale;
	CStaticCx m_LabelFontType;

	CFontComboBox m_FontComboBox;
	CComboBoxCx m_FontScaleComboBox;
	CComboBoxCx m_FontTypeComboBox;
	CButtonCx m_ButtonSetDefault;
	afx_msg void OnBnClickedOk();

};
