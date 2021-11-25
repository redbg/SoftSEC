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

    ss::GDTR gdtr;

    gdtr.SGDT();

    gdtr.Print();

    return STATUS_SUCCESS;
}
