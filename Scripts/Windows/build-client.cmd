@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET SourceDir=%RootDir%\Source
SET OutputDir=%RootDir%\out
SET ClientCLIDir=%SourceDir%\Client\CLI
SET MonitorClientDir=%SourceDir%\Monitor\Client

REM - Build each version of the monitor client dll
echo soup build %MonitorClientDir% -architecture x64 -flavor %Flavor%
call soup build %MonitorClientDir% -architecture x64 -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
echo soup build %MonitorClientDir% -architecture x86 -flavor %Flavor%
call soup build %MonitorClientDir% -architecture x86 -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Build the client
echo soup build %ClientCLIDir% -flavor %Flavor%
call soup build %ClientCLIDir% -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Get the targets
for /f %%i in ('soup target %ClientCLIDir% -flavor %Flavor%') do set ClientCLIOutputDirectory=%%i
for /f %%i in ('soup target %MonitorClientDir% -architecture x64 -flavor %Flavor%') do set MonitorClientOutputX64Directory=%%i
for /f %%i in ('soup target %MonitorClientDir% -architecture x86 -flavor %Flavor%') do set MonitorClientOutputX86Directory=%%i

REM - Copy the monitor client dlls
echo copy "%MonitorClientOutputX64Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.64.dll"
copy "%MonitorClientOutputX64Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.64.dll"
echo copy "%MonitorClientOutputX86Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.32.dll"
copy "%MonitorClientOutputX86Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.32.dll"