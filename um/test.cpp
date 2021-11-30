#include <stdio.h>
#include "softsec.h"

int main()
{
    while (!GetAsyncKeyState(VK_END))
    {
        if (GetAsyncKeyState(VK_F1))
        {
            printf("SS::Driver::Load()\n");
            SS::Driver::Load();
        }

        if (GetAsyncKeyState(VK_F2))
        {
            printf("SS::Driver::Unload()\n");
            SS::Driver::Unload();
        }

        if (GetAsyncKeyState(VK_F3))
        {
            DWORD64 pid = 0;
            DWORD64 size = 0;

            printf("SS::VirtualMemory::Allocate(pid, size)\n");
            scanf("%lld,%llx", &pid, &size);

            printf("[0x%p]\n", (void *)SS::VirtualMemory::Allocate(pid, size));
        }

        if (GetAsyncKeyState(VK_F4))
        {
            DWORD64 pid = 0;
            DWORD64 address = 0;

            printf("SS::VirtualMemory::Free(pid, address)\n");
            scanf("%lld,%llx", &pid, &address);

            printf("[%d]\n", SS::VirtualMemory::Free(pid, address));
        }

        if (GetAsyncKeyState(VK_F5))
        {
            DWORD64 pid = 0;
            DWORD64 address = 0;
            DWORD64 data = 0x1122334455667788;

            printf("SS::VirtualMemory::Copy(pid, address)\n");
            scanf("%lld,%llx", &pid, &address);

            SS::VirtualMemory::Copy(pid, address, GetCurrentProcessId(), (DWORD64)&data, sizeof(data));

            printf("[%llx]\n", data);
        }

        Sleep(200);
    }

    return 0;
}
