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
    bool LoadDriver(LPCTSTR lpServiceName, LPCTSTR lpBinaryPathName);
    
    bool UnloadDriver(LPCTSTR lpServiceName);
}
