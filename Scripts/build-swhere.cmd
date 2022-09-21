@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET SWhereDir=%SourceDir%\GenerateSharp\Swhere

REM - Build SWhere tool
echo dotnet publish %SWhereDir% -c %Flavor%
call dotnet publish %SWhereDir% -c %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
