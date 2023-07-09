@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET SourceDir=%RootDir%\Source
SET PackageManagerDir=%SourceDir%\GenerateSharp\PackageManager

REM - Build PackageManager
echo dotnet publish %PackageManagerDir% -c %Flavor% -f net7.0-windows10.0.17763.0 -r win-x64 --self-contained
call dotnet publish %PackageManagerDir% -c %Flavor% -f net7.0-windows10.0.17763.0 -r win-x64 --self-contained
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
