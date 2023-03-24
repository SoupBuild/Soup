@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\..\Source
SET GenerateDir=%SourceDir%\GenerateSharp\Generate

REM - Build Generate
echo dotnet publish %GenerateDir% -c %Flavor%
call dotnet publish %GenerateDir% -c %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%