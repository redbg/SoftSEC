/**
 * @file softsec.h
 * @author redbg (redbg@softsec.org)
 * @brief 
 * @version 0.1
 * @date 2021-11-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <Windows.h>

namespace SS
{
    /**
     * @brief 
     * 
     * @param lpServiceName 
     * @param lpBinaryPathName 
     * @return true 
     * @return false 
     */
    bool LoadDriver(LPCTSTR lpServiceName, LPCTSTR lpBinaryPathName);

    /**
     * @brief 
     * 
     * @param lpServiceName 
     * @return true 
     * @return false 
     */
    bool UnloadDriver(LPCTSTR lpServiceName);
}
