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

namespace SS::UM
{
    BOOL LoadDriver(LPCTSTR lpServiceName, LPCTSTR lpBinaryPathName)
    {
        BOOL bRet = TRUE;
        SC_HANDLE hSCManager = NULL;
        SC_HANDLE hService = NULL;

        do
        {
            // 1. OpenSCManager
            hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (hSCManager == NULL)
            {
                bRet = FALSE;
                break;
            }

            // 2. CreateService
            hService = CreateService(hSCManager,
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
                bRet = FALSE;
                break;
            }

            // 3. StartService
            if (StartService(hService, 0, nullptr) == FALSE)
            {
                bRet = FALSE;
                break;
            }
        } while (FALSE);

        // CloseServiceHandle
        if (hSCManager != NULL)
            CloseServiceHandle(hSCManager);
        if (hService != NULL)
            CloseServiceHandle(hService);
        return true;
    }

    BOOL UnloadDriver(LPCTSTR lpServiceName)
    {
        BOOL bRet = TRUE;
        SC_HANDLE hSCManager = NULL;
        SC_HANDLE hService = NULL;
        SERVICE_STATUS serviceStatus = {};

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
            hService = OpenService(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);
            if (hService == NULL)
            {
                bRet = FALSE;
                break;
            }

            // 3. ControlService
            if (ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus) == FALSE)
            {
                bRet = FALSE;
                break;
            }

            // 4. DeleteService
            if (DeleteService(hService) == FALSE)
            {
                bRet = FALSE;
                break;
            }
        } while (FALSE);

        // CloseServiceHandle
        if (hSCManager != NULL)
            CloseServiceHandle(hSCManager);
        if (hService != NULL)
            CloseServiceHandle(hService);
        return bRet;
    }
}
