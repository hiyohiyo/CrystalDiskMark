/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The MIT License
//
//                                             Copyright (c) 2007-2015 hiyohiyo
/*---------------------------------------------------------------------------*/

#pragma once

#define WM_USER_UPDATE_SCORE	(WM_USER+1)
#define WM_USER_UPDATE_MESSAGE	(WM_USER+2)
#define WM_USER_EXIT_BENCHMARK	(WM_USER+3)

#define TIMER_ID 5963

// Common Message
#define ALL_0X00_0FILL _T("<0Fill>")

enum TEST_DATA_TYPE
{
	TEST_DATA_RANDOM = 0,
	TEST_DATA_ALL0X00,
	TEST_DATA_ALL0XFF,
};

enum DISK_SPD_CMD
{
	TEST_CREATE_FILE,
	TEST_DELETE_FILE,
	TEST_SEQUENTIAL_READ1,
	TEST_SEQUENTIAL_WRITE1,
#ifdef SEQUENTIAL2
	TEST_SEQUENTIAL_READ2,
	TEST_SEQUENTIAL_WRITE2,
#endif
	TEST_RANDOM_READ_4KB1,
	TEST_RANDOM_WRITE_4KB1,
	TEST_RANDOM_READ_4KB2,
	TEST_RANDOM_WRITE_4KB2,
	TEST_RANDOM_READ_4KB3,
	TEST_RANDOM_WRITE_4KB3,
};

UINT ExecDiskBenchAll(void* dlg);
UINT ExecDiskBenchSequential1(void* dlg);
#ifdef SEQUENTIAL2
UINT ExecDiskBenchSequential2(void* dlg);
#endif
UINT ExecDiskBenchRandom4KB1(void* dlg);
UINT ExecDiskBenchRandom4KB2(void* dlg);
UINT ExecDiskBenchRandom4KB3(void* dlg);
