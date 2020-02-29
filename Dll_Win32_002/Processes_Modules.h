#pragma once
#include ".\stdwin.h"

#ifdef Processes_Modules
#define Processes_Modules_API __declspec(dllexport)
#else
#define Processes_Modules_API __declspec(dllimport)
#endif

extern "C" Processes_Modules_API DWORD PIDByName(WCHAR* AProcessName);
extern "C" Processes_Modules_API VOID PrintModuleList(HANDLE CONST hStdOut, DWORD CONST dwProcessId);
extern "C" Processes_Modules_API BOOL KillProcess( 	IN DWORD dwProcessId );


