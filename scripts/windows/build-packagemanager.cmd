@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET CodeDir=%RootDir%\code
SET PackageManagerDir=%CodeDir%\generate-sharp\package-manager

REM - Build PackageManager
echo dotnet publish %PackageManagerDir% -c %Flavor% -f net9.0 -r win-x64 --self-contained
call dotnet publish %PackageManagerDir% -c %Flavor% -f net9.0 -r win-x64 --self-contained
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
