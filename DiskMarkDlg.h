/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#include "AboutDlg.h"
#include "SettingsDlg.h"
#include "FontSelection.h"

#include "DialogCx.h"
#include "MainDialog.h"

#include "ButtonCx.h"
#include "StaticCx.h"

class CDiskMarkDlg : public CMainDialog
{
public:
	CDiskMarkDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DISKMARK_DIALOG };


	enum RESULT_TEXT
	{
		RESULT_TEXT_CLIPBOARD = 0,
		RESULT_TEXT_FILE
	};

	volatile CWinThread* m_WinThread;
	volatile BOOL m_DiskBenchStatus;

	void InitScore();
	void UpdateScore();

	double m_SequentialReadScore1;
	double m_SequentialWriteScore1;
	double m_SequentialReadScore2;
	double m_SequentialWriteScore2;
	double m_RandomRead4KBScore1;
	double m_RandomWrite4KBScore1;
	double m_RandomRead4KBScore2;
	double m_RandomWrite4KBScore2;
	double m_RandomRead4KBScore3;
	double m_RandomWrite4KBScore3;

	void SetMeter(CString ElementName, double score);
	void UpdateMessage(CString elementName, CString message);
	void ChangeLang(CString LangName);
	void ChangeButtonStatus(BOOL status);
	void ChangeButton(CString elementName, CString className, CString title, CString html);
	void ChangeSelectStatus(CString elementName, VARIANT_BOOL status);
	void ChangeSelectTitle(CString elementName, CString title);

	CString m_CurrentLocalID;
	CString m_ValueTestDrive;
	CString m_ValueTestCount;
	CString m_ValueTestSize;
	CString m_TestDriveInfo;
	CString m_TestTargetPath;
	long m_TestDriveLetter;
	long m_IndexTestDrive;
	long m_MaxIndexTestDrive;
	long m_IndexTestCount;
	long m_IndexTestSize;

	int m_SequentialMultiQueues1;
	int m_SequentialMultiThreads1;
	int m_SequentialMultiQueues2;
	int m_SequentialMultiThreads2;
	int m_RandomMultiQueues1;
	int m_RandomMultiThreads1;
	int m_RandomMultiQueues2;
	int m_RandomMultiThreads2;
	int m_RandomMultiQueues3;
	int m_RandomMultiThreads3;
	int m_FragmenteCounts;
	int m_IntervalTime;

	DWORD m_TestData;

	// Message //
	CString m_MesDiskCapacityError;
	CString m_MesDiskWriteError;
	CString m_MesDiskReadError;
	CString m_MesStopBenchmark;
	CString m_MesDiskCreateFileError;
	CString m_MesDiskSpdNotFound;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HRESULT OnAll();
	HRESULT OnSelectDrive();
	HRESULT OnSequential1();
#ifdef SEQUENTIAL2
	HRESULT OnSequential2();
#endif
	HRESULT OnRandom4KB1();
	HRESULT OnRandom4KB2();
	HRESULT OnRandom4KB3();

	void Stop();

	CString m_TitleTestDrive;
	CString m_TitleTestCount;
	CString m_TitleTestSize;
	CString m_TitleTestQSize;

	CString m_ExeDir;
	CString m_Comment;

protected:
	HICON m_hIcon;
	HICON m_hIconMini;
	HACCEL m_hAccelerator;

	int m_SizeX;
	int m_SizeY;

	CAboutDlg*		m_AboutDlg;
	CSettingsDlg*	m_SettingsDlg;

	void SetControlFont();

	void InitDrive(CString ElementName);

	BOOL CheckRadioZoomType(int id, int value);
	void CheckRadioZoomType();
	void ChangeZoom();
	void UpdateQueuesThreads();

	BOOL CheckRadioIntervalTime(int id, int value);
	void CheckIntervalTime();

	void EnableMenus();
	void DisableMenus();

	void ResultText(RESULT_TEXT type);


	CButtonCx m_ButtonAll;
	CButtonCx m_ButtonSequential1;
	CButtonCx m_ButtonSequential2;
	CButtonCx m_ButtonRandom1;
	CButtonCx m_ButtonRandom2;
	CButtonCx m_ButtonRandom3;
	
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdateScore(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitBenchmark(WPARAM wParam, LPARAM lParam);
	afx_msg void OnZoom75();
	afx_msg void OnZoom100();
	afx_msg void OnZoom125();
	afx_msg void OnZoom150();
	afx_msg void OnZoom200();
	afx_msg void OnZoom250();
	afx_msg void OnZoom300();
	afx_msg void OnZoom400();
	afx_msg void OnZoomAuto();

	afx_msg void OnIntervalTime0();
	afx_msg void OnIntervalTime1();
	afx_msg void OnIntervalTime3();
	afx_msg void OnIntervalTime5();
	afx_msg void OnIntervalTime10();
	afx_msg void OnIntervalTime30();
	afx_msg void OnIntervalTime60();
	afx_msg void OnIntervalTime180();
	afx_msg void OnIntervalTime300();
	afx_msg void OnIntervalTime600();


	afx_msg void OnExit();
	afx_msg void OnAbout();
	afx_msg void OnFontSetting();

	LRESULT OnQueryEndSession(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEditCopy();
	afx_msg void OnHelp();
	afx_msg void OnCrystalDewWorld();
	afx_msg void OnModeDefault();
	afx_msg void OnModeAll0x00();
#ifdef SUISHO_SHIZUKU_SUPPORT
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
#endif
	afx_msg void OnResultSave();
	afx_msg void OnSettingsQueuesThreads();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedAll();
};
