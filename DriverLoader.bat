@echo off

cd /d %~dp0

set TargetName=softsec
set TargetArch=x64

@REM ed nt!Kd_DEFAULT_Mask 8
@REM DbgSetDebugFilterState(DPFLTR_DEFAULT_ID, DPFLTR_INFO_LEVEL, TRUE);
@REM REG ADD "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Debug Print Filter" /v DEFAULT /t REG_DWORD /d 8 /f

@REM LoaderDriver
sc create %TargetName% binPath= %cd%\%TargetName%_%TargetArch%.sys type= kernel
sc start %TargetName%

pause

@REM UnloadDriver
sc stop %TargetName%
sc delete %TargetName%

pause
