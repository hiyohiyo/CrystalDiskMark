/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

#define SAFE_DELETE(p) {if(p){delete (p);(p)=NULL;}}

class CComboBoxCx : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxCx)

	static const int HighContrast         = 0x0001;
	static const int SystemDraw           = 0x0002;
	static const int OwnerDrawImage       = 0x0004;
	static const int OwnerDrawGlass       = 0x0008;
	static const int OwnerDrawTransparent = 0x0010;
	static const int OwnerDrawGlassImage  = 0x0020;

	static const int FT_AUTO              = 0x0000;
	static const int FT_GDI               = 0x0001;
	static const int FT_GDI_PLUS_1        = 0x0002;
	static const int FT_GDI_PLUS_2        = 0x0003;
	static const int FT_GDI_PLUS_3		  = 0x0004;
	static const int FT_D_WRITE           = 0x0005;

public:
	CComboBoxCx();
	virtual ~CComboBoxCx();

	// 初期化
	BOOL InitControl(int x, int y, int width, int height, double zoomRatio);
	void SetFontHeight(int height, double zoomRatio);
	BOOL IsHighContrast();

	// ツールチップ関係
	void SetToolTipText(LPCTSTR pText);				// ツールチップのメッセージを設定する。
	void SetToolTipActivate(BOOL bActivate = TRUE);	// ツールチップの有効無効を切り替えます。
	void SetToolTipWindowText(LPCTSTR pText);		// コントロールとツールチップのメッセージを設定する。

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// フォント関係
	void SetFontEx(CString face, int size, double zoomRatio, BYTE textAlpha = 255, COLORREF textColor = RGB(0, 0, 0), LONG fontWeight = FW_NORMAL, int fontType = FT_GDI); // フォントを設定します。
	void SetMargin(int top, int left, int bottom, int right, double zoomRatio); // テキスト描画用のマージンを設定します。
	// カーソル関係
	void SetHandCursor(BOOL bHandCuror = TRUE);	// カーソルモードを設定します。

protected:
	// ホバー制御
	BOOL m_bHover;			// ホバー中のフラグ。
	BOOL m_bFocas;			// フォーカス中のフラグ。
	BOOL m_bTrackingNow;	// トラッキング中のフラグ。

	BOOL m_bSelected;		// 選択中のフラグ。

	// ツールチップ関係
	CToolTipCtrl m_ToolTip;	// ツールチップ。
	CString m_ToolTipText;  // ツールチップ用テキスト
	// フォント関係
	CFont m_Font;
	// GDI+ 関連
	Gdiplus::Font* m_GpFont;
	Gdiplus::SolidBrush* m_GpBrush;
	Gdiplus::StringFormat* m_GpStringformat;

	BOOL m_bHighContrast;	// ハイコントラスト用のフラグ
	UINT m_RenderMode;		// レンダリングモード
	UINT m_TextAlign;		// テキストアラインモード

	BOOL m_bHandCursor;		// ハンドカーソルモード
	CRect m_Margin;			// テキスト描画マージン
	int m_FontType;		    // 描画フォントの使用テクノロジー

	INT m_FontHeight;
	double m_ZoomRatio;

	// 内部関数。
private:
	void InitToolTip();					// ツールチップの初期化。
	virtual void DrawString(CString title, CDC* drawDC, LPDRAWITEMSTRUCT lpDrawItemStruct);


	// オーバーライド
private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnKillfocus();
	afx_msg void OnSetfocus();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};


