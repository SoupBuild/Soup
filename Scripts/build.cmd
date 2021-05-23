@echo off
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET OutputDir=%ScriptsDir%..\out
SET ClientCLIDir=%SourceDir%\Client\CLI
SET DetoursDir=%SourceDir%\Monitor\Detours
if %Flavor% == release (SET OutputX64DirectorPath=fa11c194b8df931b2241afc7196c809fb2cc5692fd7e38a0ae2c176e63fd7ceb) else (SET OutputX64DirectorPath=d6cd8e54c2437463cfea37201849278a10a3fb82df1565fbc685206e25f5)
if %Flavor% == release (SET OutputX86DirectorPath=cd591834cc6c87c42810bd8097848cce03b5635656fa759ec4c2047a439d4b8) else (SET OutputX86DirectorPath=8a7e5abbb0fa75ca7dd3396963ece83dd47228fcce11496ef773c8f56294d)
SET ClientCLIOutputDirectory=%OutputDir%\C++\Soup\%OutputX64DirectorPath%
SET DetoursOutputX64Directory=%OutputDir%\C++\Monitor.Detours\%OutputX64DirectorPath%
SET DetoursOutputX86Directory=%OutputDir%\C++\Monitor.Detours\%OutputX86DirectorPath%

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
echo copy "%DetoursOutputX64Directory%\bin\Monitor.Detours.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Detours.64.dll"
copy "%DetoursOutputX64Directory%\bin\Monitor.Detours.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Detours.64.dll"
echo copy "%DetoursOutputX86Directory%\bin\Monitor.Detours.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Detours.32.dll"
copy "%DetoursOutputX86Directory%\bin\Monitor.Detours.dll" "%ClientCLIOutputDirectory%\bin\Monitor.Detours.32.dll"