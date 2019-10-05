/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#define WM_USER_UPDATE_SCORE	(WM_USER+1)
#define WM_USER_UPDATE_MESSAGE	(WM_USER+2)
#define WM_USER_EXIT_BENCHMARK	(WM_USER+3)
#define WM_USER_UPDATE_COMMENT	(WM_USER+4) // for CCommentDlg

#define TIMER_ID 5963

// Common Message
#define ALL_0X00_0FILL _T("<0Fill>")

enum TEST_DATA_TYPE
{
	TEST_DATA_RANDOM = 0,
	TEST_DATA_ALL0X00,
	TEST_DATA_ALL0XFF,
};

enum AFFINITY_MODE
{
	AFFINITY_DISABLED = 0,
	AFFINITY_ENABLED,
};

enum PROFILE
{
	PROFILE_DEFAULT = 0,
	PROFILE_PEAK,
	PROFILE_REAL,
	PROFILE_DEFAULT_MIX,
	PROFILE_PEAK_MIX,
	PROFILE_REAL_MIX,
};


enum DISK_SPD_CMD
{
	TEST_CREATE_FILE = 0,
	TEST_DELETE_FILE,
	TEST_SEQUENTIAL_READ_1,
	TEST_SEQUENTIAL_WRITE_1,
	TEST_SEQUENTIAL_MIX_1,
	TEST_SEQUENTIAL_READ_2,
	TEST_SEQUENTIAL_WRITE_2,
	TEST_SEQUENTIAL_MIX_2,
	TEST_SEQUENTIAL_READ_REAL,
	TEST_SEQUENTIAL_WRITE_REAL,
	TEST_SEQUENTIAL_MIX_REAL,
	TEST_RANDOM_READ_1,
	TEST_RANDOM_WRITE_1,
	TEST_RANDOM_MIX_1,
	TEST_RANDOM_READ_2,
	TEST_RANDOM_WRITE_2,
	TEST_RANDOM_MIX_2,
	TEST_RANDOM_READ_REAL,
	TEST_RANDOM_WRITE_REAL,
	TEST_RANDOM_MIX_REAL,
};

UINT ExecDiskBenchAll(void* dlg);
UINT ExecDiskBenchAllPeak(void* dlg);
UINT ExecDiskBenchAllReal(void* dlg);
UINT ExecDiskBenchSequential1(void* dlg);
UINT ExecDiskBenchSequential2(void* dlg);
UINT ExecDiskBenchSequentialReal(void* dlg);
UINT ExecDiskBenchRandom1(void* dlg);
UINT ExecDiskBenchRandom2(void* dlg);
UINT ExecDiskBenchRandomReal(void* dlg);
