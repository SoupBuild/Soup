@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET PackageManagerDir=%SourceDir%\GenerateSharp\PackageManager

REM - Build PackageManager
echo dotnet publish %PackageManagerDir% --self-contained -f net6.0-windows10.0.17763.0 -r win-x64 -c %Flavor%
call dotnet publish %PackageManagerDir% --self-contained -f net6.0-windows10.0.17763.0 -r win-x64 -c %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
