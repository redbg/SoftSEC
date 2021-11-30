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

            scanf("%lld,%llx", &pid, &size);

            printf("SS::VirtualMemory::Allocate(%lld, %lld):[0x%p]\n",
                   pid, size, (void *)SS::VirtualMemory::Allocate(pid, size));
        }

        if (GetAsyncKeyState(VK_F4))
        {
            DWORD64 pid = 0;
            DWORD64 address = 0;

            scanf("%lld,%llx", &pid, &address);

            printf("SS::VirtualMemory::Free(%lld, 0x%p):[%d]\n",
                   pid, (void *)address, SS::VirtualMemory::Free(pid, address));
        }

        Sleep(200);
    }

    return 0;
}
