/**
 * @file DriverEntry.cpp
 * @author redbg (redbg@softsec.org)
 * @brief 
 * @version 0.1
 * @date 2021-11-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <ntddk.h>
#include <intrin.h>

#include "../shared/amd64.h"

VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    DbgPrint("DriverUnload\n");

    DbgSetDebugFilterState(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, FALSE);
}

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    DbgSetDebugFilterState(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, TRUE);

    DbgPrint("DriverEntry\n");

    DriverObject->DriverUnload = DriverUnload;

    DbgBreakPoint();

    SS::GDTR gdtr;

    gdtr.SGDT();

    gdtr.Print();


    return STATUS_SUCCESS;
}
