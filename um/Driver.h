/**
 * @file Driver.h
 * @author redbg (redbg@softsec.org)
 * @brief 
 * @version 0.1
 * @date 2021-11-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <Windows.h>
#include <tchar.h>
#include "../shared/shared.h"

#pragma comment(lib, "Advapi32.lib")

#define SS_SERVICE_NAME TEXT("softsec")
#define SS_DRIVER_NAME TEXT("softsec.sys")
#define SS_DEVICE_NAME TEXT("\\\\.\\softsec")

namespace SS::Driver
{
    HANDLE hDevice = NULL;

    static BOOL Load()
    {
        BOOL bRet = TRUE;
        SC_HANDLE hSCManager = NULL;
        SC_HANDLE hService = NULL;
        TCHAR szBinaryPathName[MAX_PATH] = {};

        GetCurrentDirectory(MAX_PATH, szBinaryPathName);
        _tcscat(szBinaryPathName, TEXT("\\"));
        _tcscat(szBinaryPathName, SS_DRIVER_NAME);

        do
        {
            // 1. OpenSCManager
            hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (hSCManager == NULL)
            {
                break;
            }

            // 2. CreateService
            hService = CreateService(hSCManager,
                                     SS_SERVICE_NAME,
                                     nullptr,
                                     SERVICE_ALL_ACCESS,
                                     SERVICE_KERNEL_DRIVER,
                                     SERVICE_DEMAND_START,
                                     SERVICE_ERROR_NORMAL,
                                     szBinaryPathName,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr);
            if (hService == NULL)
            {
                break;
            }

            // 3. StartService
            if (StartService(hService, 0, nullptr) == FALSE)
            {
                break;
            }
        } while (FALSE);

        // 4. Device
        CloseHandle(hDevice);
        hDevice = CreateFile(SS_DEVICE_NAME,
                             0,
                             0,
                             NULL,
                             OPEN_EXISTING,
                             0,
                             NULL);

        bRet = (hDevice == INVALID_HANDLE_VALUE ? FALSE : TRUE);

        // CloseServiceHandle
        if (hService != NULL)
            CloseServiceHandle(hService);
        if (hSCManager != NULL)
            CloseServiceHandle(hSCManager);

        return bRet;
    }

    static BOOL Unload()
    {
        BOOL bRet = TRUE;
        SC_HANDLE hSCManager = NULL;
        SC_HANDLE hService = NULL;
        SERVICE_STATUS serviceStatus = {};

        CloseHandle(hDevice);

        do
        {
            // 1. OpenSCManager
            hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (hSCManager == NULL)
            {
                bRet = FALSE;
                break;
            }

            // 2. OpenService
            hService = OpenService(hSCManager, SS_SERVICE_NAME, SERVICE_ALL_ACCESS);
            if (hService == NULL)
            {
                bRet = FALSE;
                break;
            }

            // 3. ControlService
            bRet = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);

            // 4. DeleteService
            bRet = DeleteService(hService);
        } while (FALSE);

        // CloseServiceHandle
        if (hService != NULL)
            CloseServiceHandle(hService);
        if (hSCManager != NULL)
            CloseServiceHandle(hSCManager);
        return bRet;
    }
}
