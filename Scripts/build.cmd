@echo off
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET OutputDir=%ScriptsDir%..\out
SET ClientCLIDir=%SourceDir%\Client\CLI
SET DetoursDir=%SourceDir%\Monitor\Detours
SET OutputDirectorPath=MSVC\%Flavor%\win32
SET ClientCLIOutputDirectory=%OutputDir%\Soup\%OutputDirectorPath%
SET DetoursOutputDirectory=%OutputDir%\Monitor.Detours\%OutputDirectorPath%

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
echo copy %DetoursOutputDirectory%\x64\bin\Monitor.Detours.dll %ClientCLIOutputDirectory%\x64\bin\Monitor.Detours.64.dll
copy %DetoursOutputDirectory%\x64\bin\Monitor.Detours.dll %ClientCLIOutputDirectory%\x64\bin\Monitor.Detours.64.dll > nul
echo copy %DetoursOutputDirectory%\x86\bin\Monitor.Detours.dll %ClientCLIOutputDirectory%\x64\bin\Monitor.Detours.32.dll
copy %DetoursOutputDirectory%\x86\bin\Monitor.Detours.dll %ClientCLIOutputDirectory%\x64\bin\Monitor.Detours.32.dll > nul