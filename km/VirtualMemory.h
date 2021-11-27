#pragma once
#include <ntifs.h>
#include "../shared/shared.h"

namespace SS::KM
{
    struct VirtualMemory : SS::VirtualMemory
    {
        NTSTATUS Dispatch()
        {
            NTSTATUS ntStatus = STATUS_SUCCESS;

            switch (Method)
            {
            case METHOD::Allocate:
                ntStatus = Allocate();
                break;
            case METHOD::Free:
                ntStatus = Free();
                break;
            default:
                ntStatus = STATUS_INVALID_DEVICE_REQUEST;
                break;
            }

            return ntStatus;
        }

        NTSTATUS Allocate()
        {
            NTSTATUS ntStatus = STATUS_SUCCESS;

            HANDLE hProcess = nullptr;
            CLIENT_ID ClientId = {(HANDLE)ProcessId, 0};
            OBJECT_ATTRIBUTES ObjectAttributes = {sizeof(OBJECT_ATTRIBUTES)};

            DbgBreakPoint();

            ntStatus = ZwOpenProcess(&hProcess, GENERIC_ALL, &ObjectAttributes, &ClientId);
            if (!NT_SUCCESS(ntStatus))
            {
                KdPrint(("SS::KM::VirtualMemory::Allocate() -> ZwOpenProcess() error! ntStatus:[%x]\n", ntStatus));
                return ntStatus;
            }

            ntStatus = ZwAllocateVirtualMemory(hProcess, (PVOID *)&BaseAddress, 0, (PSIZE_T)&Size, AllocationType, Protect);

            ntStatus = ZwClose(hProcess);
            if (!NT_SUCCESS(ntStatus))
            {
                KdPrint(("SS::KM::VirtualMemory::Allocate() -> ZwClose() error! ntStatus:[%x]\n", ntStatus));
                return ntStatus;
            }

            return ntStatus;
        }

        NTSTATUS Free()
        {
            NTSTATUS ntStatus = STATUS_SUCCESS;

            return ntStatus;
        }
    };
}
