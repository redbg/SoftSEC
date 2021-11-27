#pragma once
#include <ntifs.h>
#include "../shared/shared.h"

namespace SS
{
    struct VirtualMemory : SS::BASE::VirtualMemory
    {
        NTSTATUS Dispatch()
        {
            NTSTATUS ntStatus = STATUS_SUCCESS;

            switch (Method)
            {
            case METHOD::Allocate:
                ntStatus = DispatchAllocate();
                break;
            case METHOD::Free:
                ntStatus = DispatchFree();
                break;
            default:
                ntStatus = STATUS_INVALID_DEVICE_REQUEST;
                break;
            }

            return ntStatus;
        }

        NTSTATUS DispatchAllocate()
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

        NTSTATUS DispatchFree()
        {
            NTSTATUS ntStatus = STATUS_SUCCESS;

            return ntStatus;
        }
    };
}
