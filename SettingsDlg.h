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

#ifdef SUISHO_SHIZUKU_SUPPORT
	static const int SIZE_X = 680;
	static const int SIZE_Y = 480;
#else
	static const int SIZE_X = 440;
	static const int SIZE_Y = 408;
#endif

public:
	CSettingsDlg(CWnd* pParent = NULL);
	virtual ~CSettingsDlg();

	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	void UpdateDialogSize();
	int GetBlockSize(CString text);
	int GetType(CString text);

	CStaticFx m_LabelType;
	CStaticFx m_LabelSize;
	CStaticFx m_LabelQueues;
	CStaticFx m_LabelThreads;
	CStaticFx m_LabelDefault;
	CStaticFx m_LabelPeak;
	CStaticFx m_LabelDemo;
	CStaticFx m_LabelMeasureTime;
	CStaticFx m_LabelIntervalTime;
	CStaticFx m_LabelAffinity;
	CStaticFx m_LabelData;

	CComboBoxFx m_ComboBenchType0;
	CComboBoxFx m_ComboBenchType1;
	CComboBoxFx m_ComboBenchType2;
	CComboBoxFx m_ComboBenchType3;
	CComboBoxFx m_ComboBenchType4;
	CComboBoxFx m_ComboBenchType5;
	CComboBoxFx m_ComboBenchType8;
	CComboBoxFx m_ComboBenchSize0;
	CComboBoxFx m_ComboBenchSize1;
	CComboBoxFx m_ComboBenchSize2;
	CComboBoxFx m_ComboBenchSize3;
	CComboBoxFx m_ComboBenchSize4;
	CComboBoxFx m_ComboBenchSize5;
	CComboBoxFx m_ComboBenchSize8;
	CComboBoxFx m_ComboBenchQueues0;
	CComboBoxFx m_ComboBenchQueues1;
	CComboBoxFx m_ComboBenchQueues2;
	CComboBoxFx m_ComboBenchQueues3;
	CComboBoxFx m_ComboBenchQueues4;
	CComboBoxFx m_ComboBenchQueues5;
	CComboBoxFx m_ComboBenchQueues8;
	CComboBoxFx m_ComboBenchThreads0;
	CComboBoxFx m_ComboBenchThreads1;
	CComboBoxFx m_ComboBenchThreads2;
	CComboBoxFx m_ComboBenchThreads3;
	CComboBoxFx m_ComboBenchThreads4;
	CComboBoxFx m_ComboBenchThreads5;
	CComboBoxFx m_ComboBenchThreads8;

	CComboBoxFx m_ComboAffinity;
	CComboBoxFx m_ComboData;
	CComboBoxFx m_ComboMeasureTime;
	CComboBoxFx m_ComboIntervalTime;

	CButtonFx m_ButtonSetNorm;
	CButtonFx m_ButtonSetNVMe;
	CButtonFx m_ButtonSetAta;
	CButtonFx m_ButtonOk;

	void OnSetNorm();
	void OnSetNVMe();
	void OnSetAta();
	void OnOk();
	void InitComboBox();

	int m_BenchType[9];
	int m_BenchSize[9];
	int m_BenchQueues[9];
	int m_BenchThreads[9];

	int m_TestData;
	int m_Affinity;
	int m_MeasureTime;
	int m_IntervalTime;
	DWORD m_Profile;

	DECLARE_MESSAGE_MAP()
};
