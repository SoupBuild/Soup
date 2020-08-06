@echo off
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET ClientCLIDir=%SourceDir%\Client\CLI
SET DetoursDir=%SourceDir%\Monitor\Detours
SET BinaryDirectorPath=out\bin\MSVC\%Flavor%\win32
SET ClientCLIBinaryDirectory=%ClientCLIDir%\%BinaryDirectorPath%
SET DetoursBinaryDirectory=%DetoursDir%\%BinaryDirectorPath%

REM - Build each version of the detours dll
echo soup build %DetoursDir% -architecture x64 -flavor %Flavor%
call soup build %DetoursDir% -architecture x64 -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
echo soup build %DetoursDir% -architecture x86 -flavor %Flavor%
call soup build %DetoursDir% -architecture x86 -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Build the host
echo soup build %ClientCLIDir% -flavor %Flavor%
call soup build %ClientCLIDir% -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Copy the detour dlls
echo copy %DetoursBinaryDirectory%\x64\Monitor.Detours.dll %ClientCLIBinaryDirectory%\x64\Monitor.Detours.64.dll
copy %DetoursBinaryDirectory%\x64\Monitor.Detours.dll %ClientCLIBinaryDirectory%\x64\Monitor.Detours.64.dll > nul
echo copy %DetoursBinaryDirectory%\x86\Monitor.Detours.dll %ClientCLIBinaryDirectory%\x64\Monitor.Detours.32.dll
copy %DetoursBinaryDirectory%\x86\Monitor.Detours.dll %ClientCLIBinaryDirectory%\x64\Monitor.Detours.32.dll > nul