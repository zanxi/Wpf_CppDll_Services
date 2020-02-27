#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include ".\Services.h"
#include ".\conv.h"



class ServiceHandle
{
    SC_HANDLE _handle = nullptr;

    void Close()
    {
        if (_handle != nullptr)
            ::CloseServiceHandle(_handle);
    }

public:
    ServiceHandle(SC_HANDLE const handle = nullptr) noexcept :_handle(handle) {}

    ServiceHandle(ServiceHandle&& other) noexcept : _handle(std::move(other._handle)) {}

    ServiceHandle& operator=(SC_HANDLE const handle)
    {
        if (_handle != handle)
        {
            Close();

            _handle = handle;
        }

        return *this;
    }

    ServiceHandle& operator=(ServiceHandle&& other)
    {
        if (this != &other)
        {
            _handle = std::move(other._handle);
            other._handle = nullptr;
        }

        return *this;
    }

    operator SC_HANDLE() const noexcept { return _handle; }

    explicit operator bool() const noexcept { return _handle != nullptr; }

    ~ServiceHandle()
    {
        Close();
    }
};


std::vector<ServiceStatusProcess> EnumerateServices(
    ServiceType const type,
    ServiceState const state,
    ServiceString const* machine,
    ServiceString const* dbname,
    ServiceString const* groupName)
{
    std::vector<ServiceStatusProcess> ssps;

    auto scHandle = ServiceHandle
    {
       ::OpenSCManager(
          machine == nullptr ? nullptr : machine->c_str(),
          dbname == nullptr ? nullptr : dbname->c_str(),
          SC_MANAGER_ENUMERATE_SERVICE)
    };
    auto bytesNeeded = DWORD{ 0 };
    auto servicesReturnedCount = DWORD{ 0 };
    auto resumeHandle = DWORD{ 0 };

    do
    {
        if (!EnumServicesStatusEx(
            scHandle,
            SC_ENUM_PROCESS_INFO,
            static_cast<DWORD>(type),
            static_cast<DWORD>(state),
            nullptr,
            0,
            &bytesNeeded,
            &servicesReturnedCount,
            &resumeHandle,
            groupName == nullptr ? nullptr : groupName->c_str()))
        {
            if (ERROR_MORE_DATA == ::GetLastError())
            {
                std::vector<unsigned char> buffer(bytesNeeded, 0);

                if (EnumServicesStatusEx(
                    scHandle,
                    SC_ENUM_PROCESS_INFO,
                    static_cast<DWORD>(type),
                    static_cast<DWORD>(state),
                    reinterpret_cast<LPBYTE>(buffer.data()),
                    bytesNeeded,
                    &bytesNeeded,
                    &servicesReturnedCount,
                    nullptr,
                    groupName == nullptr ? nullptr : groupName->c_str()))
                {
                    auto essp = reinterpret_cast<LPENUM_SERVICE_STATUS_PROCESS>(buffer.data());

                    for (auto i = DWORD{ 0 }; i < servicesReturnedCount; ++i)
                    {
                        auto ssp = ServiceStatusProcess{};
                        ssp.ServiceName = essp[i].lpServiceName;
                        ssp.DisplayName = essp[i].lpDisplayName;
                        ssp.Status = essp[i].ServiceStatusProcess;                    

                        ssps.push_back(ssp);
                    }
                }
                else break;
            }
            else break;
        }
    } while (resumeHandle != 0);

    return ssps;
}

