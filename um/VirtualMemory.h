#pragma once
#include <Windows.h>
#include "Driver.h"
#include "../shared/shared.h"

namespace SS
{
    struct VirtualMemory : SS::BASE::VirtualMemory
    {
        static DWORD64 Allocate(DWORD64 TargetProcessId,
                                DWORD64 Size,
                                ULONG AllocationType = MEM_COMMIT | MEM_RESERVE,
                                ULONG Protect = PAGE_EXECUTE_READWRITE)
        {
            SS::VirtualMemory vm = {METHOD::Allocate};

            vm.TargetProcessId = TargetProcessId;
            vm.Size = Size;
            vm.AllocationType = AllocationType;
            vm.Protection = Protect;

            DeviceIoControl(SS::Driver::hDevice,
                            IOCTL_SS_VirtualMemory,
                            &vm, sizeof(vm),
                            &vm, sizeof(vm),
                            NULL, NULL);

            return vm.TargetAddress;
        }

        static BOOL Free(DWORD64 TargetProcessId,
                         DWORD64 TargetAddress,
                         DWORD64 Size = 0,
                         ULONG AllocationType = MEM_RELEASE)
        {
            SS::VirtualMemory vm = {METHOD::Free};

            vm.TargetProcessId = TargetProcessId;
            vm.TargetAddress = TargetAddress;
            vm.Size = Size;
            vm.AllocationType = AllocationType;

            return DeviceIoControl(SS::Driver::hDevice,
                                   IOCTL_SS_VirtualMemory,
                                   &vm, sizeof(vm),
                                   &vm, sizeof(vm),
                                   NULL, NULL);
        }

        static BOOL Copy(DWORD64 TargetProcessId, DWORD64 TargetAddress,
                         DWORD64 SourceProcessId, DWORD64 SourceAddress,
                         DWORD64 Size)
        {
            SS::VirtualMemory vm = {METHOD::Copy};

            vm.TargetProcessId = TargetProcessId;
            vm.TargetAddress = TargetAddress;
            vm.SourceProcessId = SourceProcessId;
            vm.SourceAddress = SourceAddress;
            vm.Size = Size;

            return DeviceIoControl(SS::Driver::hDevice,
                                   IOCTL_SS_VirtualMemory,
                                   &vm, sizeof(vm),
                                   &vm, sizeof(vm),
                                   NULL, NULL);
        }
    };
}
