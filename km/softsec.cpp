#include <ntddk.h>
#include "../shared/shared.h"

VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    KdPrint(("SS::DriverUnload()\n"));
}

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    KdPrint(("SS::DriverEntry()\n"));

    NTSTATUS ntStatus = STATUS_SUCCESS;
    UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(NT_DEVICE_NAME);
    UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(DOS_DEVICE_NAME);
    PDEVICE_OBJECT DeviceObject = nullptr;

    // IoCreateDevice
    ntStatus = IoCreateDevice(DriverObject,
                              0,
                              &DeviceName,
                              FILE_DEVICE_UNKNOWN,
                              FILE_DEVICE_SECURE_OPEN,
                              FALSE,
                              &DeviceObject);
    if (!NT_SUCCESS(ntStatus))
    {
        KdPrint(("IoCreateDevice() error! ntStatus:[%x]\n", ntStatus));
        return ntStatus;
    }

    // IoCreateSymbolicLink
    ntStatus = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);
    if (!NT_SUCCESS(ntStatus))
    {
        KdPrint(("IoCreateSymbolicLink() error! ntStatus:[%x]\n", ntStatus));
        IoDeleteDevice(DeviceObject);
        return ntStatus;
    }

    DriverObject->DriverUnload = DriverUnload;

    return ntStatus;
}
