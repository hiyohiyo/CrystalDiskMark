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
	~CDiskMarkDlg();

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
	double m_SequentialReadScore2;
	double m_SequentialWriteScore2;
	double m_RandomReadScore1;
	double m_RandomWriteScore1;
	double m_RandomReadScore2;
	double m_RandomWriteScore2;

#ifdef MIX_MODE
	double m_SequentialMixScore1;
	double m_SequentialMixScore2;
	double m_RandomMixScore1;
	double m_RandomMixScore2;
#endif

	double m_SequentialReadLatency1;
	double m_SequentialWriteLatency1;
	double m_SequentialReadLatency2;
	double m_SequentialWriteLatency2;
	double m_RandomReadLatency1;
	double m_RandomWriteLatency1;
	double m_RandomReadLatency2;
	double m_RandomWriteLatency2;

#ifdef MIX_MODE
	double m_SequentialMixLatency1;
	double m_SequentialMixLatency2;
	double m_RandomMixLatency1;
	double m_RandomMixLatency2;
#endif

	void SetMeter(CStaticCx* control, double score, double latency, int blockSize, int unit);
	void ChangeLang(CString LangName);
	void UpdateDialogSize();
	void ChangeButtonStatus(BOOL status);
	void SetScoreToolTip(CStaticCx* cx, double score, double latency, int blockSize);

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
	int m_IndexTestMix;

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
	int m_RandomSize3;
	int m_RandomQueues3;
	int m_RandomThreads3;
	int m_FragmenteCounts;
	int m_IntervalTime;
	int m_Affinity;
	int m_FontScale;

	int m_TestData;
	BOOL m_AdminMode;
	int m_Profile;
	BOOL m_MixMode;
	int m_MixRatio;

	// Message //
	CString m_MesDiskCapacityError;
	CString m_MesDiskWriteError;
	CString m_MesDiskReadError;
	CString m_MesStopBenchmark;
	CString m_MesDiskCreateFileError;
	CString m_MesDiskSpdNotFound;

	void SetWindowTitle(CString message, CString mode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void OnAll();
	void OnTest1();
	void OnTest2();
	void OnTest3();
	void OnTest4();
	void Stop();
	void OnSequentialPeak();
	void OnRandomPeak();
	void OnSequentialReal();
	void OnRandomReal();

	void SelectDrive();
	CString GetRandomResultString(double score, double latency, int size, int queues, int threads);
	CString GetSequentialResultString(double score, double latency, int size, int queues, int threads);

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

#ifdef MIX_MODE
	CStaticCx m_TestMix1;
	CStaticCx m_TestMix2;
	CStaticCx m_TestMix3;
	CStaticCx m_TestMix4;
	CStaticCx m_MixUnit;
#endif

	CButtonCx m_ButtonAll;
	CButtonCx m_ButtonTest1;
	CButtonCx m_ButtonTest2;
	CButtonCx m_ButtonTest3;
	CButtonCx m_ButtonTest4;
	CButtonCx m_CommentEx;

	CStaticCx m_TestRead1;
	CStaticCx m_TestRead2;
	CStaticCx m_TestRead3;
	CStaticCx m_TestRead4;

	CStaticCx m_TestWrite1;
	CStaticCx m_TestWrite2;
	CStaticCx m_TestWrite3;
	CStaticCx m_TestWrite4;

	CEditEx m_Comment;

	CComboBoxCx m_ComboUnit;
	CComboBoxCx m_ComboCount;
	CComboBoxCx m_ComboSize;
	CComboBoxCx m_ComboDrive;
	CComboBoxCx m_ComboMix;

	CStaticCx m_WriteUnit;
	CStaticCx m_ReadUnit;

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

	CBrush *m_EditBrush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnChangeComment();
	afx_msg void OnCommentEx();
	afx_msg void OnKillFocusComment();
	afx_msg void OnSetFocusCommentEx();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEditCopy();
	afx_msg void OnHelp();
	afx_msg void OnCrystalDewWorld();
	afx_msg void OnModeDefault();
	afx_msg void OnModeAll0x00();
	afx_msg void OnProfileDefault();
	afx_msg void OnProfilePeak();
	afx_msg void OnProfileReal();
	afx_msg void OnProfileDefaultMix();
	afx_msg void OnProfilePeakMix();
	afx_msg void OnProfileRealMix();
	afx_msg void OnResultSave();
	afx_msg void OnSettingsQueuesThreads();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCbnSelchangeComboDrive();
	afx_msg void OnCbnSelchangeComboUnit();
#ifdef MIX_MODE
	afx_msg void OnCbnSelchangeComboMix();
#endif
	afx_msg void MoveForcus();
	afx_msg void UpdateUnitLabel();
};
