/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskMark.h"
#include "DiskMarkDlg.h"
#include "DiskBench.h"

#include <afxmt.h>
#include <winioctl.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#pragma warning(disable : 4996)

static CString TestFilePath;
static CString TestFileDir;
static CString DiskSpdExe;

static HANDLE hFile;
static int DiskTestCount;
static UINT64 DiskTestSize;
static int BenchType[9];
static int BenchSize[9];
static int BenchQueues[9];
static int BenchThreads[9];
// static int Affinity;
static BOOL MixMode;
static int MixRatio;

static void ShowErrorMessage(CString message);
static void Interval(void* dlg);

static BOOL Init(void* dlg);
static void DiskSpd(void* dlg, DISK_SPD_CMD cmd);

static UINT Exit(void* dlg);
static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
static volatile BOOL WaitFlag;

#define DISK_SPD_EXE_32           L"CdmResource\\diskspd\\diskspd32.exe"
#define DISK_SPD_EXE_64           L"CdmResource\\diskspd\\diskspd64.exe"
#define DISK_SPD_EXE_32_LEGACY    L"CdmResource\\diskspd\\diskspd32L.exe"
#define DISK_SPD_EXE_64_LEGACY    L"CdmResource\\diskspd\\diskspd64L.exe"
#define DISK_SPD_EXE_ARM32        L"CdmResource\\diskspd\\diskspdA32.exe"
#define DISK_SPD_EXE_ARM64        L"CdmResource\\diskspd\\diskspdA64.exe"

PROCESS_INFORMATION pi;

int ExecAndWait(TCHAR *pszCmd, BOOL bNoWindow, double *latency)
{
	DWORD Code = 0;
	BOOL bSuccess;
	STARTUPINFO si;

	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (bNoWindow) {
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
	}

	CString name;
	name.Format(L"CrystalDiskMark%08X", GetCurrentProcessId());
	auto size = 8;

	HANDLE hSharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, size, name.GetString());
	if (hSharedMemory != NULL)
	{
		auto pMemory = (double*)MapViewOfFile(hSharedMemory, FILE_MAP_ALL_ACCESS, NULL, NULL, size);
		if (pMemory != NULL)
		{
			bSuccess = CreateProcess(NULL, pszCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			if (bSuccess != TRUE)
			{
				UnmapViewOfFile(pMemory);
				CloseHandle(hSharedMemory);
				return 0;
			}

			WaitForInputIdle(pi.hProcess, INFINITE);
			WaitForSingleObject(pi.hProcess, INFINITE);
			GetExitCodeProcess(pi.hProcess, &Code);

			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			pi.hProcess = NULL;

			*latency = (double)*pMemory * 1000; // milli sec to micro sec

			UnmapViewOfFile(pMemory);
			CloseHandle(hSharedMemory);
		}
	}

	return Code;
}


void ShowErrorMessage(CString message)
{
	DWORD lastErrorCode = GetLastError();
	CString errorMessage;
	LPVOID lpMessageBuffer;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, lastErrorCode, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMessageBuffer, 0, NULL);
	errorMessage.Format(L"0x%08X:%s", lastErrorCode, (LPTSTR) lpMessageBuffer);

	AfxMessageBox(message + L"\r\n" + errorMessage);
	LocalFree( lpMessageBuffer );
}

void Interval(void* dlg)
{
	int intervalTime = ((CDiskMarkDlg*) dlg)->m_IntervalTime;
	CString title;

	for (int i = 0; i < intervalTime; i++)
	{
		if (!((CDiskMarkDlg*) dlg)->m_DiskBenchStatus)
		{
			return;
		}
		title.Format(L"Interval Time %d/%d sec", i, intervalTime);
		::PostMessage(((CDiskMarkDlg*) dlg)->GetSafeHwnd(), WM_UPDATE_MESSAGE, (WPARAM) &title, 0);
		Sleep(1000);
	}
}

