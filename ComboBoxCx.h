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

	void SetFontEx(CString face, int size, double zoomRatio);
	BOOL IsHighContrast();

	// ツールチップ関係
	void SetToolTipText(LPCTSTR pText);				// ツールチップのメッセージを設定する。
	void SetToolTipActivate(BOOL bActivate = TRUE);	// ツールチップの有効無効を切り替えます。
	void SetToolTipWindowText(LPCTSTR pText);		// コントロールとツールチップのメッセージを設定する。

protected:
	CFont    m_Font;
	DECLARE_MESSAGE_MAP()

	// ツールチップ関係
	void InitToolTip();					// ツールチップの初期化。
	CToolTipCtrl m_ToolTip;	// ツールチップ。
	CString m_ToolTipText;  // ツールチップ用テキスト

	// オーバーライド
private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


