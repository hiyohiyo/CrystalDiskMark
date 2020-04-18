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

class CAboutDlg : public CDialogFx
{
	DECLARE_DYNCREATE(CAboutDlg)

#ifdef SUISHO_SHIZUKU_SUPPORT
	static const int SIZE_X = 640;
	static const int SIZE_Y = 660;
#else
	static const int SIZE_X = 480;
	static const int SIZE_Y = 172;
#endif
	static const int SIZE_HC_Y = 172; // for HighContrast Mode

public:
	CAboutDlg(CWnd* pParent = NULL);
	virtual ~CAboutDlg();

	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void UpdateDialogSize();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCrystalDewWorld();
	afx_msg void OnVersion();
	afx_msg void OnLicense();
	afx_msg void OnBnClickedProjectSite1();
	afx_msg void OnBnClickedProjectSite2();
	afx_msg void OnBnClickedProjectSite3();
	afx_msg void OnBnClickedProjectSite4();
	afx_msg void OnBnClickedProjectSite5();

	CButtonFx m_CtrlCrystalDewWorld;
	CButtonFx m_CtrlProjectSite1;
	CButtonFx m_CtrlProjectSite2;
	CButtonFx m_CtrlProjectSite3;
	CButtonFx m_CtrlProjectSite4;
	CButtonFx m_CtrlProjectSite5;
	CButtonFx m_CtrlVersion;
	CButtonFx m_CtrlLicense;

	CStaticFx m_CtrlEdition;
	CStaticFx m_CtrlRelease;
	CStaticFx m_CtrlCopyright;
};
