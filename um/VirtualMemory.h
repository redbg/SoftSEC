#pragma once
#include <Windows.h>
#include "Driver.h"
#include "../shared/shared.h"

namespace SS
{
    struct VirtualMemory : SS::BASE::VirtualMemory
    {
        static DWORD64 Allocate(DWORD64 ProcessId,
                                DWORD64 Size,
                                ULONG AllocationType = MEM_COMMIT | MEM_RESERVE,
                                ULONG Protect = PAGE_EXECUTE_READWRITE)
        {
            SS::VirtualMemory vm = {METHOD::Allocate};

            vm.ProcessId = ProcessId;
            vm.Size = Size;
            vm.AllocationType = AllocationType;
            vm.Protect = Protect;

            DeviceIoControl(SS::Driver::hDevice,
                            IOCTL_SS_VirtualMemory,
                            &vm, sizeof(vm),
                            &vm, sizeof(vm),
                            NULL, NULL);

            return vm.BaseAddress;
        }

        static void Free()
        {
        }
    };
}
