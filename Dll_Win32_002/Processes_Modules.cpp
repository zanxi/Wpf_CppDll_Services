#pragma comment(lib, "version.lib" )

#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include ".\Processes_Modules.h"

#define _SCL_SECURE_NO_WARNINGS
#define EXAMPLE_MACRO_NAME
#define _CRT_NONSTDC_NO_WARNINGS

string GetPath()
{
    wchar_t path[MAX_PATH];
    //char* ppath = path;
    GetModuleFileName(NULL, path, MAX_PATH);

    int sp = sizeof(path);
    while (sp != 0) {
        if (path[sp - 1] == '\\') {
            path[sp] = '\0';
            break;
        }
        sp--;
    }

    return "";
}

DWORD PIDByName(WCHAR* AProcessName)
{
    HANDLE pHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 ProcessEntry;
    DWORD pid;
    ProcessEntry.dwSize = sizeof(ProcessEntry);
    bool Loop = Process32First(pHandle, &ProcessEntry);

    while (Loop)
    {
        if (wcsstr(ProcessEntry.szExeFile, AProcessName))
        {
            pid = ProcessEntry.th32ProcessID;
            CloseHandle(pHandle);
            return pid;
        }
        Loop = Process32Next(pHandle, &ProcessEntry);
    }
    return 0;
}

TCHAR* GetPathProcess(DWORD pID)
{
    TCHAR  path[256];
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pID);
    HMODULE mod = GetModuleHandle(L"ntdll.dll"); // Если же нужен весь список модулей, то соответственно объявляем массив.
    //EnumProcessModules(pHandle, &mod, sizeof(HMODULE), NULL);
    GetModuleFileNameEx(pHandle, 0, path, 255);
    return path;
}

VOID PrintModuleList(HANDLE CONST hStdOut, DWORD CONST dwProcessId) {
    MODULEENTRY32 meModuleEntry;
    TCHAR szBuff[1024];
    DWORD dwTemp;
    HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE, dwProcessId);
    if (INVALID_HANDLE_VALUE == hSnapshot) {
        return;
    }
    
    meModuleEntry.dwSize = sizeof(MODULEENTRY32);
    Module32First(hSnapshot, &meModuleEntry);
    do {
        wsprintf(szBuff, L"  ba: %08X, bs: %08X, %s, %s\r\n",
            meModuleEntry.modBaseAddr, meModuleEntry.modBaseSize,
            meModuleEntry.szModule, meModuleEntry.szExePath);
        WriteConsole(hStdOut, szBuff, lstrlen(szBuff), &dwTemp, NULL);
    } while (Module32Next(hSnapshot, &meModuleEntry));

    CloseHandle(hSnapshot);
}

VOID PrintProcessList(HANDLE CONST hStdOut) {
    PROCESSENTRY32 peProcessEntry;
    TCHAR szBuff[1024];
    DWORD dwTemp;
    HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) {
        return;
    }


    peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &peProcessEntry);
    do {
        TCHAR filename[MAX_PATH];
        //GetModuleFileNameEx(&peProcessEntry., NULL, filename, MAX_PATH);
        wsprintf(szBuff, L"====== %08X %s ====%s===\r\n",
            peProcessEntry.th32ProcessID, peProcessEntry.szExeFile, GetPathProcess(PIDByName(peProcessEntry.szExeFile)));

        WriteConsole(hStdOut, szBuff, lstrlen(szBuff), &dwTemp, NULL);
        PrintModuleList(hStdOut, peProcessEntry.th32ProcessID);
    } while (Process32Next(hSnapshot, &peProcessEntry));

    CloseHandle(hSnapshot);
}

//**************************** Version Windows ***********************************************

#pragma comment(lib, "version.lib" )

bool GetOSVersionString(WCHAR* version, size_t maxlen)
{
	WCHAR path[_MAX_PATH];
	if (!GetSystemDirectoryW(path, _MAX_PATH))
		return false;

	wcscat_s(path, L"\\kernel32.dll");

	//
	// Based on example code from this article
	// http://support.microsoft.com/kb/167597
	//

	DWORD handle;
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
	DWORD len = GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, path, &handle);
#else
	DWORD len = GetFileVersionInfoSizeW(path, &handle);
#endif
	if (!len)
		return false;

	std::unique_ptr<uint8_t> buff(new (std::nothrow) uint8_t[len]);
	if (!buff)
		return false;

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
	if (!GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, path, 0, len, buff.get()))
#else
	if (!GetFileVersionInfoW(path, 0, len, buff.get()))
#endif
		return false;

	VS_FIXEDFILEINFO* vInfo = nullptr;
	UINT infoSize;

	if (!VerQueryValueW(buff.get(), L"\\", reinterpret_cast<LPVOID*>(&vInfo), &infoSize))
		return false;

	if (!infoSize)
		return false;

	swprintf_s(version, maxlen, L"%u.%u.%u.%u",
		HIWORD(vInfo->dwFileVersionMS),
		LOWORD(vInfo->dwFileVersionMS),
		HIWORD(vInfo->dwFileVersionLS),
		LOWORD(vInfo->dwFileVersionLS));

	return true;
}

//********************************************************************************************

//---------------------------------------------------------------------------
// KillProcess
//
//  Terminates the specified process.
//
//  Parameters:
//	  dwProcessId - identifier of the process to terminate
//
//  Returns:
//	  TRUE, if successful, FALSE - otherwise.
//
#pragma warning(disable : 4996)

