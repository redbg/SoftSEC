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
            switch (this->Method)
            {
            case METHOD::Allocate:
                ntStatus = Allocate();
                break;
            case METHOD::Free:
                ntStatus = Free();
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

        NTSTATUS Allocate()
        {
            return ZwAllocateVirtualMemory(hProcess,
                                           (PVOID *)&BaseAddress,
                                           0,
                                           (PSIZE_T)&Size,
                                           AllocationType,
                                           Protection);
        }

        NTSTATUS Free()
        {
            return ZwFreeVirtualMemory(hProcess,
                                       (PVOID *)&BaseAddress,
                                       (PSIZE_T)&Size,
                                       AllocationType);
        }
    };
}
