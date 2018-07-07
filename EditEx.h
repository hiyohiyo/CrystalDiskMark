/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

class CEditEx : public CEdit
{
	DECLARE_DYNAMIC(CEditEx)

public:
	CEditEx();
	virtual ~CEditEx();
	void SetFontEx(CString face, int size, double zoomRatio, LONG fontWeight);

protected:
	CFont    m_Font;

	DECLARE_MESSAGE_MAP()
};


