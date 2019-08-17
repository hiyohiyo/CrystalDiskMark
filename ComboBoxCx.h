/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

class CComboBoxCx : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxCx)

public:
	CComboBoxCx();
	virtual ~CComboBoxCx();

	// 初期化
	BOOL InitControl(int x, int y, int width, int height, double zoomRatio);
	void SetFontEx(CString face, int size, double zoomRatio);
	void SetFontHeight(int height, double zoomRatio);
	BOOL IsHighContrast();

	// ツールチップ関係
	void SetToolTipText(LPCTSTR pText);				// ツールチップのメッセージを設定する。
	void SetToolTipActivate(BOOL bActivate = TRUE);	// ツールチップの有効無効を切り替えます。
	void SetToolTipWindowText(LPCTSTR pText);		// コントロールとツールチップのメッセージを設定する。

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
	CFont    m_Font;
	DECLARE_MESSAGE_MAP()

	// ツールチップ関係
	void InitToolTip();					// ツールチップの初期化。
	CToolTipCtrl m_ToolTip;	// ツールチップ。
	CString m_ToolTipText;  // ツールチップ用テキスト

	INT m_FontHeight;

	// オーバーライド
private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};