UINT ExecDiskBenchAll(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if(Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_0); Interval(dlg);
			DiskSpd(dlg, TEST_READ_1); Interval(dlg);
			DiskSpd(dlg, TEST_READ_2); Interval(dlg);
			DiskSpd(dlg, TEST_READ_3);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_0); Interval(dlg);
			DiskSpd(dlg, TEST_WRITE_1); Interval(dlg);
			DiskSpd(dlg, TEST_WRITE_2); Interval(dlg);
			DiskSpd(dlg, TEST_WRITE_3);
		}

#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_0); Interval(dlg);
			DiskSpd(dlg, TEST_MIX_1); Interval(dlg);
			DiskSpd(dlg, TEST_MIX_2); Interval(dlg);
			DiskSpd(dlg, TEST_MIX_3); 
		}
#endif
	}

	return Exit(dlg);
}

UINT ExecDiskBenchAllPeak(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_4); Interval(dlg);
			DiskSpd(dlg, TEST_READ_5);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_4); Interval(dlg);
			DiskSpd(dlg, TEST_WRITE_5);
		}

#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_4); Interval(dlg);
			DiskSpd(dlg, TEST_MIX_5);
		}
#endif
	}

	return Exit(dlg);
}

UINT ExecDiskBenchAllReal(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_6); Interval(dlg);
			DiskSpd(dlg, TEST_READ_7);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_6); Interval(dlg);
			DiskSpd(dlg, TEST_WRITE_7);
		}

#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_6); Interval(dlg);
			DiskSpd(dlg, TEST_MIX_7);
		}
#endif
	}

	return Exit(dlg);
}

UINT ExecDiskBenchAllDemo(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_8);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_8);
		}
	}

	return Exit(dlg);
}

UINT ExecDiskBench0(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_0);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_0);
		}

#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_0);
		}
#endif
	}
	return Exit(dlg);
}

UINT ExecDiskBench1(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_1);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_1);
		}
#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_1);
		}
#endif
	}
	return Exit(dlg);
}

UINT ExecDiskBench2(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_2);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_2);
		}
#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_2);
		}
#endif
	}
	return Exit(dlg);
}

UINT ExecDiskBench3(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_3);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_3);
		}
#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_3);
		}
#endif
	}
	return Exit(dlg);
}

UINT ExecDiskBench4(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_4);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_4);
		}
#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_4);
		}
#endif
	}
	return Exit(dlg);
}

UINT ExecDiskBench5(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_5);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_5);
		}
#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_5);
		}
#endif
	}
	return Exit(dlg);
}

UINT ExecDiskBench6(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_6);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_6);
		}
#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_6);
		}
#endif
	}
	return Exit(dlg);
}

UINT ExecDiskBench7(LPVOID dlg)
{
	int benchmark = ((CDiskMarkDlg*)dlg)->m_Benchmark;

	if (Init(dlg))
	{
		if (benchmark & BENCHMARK_READ)
		{
			DiskSpd(dlg, TEST_READ_7);
		}
		if ((benchmark & BENCHMARK_READ) && (benchmark & BENCHMARK_WRITE))
		{
			Interval(dlg);
		}
		if (benchmark & BENCHMARK_WRITE)
		{
			DiskSpd(dlg, TEST_WRITE_7);
		}
#ifdef MIX_MODE
		if (MixMode)
		{
			Interval(dlg);
			DiskSpd(dlg, TEST_MIX_7);
		}
#endif
	}
	return Exit(dlg);
}

