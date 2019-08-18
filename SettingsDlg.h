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
#include "ComboBoxCx.h"

class CSettingsDlg : public CDialogCx
{
	DECLARE_DYNCREATE(CSettingsDlg)

	static const int SIZE_X = 560;
	static const int SIZE_Y = 248;

public:
	CSettingsDlg(CWnd* pParent = NULL);
	virtual ~CSettingsDlg();

	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	void UpdateDialogSize();

	CStaticCx m_LabelBlockSize;
	CStaticCx m_LabelQueues;
	CStaticCx m_LabelThreads;

	CStaticCx m_SequentialLabel1;
//	CStaticCx m_SequentialLabel2;
	CComboBoxCx m_ComboSequentialSize1;
//	CComboBoxCx m_ComboSequentialSize2;
	CComboBoxCx m_ComboSequentialQueues1;
//	CComboBoxCx m_ComboSequentialQueues2;
	CComboBoxCx m_ComboSequentialThreads1;
//	CComboBoxCx m_ComboSequentialThreads2;

	CStaticCx m_RandomLabel1;
	CStaticCx m_RandomLabel2;
	CStaticCx m_RandomLabel3;
	CComboBoxCx m_ComboRandomSize1;
	CComboBoxCx m_ComboRandomSize2;
	CComboBoxCx m_ComboRandomSize3;
	CComboBoxCx m_ComboRandomQueues1;
	CComboBoxCx m_ComboRandomQueues2;
	CComboBoxCx m_ComboRandomQueues3;
	CComboBoxCx m_ComboRandomThreads1;
	CComboBoxCx m_ComboRandomThreads2;
	CComboBoxCx m_ComboRandomThreads3;

	CButtonCx m_ButtonSetDefault;
	void OnSetDefault();
	void InitComboBox();

	int m_SequentialSize1;
	int m_SequentialQueues1;
	int m_SequentialThreads1;
	//	int m_SequentialSize2;
	//	int m_SequentialQueues2;
	//	int m_SequentialThreads2;
	int m_RandomSize1;
	int m_RandomQueues1;
	int m_RandomThreads1;
	int m_RandomSize2;
	int m_RandomQueues2;
	int m_RandomThreads2;
	int m_RandomSize3;
	int m_RandomQueues3;
	int m_RandomThreads3;

	/*
	CString m_ValueSequentialSize1;
	CString m_ValueSequentialQueues1;
	CString m_ValueSequentialThreads1;
//	CString m_ValueSequentialSize2;
//	CString m_ValueSequentialQueues2;
//	CString m_ValueSequentialThreads2;
	CString m_ValueRandomSize1;
	CString m_ValueRandomQueues1;
	CString m_ValueRandomThreads1;
	CString m_ValueRandomSize2;
	CString m_ValueRandomQueues2;
	CString m_ValueRandomThreads2;
	CString m_ValueRandomSize3;
	CString m_ValueRandomQueues3;
	CString m_ValueRandomThreads3;

	int m_IndexSequentialSize1;
	int m_IndexSequentialQueues1;
	int m_IndexSequentialThreads1;
//	int m_IndexSequentialSize2;
//	int m_IndexSequentialQueues2;
//	int m_IndexSequentialThreads2;
	int m_IndexRandomSize1;
	int m_IndexRandomQueues1;
	int m_IndexRandomThreads1;
	int m_IndexRandomSize2;
	int m_IndexRandomQueues2;
	int m_IndexRandomThreads2;
	int m_IndexRandomSize3;
	int m_IndexRandomQueues3;
	int m_IndexRandomThreads3;
	*/

	DECLARE_MESSAGE_MAP()
};