BOOL KillProcess(IN DWORD dwProcessId)
{
	HANDLE hProcess;
	DWORD dwError;

	// first try to obtain handle to the process without the use of any
	// additional privileges
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
	if (hProcess == NULL)
	{
		if (GetLastError() != ERROR_ACCESS_DENIED)
			return FALSE;

		OSVERSIONINFO osvi;

		// determine operating system version
		osvi.dwOSVersionInfoSize = sizeof(osvi);
		::GetVersionEx(&osvi);
		//::GetVersionEx((OSVERSIONINFO*)&osvi);
		//IsWindow(osvi.);

		// we cannot do anything else if this is not Windows NT
		if (osvi.dwPlatformId != VER_PLATFORM_WIN32_NT)
			return SetLastError(ERROR_ACCESS_DENIED), FALSE;

		// enable SE_DEBUG_NAME privilege and try again

		TOKEN_PRIVILEGES Priv, PrivOld;
		DWORD cbPriv = sizeof(PrivOld);
		HANDLE hToken;

		// obtain the token of the current thread 
		if (!OpenThreadToken(GetCurrentThread(),
			TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
			FALSE, &hToken))
		{
			if (GetLastError() != ERROR_NO_TOKEN)
				return FALSE;

			// revert to the process token
			if (!OpenProcessToken(GetCurrentProcess(),
				TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
				&hToken))
				return FALSE;
		}

		_ASSERTE(ANYSIZE_ARRAY > 0);

		Priv.PrivilegeCount = 1;
		Priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Priv.Privileges[0].Luid);

		// try to enable the privilege
		if (!AdjustTokenPrivileges(hToken, FALSE, &Priv, sizeof(Priv),
			&PrivOld, &cbPriv))
		{
			dwError = GetLastError();
			CloseHandle(hToken);
			return SetLastError(dwError), FALSE;
		}

		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		{
			// the SE_DEBUG_NAME privilege is not present in the caller's
			// token
			CloseHandle(hToken);
			return SetLastError(ERROR_ACCESS_DENIED), FALSE;
		}

		// try to open process handle again
		hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
		dwError = GetLastError();

		// restore the original state of the privilege
		AdjustTokenPrivileges(hToken, FALSE, &PrivOld, sizeof(PrivOld),
			NULL, NULL);
		CloseHandle(hToken);

		if (hProcess == NULL)
			return SetLastError(FALSE), NULL;
	}

	// terminate the process
	if (!TerminateProcess(hProcess, (UINT)-1))
	{
		dwError = GetLastError();
		CloseHandle(hProcess);
		return SetLastError(dwError), FALSE;
	}

	CloseHandle(hProcess);

	// completed successfully
	return TRUE;
}

typedef LONG	NTSTATUS;
typedef LONG	KPRIORITY;

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)

#define SystemProcessesAndThreadsInformation	5

typedef struct _CLIENT_ID {
	DWORD	    UniqueProcess;
	DWORD	    UniqueThread;
} CLIENT_ID;

typedef struct _UNICODE_STRING {
	USHORT	    Length;
	USHORT	    MaximumLength;
	PWSTR	    Buffer;
} UNICODE_STRING;

typedef struct _VM_COUNTERS {
	SIZE_T	    PeakVirtualSize;
	SIZE_T	    VirtualSize;
	ULONG	    PageFaultCount;
	SIZE_T	    PeakWorkingSetSize;
	SIZE_T	    WorkingSetSize;
	SIZE_T	    QuotaPeakPagedPoolUsage;
	SIZE_T	    QuotaPagedPoolUsage;
	SIZE_T	    QuotaPeakNonPagedPoolUsage;
	SIZE_T	    QuotaNonPagedPoolUsage;
	SIZE_T	    PagefileUsage;
	SIZE_T	    PeakPagefileUsage;
} VM_COUNTERS;

typedef struct _SYSTEM_THREADS {
	LARGE_INTEGER   KernelTime;
	LARGE_INTEGER   UserTime;
	LARGE_INTEGER   CreateTime;
	ULONG			WaitTime;
	PVOID			StartAddress;
	CLIENT_ID	    ClientId;
	KPRIORITY	    Priority;
	KPRIORITY	    BasePriority;
	ULONG			ContextSwitchCount;
	LONG			State;
	LONG			WaitReason;
} SYSTEM_THREADS, * PSYSTEM_THREADS;

// Note that the size of the SYSTEM_PROCESSES structure is different on
// NT 4 and Win2K, but we don't care about it, since we don't access neither
// IoCounters member nor Threads array

typedef struct _SYSTEM_PROCESSES {
	ULONG			NextEntryDelta;
	ULONG			ThreadCount;
	ULONG			Reserved1[6];
	LARGE_INTEGER   CreateTime;
	LARGE_INTEGER   UserTime;
	LARGE_INTEGER   KernelTime;
	UNICODE_STRING  ProcessName;
	KPRIORITY	    BasePriority;
	ULONG			ProcessId;
	ULONG			InheritedFromProcessId;
	ULONG			HandleCount;
	ULONG			Reserved2[2];
	VM_COUNTERS	    VmCounters;
#if _WIN32_WINNT >= 0x500
	IO_COUNTERS	    IoCounters;
#endif
	SYSTEM_THREADS  Threads[1];
} SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;

//---------------------------------------------------------------------------
// KillProcessTreeNtHelper
//
//  This is a recursive helper function that terminates all the processes
//  started by the specified process and them terminates the process itself
//
//  Parameters:
//	  pInfo       - processes information
//	  dwProcessId - identifier of the process to terminate
//
//  Returns:
//	  Win32 error code.
//

