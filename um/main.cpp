#include <Windows.h>
#include "softsec.h"

int main()
{
    SS::LoadDriver(TEXT("softsec"), TEXT("C:\\Users\\Win10_64\\Desktop\\softsec_x64.sys"));
    system("pause");
    SS::UnloadDriver(TEXT("softsec"));
    return 0;
}
