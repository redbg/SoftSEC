@echo off

set TargetName=softsec
set TargetArch=x64
set TargetVersion=Win7
set Configuration=Debug

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" %TargetArch%

@REM -------------------- Kernel mode --------------------

@REM Include
set INCLUDE=%INCLUDE%;%WindowsSdkDir%Include\%WindowsSDKVersion%km

@REM Lib
set LIB=%LIB%;%WindowsSdkDir%Lib\%WindowsSDKVersion%km\%TargetArch%

@REM Compiler Options
set CompilerFiles=km\softsec.cpp
set CompilerOptions=/kernel /Gz /W3 /WX /std:c++17
if %TargetArch% == x86 (
    set Preprocessor=/D "_X86_=1" /D "i386=1" /D "STD_CALL"
)
if %TargetArch% == x64 (
    set Preprocessor=/D "_WIN64" /D "_AMD64_" /D "AMD64"
)
if %TargetVersion% == Win7 (
    set Preprocessor=%Preprocessor% /D "_WIN32_WINNT=0x0601" /D "WINVER=0x0601" /D "WINNT=1" /D "NTDDI_VERSION=0x06010000"
)
if %TargetVersion% == Win10 (
    set Preprocessor=%Preprocessor% /D "_WIN32_WINNT=0x0A00" /D "WINVER=0x0A00" /D "WINNT=1" /D "NTDDI_VERSION=0x0A000008"
)
@REM Linker Options
set LinkerOptions=/Driver /ENTRY:"DriverEntry" /SUBSYSTEM:NATIVE,"6.01" /NODEFAULTLIB /OUT:bin\%TargetName%_%TargetArch%.sys
set LinkerLibrary="BufferOverflowFastFailK.lib" "ntoskrnl.lib"

if %Configuration% == Debug (
    set CompilerOptions=%CompilerOptions% /Zi
    set Preprocessor=%Preprocessor% /D "DBG=1"
    set LinkerOptions=%LinkerOptions% /DEBUG
)

@REM Compiler & Linker
cl %CompilerFiles% %CompilerOptions% %Preprocessor% /link %LinkerOptions% %LinkerLibrary%

@REM -------------------- clear --------------------

del *.obj
del *.pdb
