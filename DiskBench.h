/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#define WM_UPDATE_SCORE		(WM_APP+1)
#define WM_UPDATE_MESSAGE	(WM_APP+2)
#define WM_EXIT_BENCHMARK	(WM_APP+3)

#define TIMER_ID 5963

// Common Message
#define ALL_0X00_0FILL L"<0Fill>"

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
	TEST_READ_0,
	TEST_READ_1,
	TEST_READ_2,
	TEST_READ_3,
	TEST_READ_4,
	TEST_READ_5,
	TEST_READ_6,
	TEST_READ_7,
	TEST_WRITE_0,
	TEST_WRITE_1,
	TEST_WRITE_2,
	TEST_WRITE_3,
	TEST_WRITE_4,
	TEST_WRITE_5,
	TEST_WRITE_6,
	TEST_WRITE_7,
	TEST_MIX_0,
	TEST_MIX_1,
	TEST_MIX_2,
	TEST_MIX_3,
	TEST_MIX_4,
	TEST_MIX_5,
	TEST_MIX_6,
	TEST_MIX_7,
};

UINT ExecDiskBenchAll(LPVOID  dlg);
UINT ExecDiskBenchAllPeak(LPVOID  dlg);
UINT ExecDiskBenchAllReal(LPVOID  dlg);
UINT ExecDiskBench0(LPVOID  dlg);
UINT ExecDiskBench1(LPVOID  dlg);
UINT ExecDiskBench2(LPVOID  dlg);
UINT ExecDiskBench3(LPVOID  dlg);
UINT ExecDiskBench4(LPVOID  dlg);
UINT ExecDiskBench5(LPVOID  dlg);
UINT ExecDiskBench6(LPVOID  dlg);
UINT ExecDiskBench7(LPVOID  dlg);
