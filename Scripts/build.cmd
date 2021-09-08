@echo off
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET OutputDir=%ScriptsDir%..\out
SET ClientCLIDir=%SourceDir%\Client\CLI
SET MonitorClientDir=%SourceDir%\Monitor\Client
if %Flavor% == release (SET OutputX64DirectorPath=1281a639dd5d393781f4188942c6bc05544b7c2c0124eae4cc59236248e36) else (SET OutputX64DirectorPath=3d80717e7d2695f0a7aa5c38e2849624beeaa5714331ca96898fe4b8b8023fe)
if %Flavor% == release (SET OutputX86DirectorPath=a15736f28dbf78415c82858ab76573eb6efdf98c85e3d67b3984210f54747) else (SET OutputX86DirectorPath=9e1e87dea39eeb349e44752a9a37cdf3ed1c2e7aea3044a45ed8bb44609efd)
SET ClientCLIOutputDirectory=%OutputDir%\C++\Soup\%OutputX64DirectorPath%
SET MonitorClientOutputX64Directory=%OutputDir%\C++\Monitor.Client\%OutputX64DirectorPath%
SET MonitorClientOutputX86Directory=%OutputDir%\C++\Monitor.Client\%OutputX86DirectorPath%

REM - Build each version of the monitor client dll
echo soup build %MonitorClientDir% -architecture x64 -flavor %Flavor%
call soup build %MonitorClientDir% -architecture x64 -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
echo soup build %MonitorClientDir% -architecture x86 -flavor %Flavor%
call soup build %MonitorClientDir% -architecture x86 -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Build the host
echo soup build %ClientCLIDir% -flavor %Flavor%
call soup build %ClientCLIDir% -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Copy the monitor client dlls
echo copy "%MonitorClientOutputX64Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.64.dll"
copy "%MonitorClientOutputX64Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.64.dll"
echo copy "%MonitorClientOutputX86Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.32.dll"
copy "%MonitorClientOutputX86Directory%\bin\Monitor.Client.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Client.32.dll"