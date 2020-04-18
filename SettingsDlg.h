/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once
#include "DialogFx.h"
#include "StaticFx.h"
#include "ButtonFx.h"
#include "ComboBoxFx.h"

class CSettingsDlg : public CDialogFx
{
	DECLARE_DYNCREATE(CSettingsDlg)

	static const int SIZE_X = 564;
	static const int SIZE_Y = 284;

public:
	CSettingsDlg(CWnd* pParent = NULL);
	virtual ~CSettingsDlg();

	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	void UpdateDialogSize();

	CStaticFx m_LabelBlockSize;
	CStaticFx m_LabelQueues;
	CStaticFx m_LabelThreads;
	CStaticFx m_LabelAffinity;

	CStaticFx m_SequentialLabel1;
	CStaticFx m_SequentialLabel2;
	CComboBoxFx m_ComboSequentialSize1;
	CComboBoxFx m_ComboSequentialSize2;
	CComboBoxFx m_ComboSequentialQueues1;
	CComboBoxFx m_ComboSequentialQueues2;
	CComboBoxFx m_ComboSequentialThreads1;
	CComboBoxFx m_ComboSequentialThreads2;

	CStaticFx m_RandomLabel1;
	CStaticFx m_RandomLabel2;
	CComboBoxFx m_ComboRandomSize1;
	CComboBoxFx m_ComboRandomSize2;
	CComboBoxFx m_ComboRandomQueues1;
	CComboBoxFx m_ComboRandomQueues2;
	CComboBoxFx m_ComboRandomThreads1;
	CComboBoxFx m_ComboRandomThreads2;

	CComboBoxFx m_ComboAffinity;

	CButtonFx m_ButtonSetDefault;
	CButtonFx m_ButtonOk;

	void OnSetDefault();
	void OnOk();
	void InitComboBox();

	int m_SequentialSize1;
	int m_SequentialQueues1;
	int m_SequentialThreads1;
	int m_SequentialSize2;
	int m_SequentialQueues2;
	int m_SequentialThreads2;
	int m_RandomSize1;
	int m_RandomQueues1;
	int m_RandomThreads1;
	int m_RandomSize2;
	int m_RandomQueues2;
	int m_RandomThreads2;
	int m_Affinity;
	DWORD m_Profile;

	DECLARE_MESSAGE_MAP()
};