std::string GetServiceConfig(LPCTSTR lpszServiceName)
{
    std::string str_group_path = "";
    std::string rl = "|";

    SC_HANDLE schService, schSCManager;
    LPQUERY_SERVICE_CONFIG lpqscBuf;    
    LPSERVICE_DESCRIPTION lpqscBuf2;
    DWORD dwBytesNeeded;
    BOOL bSuccess = TRUE;    
    schSCManager = OpenSCManager(
        NULL, // local machine
        NULL, // SERVICES_ACTIVE_DATABASE database is opened by default
        SC_MANAGER_ALL_ACCESS); // full access rights, not a good choice
    if (NULL == schSCManager) { return " error schSCManager  | "; }    
    else {};

    schService = OpenService(
        schSCManager, // SCManager database
        lpszServiceName, // name of service
        SERVICE_QUERY_CONFIG);// need QUERY access
    if (schService == NULL)
    {
        //printf("OpenService() failed, error: %d", GetLastError());
        return " error schService  | ";
        //return FALSE;
    }
    else {}
    //printf("OpenService() is OK\n");

// Allocate a buffer for the configuration information.
    lpqscBuf = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 4096);
    if (lpqscBuf == NULL)
    {
        return " error Alloc lpqscBuf  | ";
        //return FALSE;
    }

    lpqscBuf2 = (LPSERVICE_DESCRIPTION)LocalAlloc(LPTR, 4096);
    if (lpqscBuf2 == NULL)
    {
        return " error Alloc lpqscBuf2  | ";
        //return FALSE;
    }
    // Get the Windows service configuration information.
    if (!QueryServiceConfig(schService, lpqscBuf, 4096, &dwBytesNeeded))
    {
        //printf("QueryServiceConfig() failed, error: %d", GetLastError());
        bSuccess = FALSE;
    }
    else
        //printf("QueryServiceConfig() is OK\n");

        if (!QueryServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)lpqscBuf2, 4096, &dwBytesNeeded))
        {
            return " | ";
            bSuccess = FALSE;
        }
        else {}
    
    if (lpqscBuf->lpLoadOrderGroup != NULL)
        str_group_path += conv::stdlocal::convert(lpqscBuf->lpLoadOrderGroup) + rl;
    else str_group_path += " " + rl;
    str_group_path += conv::stdlocal::convert(lpqscBuf->lpBinaryPathName);
        
    LocalFree(lpqscBuf);
    LocalFree(lpqscBuf2);
    return str_group_path;
}


std::vector<ServiceStatusProcess> services;
//std::vector<std::string> 
int InitInfoServicesWindowSystem()
{
    ::services.clear();
    ::services = EnumerateServices();
    return ::services.size();
}

int GetCountServicesWindowSystem()
{
    return ::services.size();
}

// Серьезность	Код	Описание	Проект	Файл	Строка	Состояние подавления
//Ошибка	C4996	'strcpy': This function or variable may be unsafe.Consider using strcpy_s instead.To disable deprecation, 
//use _CRT_SECURE_NO_WARNINGS.See online help for details.Dll_Win32_002	D : \_V2020_\_Rabota_2020__\_Nuclear___\Dll_Win32_002\Services.cpp	238


char* GetInfoServicesWindowSystem(int k)
{   
    //return new char[20]{" errrorr "};
       

    //std::string st__ = "error";    
    //char* cstr = (char*)LocalAlloc(LPTR, st__.size() + 1);
    //strcpy(cstr,st__.c_str());
    //return cstr;


    std::vector<std::string> strall;
    int i = 0;
    int pid = 12345;
    std::string rl = "|";
    
    //if (::services.size() < 1)return "";
//    if (k<0||k>::services.size())return "";
    //for (int k = 0; k < services.size(); k++)
    std::string str_info = "";
    
    string group_path = GetServiceConfig(::services[k].ServiceName.c_str());
    str_info += conv::stdlocal::convert(::services[k].ServiceName) +
        rl + // Name service
        "12345" +
        rl + // pid service
        conv::stdlocal::convert(::services[k].DisplayName) +
        rl +  // descriptiopn
        conv::stdlocal::convert(
            ServiceStatusToString(static_cast<ServiceStatus>(
                ::services[k].Status.dwCurrentState))) +
        rl
        +
        group_path; // group & path

        //std::cout << "\n\n" << str_info << std::endl;
        //strall.push_back(str_info);
        //if (i > 5)break;
        i++;
    
        char* cstr = (char*)LocalAlloc(LPTR, str_info.size() + 1);
        strcpy(cstr, str_info.c_str());

     //st__ = "error";
    //int nn = st__.size() + 1;
    //cstr = (char*)LocalAlloc(LPTR, st__.size() + 1);
    //strcpy(cstr,st__.c_str());
    //return cstr;
        
        return cstr;
    //return str_info;
    //return strall;
}


bool _StartService(const TCHAR* name)
{
    SC_HANDLE manager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    SC_HANDLE service = OpenService(manager, name, SERVICE_START);
    if (!service)
        return false;

    bool ret = ::StartService(service, 0, NULL);

    CloseServiceHandle(service);
    CloseServiceHandle(manager);
    return ret;
}

bool _StopService(const TCHAR* name)
{
    SC_HANDLE manager = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    SC_HANDLE service = OpenService(manager, name, SERVICE_STOP);
    if (!service)
        return false;

    SERVICE_STATUS status;
    bool ret = ControlService(service, SERVICE_CONTROL_STOP, &status);

    CloseServiceHandle(service);
    CloseServiceHandle(manager);    
    return ret;
}