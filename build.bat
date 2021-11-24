@echo off

set TargetName=softsec
set TargetArch=x64

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" %TargetArch%

@REM -------------------- Kernel mode --------------------

@REM Include
set INCLUDE=%INCLUDE%;%WindowsSdkDir%Include\%WindowsSDKVersion%km

@REM Lib
set LIB=%LIB%;%WindowsSdkDir%Lib\%WindowsSDKVersion%km\%TargetArch%

@REM Compiler Options
set CompilerFiles=km\DriverEntry.cpp
set CompilerOptions=/kernel /Gz /W3 /WX /Zi /std:c++17
set Preprocessor=/D "_WIN32_WINNT=0x0A00" /D "WINVER=0x0A00" /D "WINNT=1" /D "NTDDI_VERSION=0x0A000008" /D "DBG=1"
if %TargetArch% == x86 (
    set Preprocessor=%Preprocessor% /D "_X86_=1" /D "i386=1" /D "STD_CALL"
)
if %TargetArch% == x64 (
    set Preprocessor=%Preprocessor% /D "_WIN64" /D "_AMD64_" /D "AMD64"
)
@REM Linker Options
set LinkerOptions=/Driver /ENTRY:"DriverEntry" /SUBSYSTEM:NATIVE /NODEFAULTLIB /DEBUG /OUT:%TargetName%_%TargetArch%.sys
set LinkerLibrary="ntoskrnl.lib"

@REM Compiler & Linker
cl %CompilerFiles% %CompilerOptions% %Preprocessor% /link %LinkerOptions% %LinkerLibrary%

@REM -------------------- User mode --------------------

cl um\main.cpp

@REM -------------------- clear --------------------

del *.obj
