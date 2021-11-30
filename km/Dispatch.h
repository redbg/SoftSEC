#pragma once
#include <ntifs.h>
#include "VirtualMemory.h"

namespace SS::Dispatch
{
    NTSTATUS DispatchCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
    {
        Irp->IoStatus.Status = STATUS_SUCCESS;
        Irp->IoStatus.Information = 0;

        IoCompleteRequest(Irp, IO_NO_INCREMENT);

        return STATUS_SUCCESS;
    }

    NTSTATUS DispatchDeviceControl(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
    {
        NTSTATUS ntStatus = STATUS_SUCCESS;

        PIO_STACK_LOCATION irpSp;
        irpSp = IoGetCurrentIrpStackLocation(Irp);
        ULONG inputBufferLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;
        ULONG outputBufferLength = irpSp->Parameters.DeviceIoControl.OutputBufferLength;
        PVOID buffer = Irp->AssociatedIrp.SystemBuffer;

        switch (irpSp->Parameters.DeviceIoControl.IoControlCode)
        {
        case IOCTL_SS_VirtualMemory:
            ntStatus = ((SS::VirtualMemory *)buffer)->Dispatch();
            Irp->IoStatus.Information = sizeof(SS::VirtualMemory);
            break;
        default:
            ntStatus = STATUS_INVALID_DEVICE_REQUEST;
            break;
        }

        Irp->IoStatus.Status = ntStatus;

        IoCompleteRequest(Irp, IO_NO_INCREMENT);

        return ntStatus;
    }
}
