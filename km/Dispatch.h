#pragma once
#include <ntifs.h>
#include "VirtualMemory.h"

namespace SS::KM::Dispatch
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
        ULONG InputBufferLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;
        ULONG OutputBufferLength = irpSp->Parameters.DeviceIoControl.OutputBufferLength;

        switch (irpSp->Parameters.DeviceIoControl.IoControlCode)
        {
        case IOCTL_SS_VirtualMemory:
            ntStatus = ((SS::KM::VirtualMemory *)Irp->AssociatedIrp.SystemBuffer)->Dispatch();
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
