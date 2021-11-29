#include <stdio.h>
#include "softsec.h"
#include <iostream>

int main()
{
    while (!GetAsyncKeyState(VK_END))
    {
        if (GetAsyncKeyState(VK_F1))
        {
            SS::Driver::Load();
            printf("%x\n", GetLastError());
        }

        if (GetAsyncKeyState(VK_F2))
        {
            SS::Driver::Unload();
            printf("%x\n", GetLastError());
        }

        if (GetAsyncKeyState(VK_F3))
        {
            DWORD64 pid = 0;
            DWORD64 size = 0;

            std::cout << "pid:";
            std::cin >> pid;
            std::cout << "size:";
            std::cin >> size;

            printf("%p\n", (void *)SS::VirtualMemory::Allocate(pid, size));
        }

        Sleep(200);
    }

    return 0;
}
