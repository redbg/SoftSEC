/**
 * @file softsec.cpp
 * @author redbg (redbg@softsec.org)
 * @brief 
 * @version 0.1
 * @date 2021-11-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "softsec.h"

namespace SS
{
    bool LoadDriver(LPCTSTR lpServiceName, LPCTSTR lpBinaryPathName)
    {
        // OpenSCManager
        SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (hSCManager == NULL)
        {
            return false;
        }

        // CreateService
        SC_HANDLE hService = CreateService(hSCManager,
                                           lpServiceName,
                                           nullptr,
                                           SERVICE_ALL_ACCESS,
                                           SERVICE_KERNEL_DRIVER,
                                           SERVICE_DEMAND_START,
                                           SERVICE_ERROR_NORMAL,
                                           lpBinaryPathName,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr,
                                           nullptr);

        if (hService == NULL)
        {
            CloseServiceHandle(hSCManager);
            return false;
        }

        // StartService
        if (StartService(hService, 0, nullptr) == FALSE)
        {
            CloseServiceHandle(hSCManager);
            CloseServiceHandle(hService);
            return false;
        }

        CloseServiceHandle(hSCManager);
        CloseServiceHandle(hService);
        return true;
    }

    bool UnloadDriver(LPCTSTR lpServiceName)
    {
        // OpenSCManager
        SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (hSCManager == NULL)
        {
            return false;
        }

        // OpenService
        SC_HANDLE hService = OpenService(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);

        if (hService == NULL)
        {
            CloseServiceHandle(hSCManager);
            return false;
        }

        // ControlService
        SERVICE_STATUS ServiceStatus = {};
        ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);

        // DeleteService
        DeleteService(hService);

        CloseServiceHandle(hSCManager);
        CloseServiceHandle(hService);
        return true;
    }
}