BOOL Init(void* dlg)
{
	BOOL FlagArc;
	BOOL result;
	static CString cstr;
	TCHAR drive;

	ULARGE_INTEGER freeBytesAvailableToCaller;
	ULARGE_INTEGER totalNumberOfBytes;
	ULARGE_INTEGER totalNumberOfFreeBytes;

	// Init m_Ini
	TCHAR *ptrEnd;
	TCHAR temp[MAX_PATH];
	::GetModuleFileName(NULL, temp, MAX_PATH);
	if ((ptrEnd = _tcsrchr(temp, '\\')) != NULL)
	{
		*ptrEnd = '\0';
	}

	pi.hProcess = NULL;

#ifdef _M_ARM
	DiskSpdExe.Format(L"%s\\%s", temp, DISK_SPD_EXE_ARM32);
#elif _M_ARM64
	DiskSpdExe.Format(L"%s\\%s", temp, DISK_SPD_EXE_ARM64);
#elif _M_X64
	if(IsWin8orLater())
	{
		DiskSpdExe.Format(L"%s\\%s", temp, DISK_SPD_EXE_64);
	}
	else
	{
		DiskSpdExe.Format(L"%s\\%s", temp, DISK_SPD_EXE_64_LEGACY);
	}
#else
	if (IsWin8orLater())
	{
		DiskSpdExe.Format(L"%s\\%s", temp, DISK_SPD_EXE_32);
	}
	else
	{
		DiskSpdExe.Format(L"%s\\%s", temp, DISK_SPD_EXE_32_LEGACY);
	}
#endif

	if (! IsFileExist(DiskSpdExe))
	{
		AfxMessageBox(((CDiskMarkDlg*) dlg)->m_MesDiskSpdNotFound);
		((CDiskMarkDlg*) dlg)->m_DiskBenchStatus = FALSE;
		return FALSE;
	}
	DiskTestCount = ((CDiskMarkDlg*) dlg)->m_IndexTestCount + 1;
	
	CString testSize = ((CDiskMarkDlg*)dlg)->m_ValueTestSize;
	if (testSize.Find(L"M") == -1) // GiB
	{
		DiskTestSize = (UINT64)_tstoi(testSize) * 1024;
	}
	else // MiB
	{
		DiskTestSize = (UINT64)_tstoi(testSize);
	}

	for (int i = 0; i < 9; i++)
	{
		BenchType[i] = ((CDiskMarkDlg*)dlg)->m_BenchType[i];
		BenchSize[i] = ((CDiskMarkDlg*)dlg)->m_BenchSize[i];
		BenchQueues[i] = ((CDiskMarkDlg*)dlg)->m_BenchQueues[i];
		BenchThreads[i] = ((CDiskMarkDlg*)dlg)->m_BenchThreads[i];
	}

	MixMode = ((CDiskMarkDlg*)dlg)->m_MixMode;
	MixRatio = ((CDiskMarkDlg*)dlg)->m_MixRatio;

	CString RootPath;
	if(((CDiskMarkDlg*)dlg)->m_MaxIndexTestDrive != ((CDiskMarkDlg*)dlg)->m_IndexTestDrive)
	{

		drive = ((CDiskMarkDlg*)dlg)->m_ValueTestDrive.GetAt(0);
		cstr.Format(L"%C:\\", drive);
		GetDiskFreeSpaceEx(cstr, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes);
		if (totalNumberOfBytes.QuadPart < ((ULONGLONG)8 * 1024 * 1024 * 1024)) // < 8 GB
		{
			((CDiskMarkDlg*)dlg)->m_TestDriveInfo.Format(L"%C: %.1f%% (%.1f/%.1f MiB)", drive,
				(double)(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100,
				(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / 1024 / 1024.0,
				totalNumberOfBytes.QuadPart / 1024 / 1024.0);
		}
		else
		{
			((CDiskMarkDlg*)dlg)->m_TestDriveInfo.Format(L"%C: %.1f%% (%.1f/%.1f GiB)", drive,
				(double)(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / (double)totalNumberOfBytes.QuadPart * 100,
				(totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / 1024 / 1024 / 1024.0,
				totalNumberOfBytes.QuadPart / 1024 / 1024 / 1024.0);
		}
		RootPath.Format(L"%c:\\", drive);
	}
	else
	{
		RootPath = ((CDiskMarkDlg*)dlg)->m_TestTargetPath;
		RootPath += L"\\";
	}

	TestFileDir.Format(L"%sCrystalDiskMark%08X", (LPTSTR)RootPath.GetString(), timeGetTime());
	CreateDirectory(TestFileDir, NULL);
	TestFilePath.Format(L"%s\\CrystalDiskMark%08X.tmp", (LPTSTR)TestFileDir.GetString(), timeGetTime());

	DWORD FileSystemFlags;
	GetVolumeInformation(RootPath, NULL, NULL, NULL, NULL, &FileSystemFlags, NULL, NULL);
	if(FileSystemFlags & FS_VOL_IS_COMPRESSED)
	{
		FlagArc = TRUE;
	}
	else
	{
		FlagArc = FALSE;
	}

// Check Disk Capacity //
	OSVERSIONINFO osVersionInfo;
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osVersionInfo);

	ULARGE_INTEGER FreeBytesAvailableToCaller, TotalNumberOfBytes, TotalNumberOfFreeBytes;
	GetDiskFreeSpaceEx(RootPath, &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);
	if(DiskTestSize > TotalNumberOfFreeBytes.QuadPart / 1024 / 1024 )
	{
		AfxMessageBox(((CDiskMarkDlg*)dlg)->m_MesDiskCapacityError);
		((CDiskMarkDlg*)dlg)->m_DiskBenchStatus = FALSE;
		return FALSE;
	}

	CString title;
	title.Format(L"Preparing... Create Test File");
	::PostMessage(((CDiskMarkDlg*)dlg)->GetSafeHwnd(), WM_UPDATE_MESSAGE, (WPARAM)& title, 0);

// Preapare Test File
	hFile = ::CreateFile(TestFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING|FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(((CDiskMarkDlg*)dlg)->m_MesDiskCreateFileError);
		((CDiskMarkDlg*)dlg)->m_DiskBenchStatus = FALSE;
		return FALSE;
	}

// Set End Of File to prevent fragmentation of test file
	LARGE_INTEGER nFileSize;
	nFileSize.QuadPart = 1024 * 1024 * DiskTestSize;

	LARGE_INTEGER nStart;
	nStart.QuadPart = 0;

	SetFilePointerEx(hFile, nFileSize, NULL, FILE_BEGIN);
	SetEndOfFile(hFile);
	SetFilePointerEx(hFile, nStart, NULL, FILE_BEGIN);

// COMPRESSION_FORMAT_NONE
	USHORT lpInBuffer = COMPRESSION_FORMAT_NONE;
	DWORD lpBytesReturned = 0;
	DeviceIoControl(hFile, FSCTL_SET_COMPRESSION, (LPVOID) &lpInBuffer,
				sizeof(USHORT), NULL, 0, (LPDWORD)&lpBytesReturned, NULL);

// Fill Test Data
	char* buf = NULL;
	int BufSize;
	int Loop;
	int i;
	DWORD writesize;
	BufSize = 1024 * 1024;
	Loop = (int)DiskTestSize;

	buf = (char*) VirtualAlloc(NULL, BufSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (buf == NULL)
	{
		AfxMessageBox(L"Failed VirtualAlloc().");
		((CDiskMarkDlg*) dlg)->m_DiskBenchStatus = FALSE;
		return FALSE;
	}

	if (((CDiskMarkDlg*) dlg)->m_TestData == TEST_DATA_ALL0X00)
	{
		for (i = 0; i < BufSize; i++)
		{
			buf[i] = 0;
		}
	}
	else
	{
		// Compatible with DiskSpd
		for (i = 0; i < BufSize; i++)
		{
			buf[i] = (char) (rand() % 256);
		}
	}

	for (i = 0; i < Loop; i++)
	{
		if (((CDiskMarkDlg*) dlg)->m_DiskBenchStatus)
		{
			result = WriteFile(hFile, buf, BufSize, &writesize, NULL);
		}
		else
		{
			CloseHandle(hFile);
			VirtualFree(buf, 0, MEM_RELEASE);
			((CDiskMarkDlg*) dlg)->m_DiskBenchStatus = FALSE;
			return FALSE;
		}
	}
	VirtualFree(buf, 0, MEM_RELEASE);
	CloseHandle(hFile);

	return TRUE;
}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if(idEvent == TIMER_ID)
	{
		WaitFlag = FALSE;
		KillTimer(hwnd, idEvent);
	}
}

UINT Exit(void* dlg)
{
	DeleteFile(TestFilePath);
	RemoveDirectory(TestFileDir);

	static CString cstr;
	cstr = L"";

	if(((CDiskMarkDlg*)dlg)->m_TestData == TEST_DATA_ALL0X00)
	{
		cstr = ALL_0X00_0FILL;
	}

	::PostMessage(((CDiskMarkDlg*)dlg)->GetSafeHwnd(), WM_UPDATE_MESSAGE, NULL, (LPARAM)&cstr);
	::PostMessage(((CDiskMarkDlg*)dlg)->GetSafeHwnd(), WM_EXIT_BENCHMARK, 0, 0);

	((CDiskMarkDlg*)dlg)->m_DiskBenchStatus = FALSE;
	((CDiskMarkDlg*)dlg)->m_WinThread = NULL;

	return 0;
}

void DiskSpd(void* dlg, DISK_SPD_CMD cmd)
{
	static CString cstr;
	double *maxScore = NULL;
	double *minLatency = NULL;
	CString command;
	CString title;
	CString option;
	CString bufOption;

	int duration = 5;
	int index = 0;
	int j = 0;

	if (!((CDiskMarkDlg*) dlg)->m_DiskBenchStatus)
	{
		return;
	}

	if (((CDiskMarkDlg*) dlg)->m_TestData == TEST_DATA_ALL0X00)
	{
		bufOption += L" -Z";
	}
	else
	{
		switch (cmd)
		{
		case TEST_WRITE_0:
		case TEST_WRITE_1:
		case TEST_WRITE_2:
		case TEST_WRITE_3:
		case TEST_WRITE_4:
		case TEST_WRITE_5:
		case TEST_WRITE_6:
		case TEST_WRITE_7:
		case TEST_WRITE_8:
			index = cmd - TEST_WRITE_0;
			cstr.Format(L" -Z%dK", BenchSize[index]);
			bufOption += cstr;
			break;
		case TEST_MIX_0:
		case TEST_MIX_1:
		case TEST_MIX_2:
		case TEST_MIX_3:
		case TEST_MIX_4:
		case TEST_MIX_5:
		case TEST_MIX_6:
		case TEST_MIX_7:
		case TEST_MIX_8:
			index = cmd - TEST_MIX_0;
			cstr.Format(L" -Z%dK", BenchSize[index]);
			bufOption += cstr;
			break;
		}
	}

	switch (cmd)
	{
	case TEST_READ_0:
	case TEST_READ_1:
	case TEST_READ_2:
	case TEST_READ_3:
	case TEST_READ_4:
	case TEST_READ_5:
	case TEST_READ_6:
	case TEST_READ_7:
	case TEST_READ_8:
		index = cmd - TEST_READ_0;
		if (BenchType[index])
		{
			title.Format(L"Random Read");
			option.Format(L"-b%dK -o%d -t%d -W0 -S -w0 -r", BenchSize[index], BenchQueues[index], BenchThreads[index]);
		}
		else
		{
			title.Format(L"Sequential Read");
			option.Format(L"-b%dK -o%d -t%d -W0 -S -w0", BenchSize[index], BenchQueues[index], BenchThreads[index]);
		}
		maxScore = &(((CDiskMarkDlg*) dlg)->m_ReadScore[index]);
		minLatency = &(((CDiskMarkDlg*)dlg)->m_ReadLatency[index]);
		break;
	case TEST_WRITE_0:
	case TEST_WRITE_1:
	case TEST_WRITE_2:
	case TEST_WRITE_3:
	case TEST_WRITE_4:
	case TEST_WRITE_5:
	case TEST_WRITE_6:
	case TEST_WRITE_7:
	case TEST_WRITE_8:
		index = cmd - TEST_WRITE_0;
		if (BenchType[index])
		{
			title.Format(L"Random Write");
			option.Format(L"-b%dK -o%d -t%d -W0 -S -w100 -r", BenchSize[index], BenchQueues[index], BenchThreads[index]);
		}
		else
		{
			title.Format(L"Sequential Write");
			option.Format(L"-b%dK -o%d -t%d -W0 -S -w100", BenchSize[index], BenchQueues[index], BenchThreads[index]);
		}
		option += bufOption;
		maxScore = &(((CDiskMarkDlg*)dlg)->m_WriteScore[index]);
		minLatency = &(((CDiskMarkDlg*)dlg)->m_WriteLatency[index]);
		break;
#ifdef MIX_MODE
	case TEST_MIX_0:
	case TEST_MIX_1:
	case TEST_MIX_2:
	case TEST_MIX_3:
	case TEST_MIX_4:
	case TEST_MIX_5:
	case TEST_MIX_6:
	case TEST_MIX_7:
	case TEST_MIX_8:
		index = cmd - TEST_MIX_0;
		if (BenchType[index])
		{
			title.Format(L"Random Mix");
			option.Format(L"-b%dK -o%d -t%d -W0 -S -w%d -r", BenchSize[index], BenchQueues[index], BenchThreads[index], MixRatio);
		}
		else
		{
			title.Format(L"Sequential Mix");
			option.Format(L"-b%dK -o%d -t%d -W0 -S -w%d", BenchSize[index], BenchQueues[index], BenchThreads[index], MixRatio);
		}
		option += bufOption;
		maxScore = &(((CDiskMarkDlg*)dlg)->m_MixScore[index]);
		minLatency = &(((CDiskMarkDlg*)dlg)->m_MixLatency[index]);
		break;
#endif
	}

	option += L" -ag";
	if(IsWin8orLater() && BenchType[index] == 0 && BenchThreads[index] > 1) // Sequential 
	{
		option += L" -si";
	}

	double score = 0.0;
	double latency = 0.0;

	if (maxScore == NULL || minLatency == NULL)
	{
		return ;
	}
	*maxScore = 0.0;
	*minLatency = -1.0;

	for (j = 0; j <= DiskTestCount; j++)
	{
		if (j == 0)
		{
			duration = ((CDiskMarkDlg*)dlg)->m_MeasureTime;
			cstr.Format(L"Preparing... %s", title.GetString());
		}
		else
		{
			duration = ((CDiskMarkDlg*)dlg)->m_MeasureTime;
			cstr.Format(L"%s (%d/%d)", title.GetString(), j, DiskTestCount);
		}
		::PostMessage(((CDiskMarkDlg*) dlg)->GetSafeHwnd(), WM_UPDATE_MESSAGE, (WPARAM) &cstr, 0);
		

		command.Format(L"\"%s\" %s -d%d -A%d -L \"%s\"", (LPTSTR)DiskSpdExe.GetString(), (LPTSTR)option.GetString(), duration, GetCurrentProcessId(), (LPTSTR)TestFilePath.GetString());

		score = ExecAndWait((TCHAR*) (command.GetString()), TRUE, &latency) / 10 / 1000.0;

		if (j > 0 && score > *maxScore)
		{
			*maxScore = score;
			::PostMessage(((CDiskMarkDlg*) dlg)->GetSafeHwnd(), WM_UPDATE_SCORE, 0, 0);
		}

		if (j > 0 && score > 0.0 && (latency < *minLatency || *minLatency < 0))
		{
			*minLatency = latency;
			::PostMessage(((CDiskMarkDlg*)dlg)->GetSafeHwnd(), WM_UPDATE_SCORE, 0, 0);
		}

		if (!((CDiskMarkDlg*) dlg)->m_DiskBenchStatus)
		{
			return;
		}
	}
	::PostMessage(((CDiskMarkDlg*) dlg)->GetSafeHwnd(), WM_UPDATE_SCORE, 0, 0);
}
