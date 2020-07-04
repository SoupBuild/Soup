@echo off
SET HostDir=%~dp0
SET DetoursDir=%HostDir%..\Detours
SET BinaryDirectorPath=out\bin\MSVC\debug\win32
SET HostBinaryDirectory=%HostDir%\%BinaryDirectorPath%
SET DetoursBinaryDirectory=%DetoursDir%\%BinaryDirectorPath%
REM - Build the host
echo soup build %HostDir%
call soup build %HostDir%
REM - Build Each version of the detours dll
echo soup build %DetoursDir% -architecture x64
call soup build %DetoursDir% -architecture x64
echo copy %DetoursBinaryDirectory%\x64\Monitor.Detours.dll %HostBinaryDirectory%\x64\Monitor.Detours.64.dll
copy %DetoursBinaryDirectory%\x64\Monitor.Detours.dll %HostBinaryDirectory%\x64\Monitor.Detours.64.dll > nul
echo soup build %DetoursDir% -architecture x86
call soup build %DetoursDir% -architecture x86
echo copy %DetoursBinaryDirectory%\x86\Monitor.Detours.dll %HostBinaryDirectory%\x64\Monitor.Detours.32.dll
copy %DetoursBinaryDirectory%\x86\Monitor.Detours.dll %HostBinaryDirectory%\x64\Monitor.Detours.32.dll > nul