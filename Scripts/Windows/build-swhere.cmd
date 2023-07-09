@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET SourceDir=%RootDir%\Source
SET SWhereDir=%SourceDir%\GenerateSharp\Swhere

REM - Build SWhere tool
echo dotnet publish %SWhereDir% -c %Flavor% -f net7.0 -r win-x64 --self-contained
call dotnet publish %SWhereDir% -c %Flavor% -f net7.0 -r win-x64 --self-contained
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
