/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "ComboBoxCx.h"

IMPLEMENT_DYNAMIC(CComboBoxCx, CComboBox)

BOOL CComboBoxCx::IsHighContrast()
{
	HIGHCONTRAST hc;
	hc.cbSize = sizeof(HIGHCONTRAST);
	SystemParametersInfo(SPI_GETHIGHCONTRAST, sizeof(HIGHCONTRAST), &hc, 0);
	return hc.dwFlags & HCF_HIGHCONTRASTON;
}

CComboBoxCx::CComboBoxCx()
{
}

CComboBoxCx::~CComboBoxCx()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CComboBoxCx, CComboBox)
END_MESSAGE_MAP()

void CComboBoxCx::SetFontEx(CString face, int size, double zoomRatio)
{
	LOGFONT logFont = {0};
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = (LONG)(-1 * size * zoomRatio);
	logFont.lfQuality = 6;
	if(face.GetLength() < 32)
	{
		wsprintf(logFont.lfFaceName, _T("%s"), face.GetString());
	}
	else
	{
		wsprintf(logFont.lfFaceName, _T(""));
	}

	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&logFont);
	SetFont(&m_Font);

	SetItemHeight(-1, (UINT)(size * zoomRatio * 1.25));
}

//------------------------------------------------
// ツールチップ関連
//------------------------------------------------

// ツールチップを初期化する。
void CComboBoxCx::InitToolTip()
{
	// ツールチップは未初期化？
	if (m_ToolTip.m_hWnd == NULL)
	{
		// ツールチップを初期化する。
		m_ToolTip.Create(this, TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOANIMATE | TTS_NOFADE);
		// ツールチップを無効にする。
		m_ToolTip.Activate(FALSE);
		// ツールチップにフォントを設定します。
		m_ToolTip.SetFont(&m_Font);
		// ツールチップを複数行対応にする。
		m_ToolTip.SendMessageW(TTM_SETMAXTIPWIDTH, 0, 1024);
		// ツールチップを表示するまでの時間を設定する。
		// m_ToolTip.SetDelayTime(1000);
	}
}

// コントロールに渡る前のメッセージを処理。
BOOL CComboBoxCx::PreTranslateMessage(MSG* pMsg)
{
	// ツールチップを初期化する。
	InitToolTip();

	// ツール ヒント コントロールにメッセージを渡して処理。
	m_ToolTip.RelayEvent(pMsg);

	return CComboBox::PreTranslateMessage(pMsg);
}

// ツールチップのメッセージを設定する。
void CComboBoxCx::SetToolTipText(LPCTSTR pText)
{
	// ツールチップを初期化する。
	InitToolTip();

	// テキストが有効？
	if (pText == NULL) { return; }

	CString toolTipText = pText;
	// テキストの内容が異なる？
	if (!toolTipText.IsEmpty() && m_ToolTipText.Find(toolTipText) == 0) { return; }
	m_ToolTipText = toolTipText;

	// ツール ヒント コントロールに登録されているツールの数は無効？
	if (m_ToolTip.GetToolCount() == 0)
	{
		// クライアント領域のサイズを取得します
		CRect rect;
		GetClientRect(rect);
		// ツール ヒント コントロールにツールを登録します
		m_ToolTip.AddTool(this, m_ToolTipText, rect, 1);
	}
	else
	{
		// ツールにツール ヒント テキストを設定します。
		m_ToolTip.UpdateTipText(m_ToolTipText, this, 1);
	}
	// ツールチップを有効にします。
	SetToolTipActivate(TRUE);
}

// ツールチップの有効無効を切り替えます。
void CComboBoxCx::SetToolTipActivate(BOOL bActivate)
{
	// ツール ヒント コントロールに登録されているツールの数は無効？
	if (m_ToolTip.GetToolCount() == 0) return;

	// ツールチップの有効無効を切り替えます。
	m_ToolTip.Activate(bActivate);
}

// コントロールとツールチップのメッセージを設定する。
void CComboBoxCx::SetToolTipWindowText(LPCTSTR pText)
{
	SetToolTipText(pText);
	SetWindowText(pText);
}
