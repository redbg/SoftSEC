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
     * 
     * @par example
     * @code {.cpp}
     * SS::LoadDriver(TEXT("softsec"), TEXT("C:\\Users\\Win10_64\\Desktop\\softsec_x64.sys"));
     * @endcode
     */
    bool LoadDriver(LPCTSTR lpServiceName, LPCTSTR lpBinaryPathName);

    /**
     * @brief 
     * 
     * @param lpServiceName 
     * @return true 
     * @return false 
     * 
     * @par example
     * @code {.cpp}
     * SS::UnloadDriver(TEXT("softsec"));
     * @endcode
     */
    bool UnloadDriver(LPCTSTR lpServiceName);
}
