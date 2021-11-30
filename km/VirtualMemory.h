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

            // OpenProcess
            ntStatus = OpenProcess();
            if (!NT_SUCCESS(ntStatus))
            {
                return ntStatus;
            }

            // Dispatch Method
            switch (Method)
            {
            case METHOD::Allocate:
                ntStatus = DispatchAllocate();
                break;
            case METHOD::Free:
                ntStatus = DispatchFree();
                break;
            default:
                ntStatus = STATUS_INVALID_PARAMETER;
                break;
            }

            ZwClose(this->hProcess);
            return ntStatus;
        }

        NTSTATUS OpenProcess()
        {
            NTSTATUS ntStatus = STATUS_SUCCESS;

            this->hProcess = nullptr;
            CLIENT_ID ClientId = {(HANDLE)this->ProcessId, 0};
            OBJECT_ATTRIBUTES ObjectAttributes = {sizeof(OBJECT_ATTRIBUTES)};

            ntStatus = ZwOpenProcess(&this->hProcess,
                                     GENERIC_ALL,
                                     &ObjectAttributes,
                                     &ClientId);

            if (!NT_SUCCESS(ntStatus))
            {
                KdPrint(("SS::KM::VirtualMemory::OpenProcess() error! ntStatus:[%x]\n", ntStatus));
                return ntStatus;
            }

            return ntStatus;
        }

        NTSTATUS DispatchAllocate()
        {
            return ZwAllocateVirtualMemory(hProcess,
                                           (PVOID *)&BaseAddress,
                                           0,
                                           (PSIZE_T)&Size,
                                           AllocationType,
                                           Protection);
        }

        NTSTATUS DispatchFree()
        {
            return ZwFreeVirtualMemory(hProcess,
                                       (PVOID *)&BaseAddress,
                                       (PSIZE_T)&Size,
                                       AllocationType);
        }
    };
}
