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
#include "EditEx.h"

class CDiskMarkDlg : public CMainDialog
{
public:
	CDiskMarkDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_DISKMARK_DIALOG };

#ifdef SUISHO_SHIZUKU_SUPPORT
	static const int OFFSET_X = 0;
#else
	static const int OFFSET_X = 0;
#endif

	enum RESULT_TEXT
	{
		RESULT_TEXT_CLIPBOARD = 0,
		RESULT_TEXT_FILE
	};

	enum SCORE_UNIT
	{
		SCORE_MBS = 0,
		SCORE_GBS,
		SCORE_IOPS,
		SCORE_US,
	};

	volatile CWinThread* m_WinThread;
	volatile BOOL m_DiskBenchStatus;

	void InitScore();
	void UpdateScore();

	double m_SequentialReadScore1;
	double m_SequentialWriteScore1;
	double m_SequentialMixScore1;
	double m_SequentialReadScore2;
	double m_SequentialWriteScore2;
	double m_SequentialMixScore2;
	double m_RandomRead4KBScore1;
	double m_RandomWrite4KBScore1;
	double m_RandomMix4KBScore1;
	double m_RandomRead4KBScore2;
	double m_RandomWrite4KBScore2;
	double m_RandomMix4KBScore2;
	double m_RandomRead4KBScore3;
	double m_RandomWrite4KBScore3;
	double m_RandomMix4KBScore3;

	void SetMeter(CStaticCx* control, double score, int blockSize);
	void ChangeLang(CString LangName);
	void UpdateDialogSize();
	void ChangeButtonStatus(BOOL status);
	void SetScoreToolTip(CStaticCx* cx, double score, int blockSize);

	CString m_CurrentLocalID;

	CString m_ValueTestUnit;
	CString m_ValueTestCount;
	CString m_ValueTestSize;
	CString m_ValueTestDrive;
	CString m_TestDriveInfo;
	CString m_TestTargetPath;
	long m_TestDriveLetter;

	int m_MaxIndexTestDrive;
	int m_IndexTestUnit;
	int m_IndexTestCount;
	int m_IndexTestSize;
	int m_IndexTestDrive;

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

	void OnAll();
	void OnSequential1();
	void OnSequential2();
	void OnRandom4KB1();
	void OnRandom4KB2();
	void OnRandom4KB3();
	void Stop();
	void SelectDrive();

	CString m_TitleTestDrive;
	CString m_TitleTestCount;
	CString m_TitleTestSize;
	CString m_TitleTestQSize;

	CString m_ExeDir;

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

	void ResultText(RESULT_TEXT type);

	void SetLayeredWindow(HWND hWnd, BYTE alpha);


	CButtonCx m_ButtonAll;
	CButtonCx m_ButtonSequential1;
	CButtonCx m_ButtonSequential2;
	CButtonCx m_ButtonRandom1;
	CButtonCx m_ButtonRandom2;
	CButtonCx m_ButtonRandom3;

	CStaticCx m_SequentialRead1;
	CStaticCx m_SequentialRead2;
	CStaticCx m_RandomRead1;
	CStaticCx m_RandomRead2;
	CStaticCx m_RandomRead3;

	CStaticCx m_SequentialWrite1;
	CStaticCx m_SequentialWrite2;
	CStaticCx m_RandomWrite1;
	CStaticCx m_RandomWrite2;
	CStaticCx m_RandomWrite3;

	CStaticCx m_SequentialMix1;
	CStaticCx m_SequentialMix2;
	CStaticCx m_RandomMix1;
	CStaticCx m_RandomMix2;
	CStaticCx m_RandomMix3;

	CEditEx m_Comment;

	CComboBoxCx m_ComboUnit;
	CComboBoxCx m_ComboCount;
	CComboBoxCx m_ComboSize;
	CComboBoxCx m_ComboDrive;

	CStaticCx m_WriteUnit;
	CStaticCx m_ReadUnit;
	CStaticCx m_MixUnit;

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
	afx_msg void OnCbnSelchangeComboDrive();
	afx_msg void OnCbnSelchangeComboUnit();
	afx_msg void MoveForcus();
	afx_msg void UpdateUnitLabel();

};
