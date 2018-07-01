/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once
#include "DialogCx.h"
#include "StaticCx.h"
#include "ButtonCx.h"

class CSettingsDlg : public CDialogCx
{
	DECLARE_DYNCREATE(CSettingsDlg)

	static const int SIZE_X = 320;
	static const int SIZE_Y = 180;

public:
	CSettingsDlg(CWnd* pParent = NULL);
	virtual ~CSettingsDlg();

	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnCancel();


	HRESULT OnDefaultSetting();

	void InitSelectBoxQ(CString ElementName, int currentValue, int maxValue, long *index);
	void InitSelectBoxT(CString ElementName, int currentValue, int maxValue, long *index);

	CString m_ValueSequentialQueues1;
	CString m_ValueSequentialThreads1;

	CString m_ValueRandomQueues1;
	CString m_ValueRandomThreads1;
	CString m_ValueRandomQueues2;
	CString m_ValueRandomThreads2;
	CString m_ValueRandomQueues3;
	CString m_ValueRandomThreads3;

	long m_SequentialQueues1;
	long m_SequentialThreads1;

	long m_RandomQueues1;
	long m_RandomThreads1;
	long m_RandomQueues2;
	long m_RandomThreads2;
	long m_RandomQueues3;
	long m_RandomThreads3;

	long m_IndexSequentialQueues1;
	long m_IndexSequentialThreads1;

	long m_IndexRandomQueues1;
	long m_IndexRandomThreads1;
	long m_IndexRandomQueues2;
	long m_IndexRandomThreads2;
	long m_IndexRandomQueues3;
	long m_IndexRandomThreads3;

#ifdef SEQUENTIAL2
	CString m_ValueSequentialQueues2;
	CString m_ValueSequentialThreads2;
	long m_SequentialQueues2;
	long m_SequentialThreads2;
	long m_IndexSequentialQueues2;
	long m_IndexSequentialThreads2;
#endif

	DECLARE_MESSAGE_MAP()
};
