#pragma once
#include <ntifs.h>
#include "../shared/shared.h"

extern "C" NTSTATUS
    NTAPI
    MmCopyVirtualMemory(
        IN PEPROCESS FromProcess,
        IN CONST PVOID FromAddress,
        IN PEPROCESS ToProcess,
        OUT PVOID ToAddress,
        IN SIZE_T BufferSize,
        IN KPROCESSOR_MODE PreviousMode,
        OUT PSIZE_T NumberOfBytesCopied);

extern "C" NTSTATUS
    NTAPI
    ZwProtectVirtualMemory(
        IN HANDLE ProcessHandle,
        IN OUT PVOID *BaseAddress,
        IN OUT PSIZE_T RegionSize,
        IN ULONG NewProtect,
        OUT PULONG OldProtect);

namespace SS
{
    struct VirtualMemory : SS::BASE::VirtualMemory
    {
        NTSTATUS Dispatch()
        {
            // Dispatch Method
            switch (this->Method)
            {
            case METHOD::Allocate:
                KdPrint(("=============== SS::VirtualMemory::Allocate ===============\n"));
                this->ntStatus = Allocate();
                break;
            case METHOD::Free:
                KdPrint(("=============== SS::VirtualMemory::Free ===============\n"));
                this->ntStatus = Free();
                break;
            case METHOD::Copy:
                KdPrint(("=============== SS::VirtualMemory::Copy ===============\n"));
                this->ntStatus = Copy();
                break;
            default:
                this->ntStatus = STATUS_INVALID_PARAMETER;
                break;
            }
            PrintInfo();
            return this->ntStatus;
        }

        void PrintInfo()
        {
            KdPrint(("TargetProcessId:[%d] TargetAddress[0x%p]\n", TargetProcessId, TargetAddress));
            KdPrint(("SourceProcessId:[%d] SourceAddress[0x%p]\n", SourceProcessId, SourceAddress));
            KdPrint(("Size:[0x%x]\n", Size));
            KdPrint(("AllocationType:[0x%x] Protection[0x%x]\n", AllocationType, Protection));
            KdPrint(("ntStatus:[0x%x]\n", ntStatus));
            KdPrint(("============================================================\n"));
        }

        NTSTATUS OpenProcess(_Out_ PHANDLE ProcessHandle)
        {
            CLIENT_ID ClientId = {(HANDLE)this->TargetProcessId, 0};
            OBJECT_ATTRIBUTES ObjectAttributes = {sizeof(OBJECT_ATTRIBUTES)};

            this->ntStatus = ZwOpenProcess(ProcessHandle,
                                           GENERIC_ALL,
                                           &ObjectAttributes,
                                           &ClientId);

            if (!NT_SUCCESS(this->ntStatus))
            {
                KdPrint(("SS::VirtualMemory::OpenProcess() error! this->ntStatus:[%x]\n", this->ntStatus));
                return this->ntStatus;
            }

            return this->ntStatus;
        }

        NTSTATUS Allocate()
        {
            HANDLE hProcess = nullptr;

            this->ntStatus = OpenProcess(&hProcess);
            if (!NT_SUCCESS(this->ntStatus))
            {
                return this->ntStatus;
            }

            this->ntStatus = ZwAllocateVirtualMemory(hProcess,
                                                     (PVOID *)&TargetAddress,
                                                     0,
                                                     (PSIZE_T)&Size,
                                                     AllocationType,
                                                     Protection);

            if (!NT_SUCCESS(this->ntStatus))
            {
                KdPrint(("SS::VirtualMemory::Allocate() error! this->ntStatus:[%x]\n", this->ntStatus));
                return this->ntStatus;
            }

            ZwClose(hProcess);

            return this->ntStatus;
        }

        NTSTATUS Free()
        {
            HANDLE hProcess = nullptr;

            this->ntStatus = OpenProcess(&hProcess);
            if (!NT_SUCCESS(this->ntStatus))
            {
                return this->ntStatus;
            }

            this->ntStatus = ZwFreeVirtualMemory(hProcess,
                                                 (PVOID *)&TargetAddress,
                                                 (PSIZE_T)&Size,
                                                 AllocationType);

            ZwClose(hProcess);
            return this->ntStatus;
        }

        NTSTATUS Copy()
        {
            PEPROCESS pTargetProcess = NULL;
            PEPROCESS pSourceProcess = NULL;
            SIZE_T NumberOfBytesCopied = 0;

            do
            {
                this->ntStatus = PsLookupProcessByProcessId((HANDLE)this->TargetProcessId,
                                                            &pTargetProcess);

                if (!NT_SUCCESS(this->ntStatus))
                {
                    break;
                }

                this->ntStatus = PsLookupProcessByProcessId((HANDLE)this->SourceProcessId,
                                                            &pSourceProcess);
                if (!NT_SUCCESS(this->ntStatus))
                {
                    break;
                }

                this->ntStatus = MmCopyVirtualMemory(pSourceProcess, (PVOID)this->SourceAddress,
                                                     pTargetProcess, (PVOID)this->TargetAddress,
                                                     this->Size, KernelMode, &NumberOfBytesCopied);
            } while (FALSE);

            ObDereferenceObject(pTargetProcess);
            ObDereferenceObject(pSourceProcess);
            return this->ntStatus;
        }
    };
}
