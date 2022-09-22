@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET PackageManagerDir=%SourceDir%\GenerateSharp\PackageManager

REM - Build PackageManager
echo dotnet publish %PackageManagerDir% -c %Flavor%
call dotnet publish %PackageManagerDir% -c %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
