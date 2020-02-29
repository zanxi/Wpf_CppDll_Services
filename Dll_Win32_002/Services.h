#pragma once

#include ".\stdwin.h" 
#include <tchar.h>


#ifdef Processes_Modules
#define Processes_Modules_API __declspec(dllexport)
#else
#define Processes_Modules_API __declspec(dllimport)
#endif


#ifdef UNICODE
#define ServiceString   std::wstring
#else
#define ServiceString   std::string
#endif

struct ServiceStatusProcess
{
    ServiceString           ServiceName;
    ServiceString           DisplayName;
    SERVICE_STATUS_PROCESS  Status;
};

enum class ServiceType
{
    // A Kernel device driver such as a hard disk or other low-level hardware device driver.
    KernelDriver = SERVICE_KERNEL_DRIVER,

    // A file system driver, which is also a Kernel device driver.
    FileSystemDriver = SERVICE_FILE_SYSTEM_DRIVER,

    // A service for a hardware device that requires its own driver.
    Adapter = SERVICE_ADAPTER,

    // A file system driver used during startup to determine the file systems present on the system.
    RecognizerDriver = SERVICE_RECOGNIZER_DRIVER,

    // A Win32 service that runs in its own process.
    Win32OwnProcess = SERVICE_WIN32_OWN_PROCESS,

    // A Win32 service that can share a process with other Win32 services.
    Win32ShareProcess = SERVICE_WIN32_SHARE_PROCESS,

    // A service that can communicate with the desktop.
    InteractiveDriver = SERVICE_INTERACTIVE_PROCESS,

    Driver = SERVICE_DRIVER,
    Win32 = SERVICE_WIN32,
    All = SERVICE_TYPE_ALL
};

enum class ServiceStatus
{
    Unknown = 0,
    Stopped = SERVICE_STOPPED,
    Starting = SERVICE_START_PENDING,
    Stopping = SERVICE_STOP_PENDING,
    Running = SERVICE_RUNNING,
    Continuing = SERVICE_CONTINUE_PENDING,
    Pausing = SERVICE_PAUSE_PENDING,
    Paused = SERVICE_PAUSED
};

ServiceString ServiceStatusToString(ServiceStatus const status)
{
    switch (status)
    {
    case ServiceStatus::Stopped:     return _T("Stopped");
    case ServiceStatus::Starting:    return _T("Starting");
    case ServiceStatus::Stopping:    return _T("Stopping");
    case ServiceStatus::Running:     return _T("Running");
    case ServiceStatus::Continuing:  return _T("Continuing");
    case ServiceStatus::Pausing:     return _T("Pausing");
    case ServiceStatus::Paused:      return _T("Paused");
    default:                         return _T("Unknown");
    }
}


enum class ServiceState
{
    // Services that are in the following states: SERVICE_START_PENDING, SERVICE_STOP_PENDING, SERVICE_RUNNING, SERVICE_CONTINUE_PENDING, SERVICE_PAUSE_PENDING, and SERVICE_PAUSED.
    Active = SERVICE_ACTIVE,

    // Services that are in the SERVICE_STOPPED state.
    Inactive = SERVICE_INACTIVE,

    // Combines the SERVICE_ACTIVE and SERVICE_INACTIVE states.
    All = SERVICE_STATE_ALL
};


std::vector<ServiceStatusProcess> EnumerateServices(
    ServiceType const type = ServiceType::All,
    ServiceState const state = ServiceState::All,
    ServiceString const* machine = nullptr,
    ServiceString const* dbname = nullptr,
    ServiceString const* groupName = nullptr);


extern "C" Processes_Modules_API char* GetInfoServicesWindowSystem(int k);
extern "C" Processes_Modules_API std::string GetServiceConfig(LPCTSTR lpszServiceName);
extern "C" Processes_Modules_API int __stdcall InitInfoServicesWindowSystem();
extern "C" Processes_Modules_API int __stdcall GetCountServicesWindowSystem();

extern "C" Processes_Modules_API bool _StartService(const TCHAR* name);
extern "C" Processes_Modules_API bool _StopService(const TCHAR* name);



