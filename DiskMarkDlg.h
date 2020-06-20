/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#include "AboutDlg.h"
#include "SettingsDlg.h"
#include "FontSelectionDlg.h"

#include "DialogFx.h"
#include "MainDialogFx.h"
#include "ButtonFx.h"
#include "StaticFx.h"
#include "ComboBoxFx.h"
#include "EditFx.h"
#include "UtilityFx.h"
#include "OsInfoFx.h"

class CDiskMarkDlg : public CMainDialogFx
{
public:
	CDiskMarkDlg(CWnd* pParent = NULL);
	~CDiskMarkDlg();

	enum { IDD = IDD_DISKMARK_DIALOG };

	enum SCORE_UNIT
	{
		SCORE_MBS = 0,
		SCORE_GBS,
		SCORE_IOPS,
		SCORE_US,
	};

	enum BENCH_TYPE
	{
		BENCH_SEQ = 0,
		BENCH_RND,
	};

	volatile CWinThread* m_WinThread;
	volatile BOOL m_DiskBenchStatus;

	void InitScore();
	void UpdateScore();

	double m_ReadScore[8];
	double m_WriteScore[8];
	double m_ReadLatency[8];
	double m_WriteLatency[8];

#ifdef MIX_MODE
	double m_MixScore[8];
	double m_MixLatency[8];
#endif

	void SetMeter(CStaticFx* control, double score, double latency, int blockSize, int unit);
	void ChangeLang(CString LangName);
	void UpdateDialogSize();
	void ChangeButtonStatus(BOOL status);
	void SetScoreToolTip(CStaticFx* cx, double score, double latency, int blockSize);

	CString m_ValueTestUnit;
	CString m_ValueTestCount;
	CString m_ValueTestSize;
	CString m_ValueTestDrive;
	CString m_ValueTestMode;
	CString m_TestDriveInfo;
	CString m_TestTargetPath;
	long m_TestDriveLetter;

	int m_MaxIndexTestDrive;
	int m_IndexTestUnit;
	int m_IndexTestCount;
	int m_IndexTestSize;
	int m_IndexTestDrive;
	int m_IndexTestMix;
	int m_IndexTestMode;

	int m_BenchType[8];
	int m_BenchSize[8];
	int m_BenchQueues[8];
	int m_BenchThreads[8];
	int m_IntervalTime;
	int m_Affinity;
	int m_MeasureTime;

	int m_TestData;
	int m_Profile;
	BOOL m_AdminMode;
	BOOL m_MixMode;
	int m_MixRatio;

	// Message //
	CString m_MesDiskCapacityError;
	CString m_MesDiskWriteError;
	CString m_MesDiskReadError;
	CString m_MesStopBenchmark;
	CString m_MesDiskCreateFileError;
	CString m_MesDiskSpdNotFound;

	void SetWindowTitle(CString message);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void OnAll();
	void OnTest0();
	void OnTest1();
	void OnTest2();
	void OnTest3();
	void Stop();
	void OnSequentialPeak();
	void OnRandomPeak();
	void OnSequentialReal();
	void OnRandomReal();

	void SelectDrive();
	CString GetResultString(int type, double score, double latency, int size, int queues, int threads);
	CString GetButtonText(int type, int size, int queues, int threads, int unit);
	CString GetButtonToolTipText(int type, int size, int queues, int threads, int unit);

	CString m_TitleTestDrive;
	CString m_TitleTestCount;
	CString m_TitleTestSize;
	CString m_TitleTestQSize;

protected:
	HICON m_hIcon;
	HICON m_hIconMini;
	HACCEL m_hAccelerator;

	int m_SizeX;
	int m_SizeY;

	CAboutDlg*		m_AboutDlg;
	CSettingsDlg*	m_SettingsDlg;

	void SetControlFont();
	CString IP(CString imagePath); // IP means Image Path!!

	void InitDrive();

	BOOL CheckRadioZoomType(int id, int value);
	void CheckRadioZoomType();
	void UpdateQueuesThreads();

	BOOL CheckRadioIntervalTime(int id, int value);
	void CheckIntervalTime();

	void EnableMenus();
	void DisableMenus();

	void SaveText(CString fileName);

	void SetLayeredWindow(HWND hWnd, BYTE alpha);
	void UpdateComboTooltip();

	virtual BOOL CheckThemeEdition(CString name);

	BOOL IsNormMode();
	BOOL IsNVMeMode();
	BOOL IsAtaMode();

#ifdef MIX_MODE
	CStaticFx m_TestMix0;
	CStaticFx m_TestMix1;
	CStaticFx m_TestMix2;
	CStaticFx m_TestMix3;
	CStaticFx m_MixUnit;
	CComboBoxFx m_ComboMix;
#endif

	CButtonFx m_ButtonAll;
	CButtonFx m_ButtonTest0;
	CButtonFx m_ButtonTest1;
	CButtonFx m_ButtonTest2;
	CButtonFx m_ButtonTest3;

	CStaticFx m_TestRead0;
	CStaticFx m_TestRead1;
	CStaticFx m_TestRead2;
	CStaticFx m_TestRead3;

	CStaticFx m_TestWrite0;
	CStaticFx m_TestWrite1;
	CStaticFx m_TestWrite2;
	CStaticFx m_TestWrite3;

	CEditFx m_Comment;

	CComboBoxFx m_ComboCount;
	CComboBoxFx m_ComboSize;
	CComboBoxFx m_ComboDrive;
	CComboBoxFx m_ComboUnit;
	CComboBoxFx m_ComboMode;

	CStaticFx m_WriteUnit;
	CStaticFx m_ReadUnit;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdateScore(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitBenchmark(WPARAM wParam, LPARAM lParam);
	afx_msg void OnZoom100();
	afx_msg void OnZoom125();
	afx_msg void OnZoom150();
	afx_msg void OnZoom200();
	afx_msg void OnZoom250();
	afx_msg void OnZoom300();
//	afx_msg void OnZoom400();
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
	afx_msg void OnCopy();
	afx_msg void OnHelp();
	afx_msg void OnCrystalDewWorld();
	afx_msg void OnModeDefault();
	afx_msg void OnModeAll0x00();
	afx_msg void OnProfileDefault();
	afx_msg void OnProfilePeak();
	afx_msg void OnProfileReal();
	afx_msg void OnSaveText();
	afx_msg void OnSaveImage();
	afx_msg void OnSettingsQueuesThreads();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCbnSelchangeComboDrive();
	afx_msg void OnCbnSelchangeComboUnit();
	afx_msg void OnCbnSelchangeComboMode();
	afx_msg void MoveForcus();
	afx_msg void UpdateUnitLabel();

	void ProfileDefault();
	void ProfilePeak();
	void ProfileReal();

	void SettingsQueuesThreads(int type);

#ifdef MIX_MODE
	afx_msg void OnProfileDefaultMix();
	afx_msg void OnProfilePeakMix();
	afx_msg void OnProfileRealMix();
	void ProfileDefaultMix();
	void ProfilePeakMix();
	void ProfileRealMix();
	afx_msg void OnCbnSelchangeComboMix();
#endif

};
