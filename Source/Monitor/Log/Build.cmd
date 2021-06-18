@echo off
SET HostDir=%~dp0
SET DetoursDir=%HostDir%..\Detours
SET OutputDirectorPath=out\MSVC\debug\win32
SET HostOutputDirectory=%HostDir%\%OutputDirectorPath%
SET DetoursOutputDirectory=%DetoursDir%\%OutputDirectorPath%
REM - Build the host
echo soup build %HostDir%
call soup build %HostDir%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
REM - Build Each version of the detours dll
echo soup build %DetoursDir% -architecture x64
call soup build %DetoursDir% -architecture x64
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
echo copy %DetoursOutputDirectory%\x64\bin\Monitor.Client.dll %HostOutputDirectory%\x64\bin\Monitor.Client.64.dll
copy %DetoursOutputDirectory%\x64\bin\Monitor.Client.dll %HostOutputDirectory%\x64\bin\Monitor.Client.64.dll > nul
copy %DetoursOutputDirectory%\x64\bin\Monitor.Client.pdb %HostOutputDirectory%\x64\bin\Monitor.Client.64.pdb > nul
echo soup build %DetoursDir% -architecture x86
call soup build %DetoursDir% -architecture x86
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
echo copy %DetoursOutputDirectory%\x86\bin\Monitor.Client.dll %HostOutputDirectory%\x64\bin\Monitor.Client.32.dll
copy %DetoursOutputDirectory%\x86\bin\Monitor.Client.dll %HostOutputDirectory%\x64\bin\Monitor.Client.32.dll > nul
copy %DetoursOutputDirectory%\x86\bin\Monitor.Client.pdb %HostOutputDirectory%\x64\bin\Monitor.Client.32.pdb > nul