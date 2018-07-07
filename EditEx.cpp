/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "EditEx.h"

IMPLEMENT_DYNAMIC(CEditEx, CEdit)

CEditEx::CEditEx()
{
}

CEditEx::~CEditEx()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
END_MESSAGE_MAP()

void CEditEx::SetFontEx(CString face, int size, double zoomRatio, LONG fontWeight)
{
	LOGFONT logFont = { 0 };
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = (LONG)(-1 * size * zoomRatio);
	logFont.lfQuality = 6;
	logFont.lfWeight = fontWeight;
	if (face.GetLength() < 32)
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
}
