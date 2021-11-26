/**
 * @file softsec.h
 * @author redbg (redbg@softsec.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include <ntddk.h>
#include "../shared/amd64.h"

// DriverEntry Routine
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath);
// Unload Routine
VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject);