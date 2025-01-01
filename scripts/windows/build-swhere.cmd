@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET CodeDir=%RootDir%\code
SET SWhereDir=%CodeDir%\generate-sharp\swhere

REM - Build SWhere tool
echo dotnet publish %SWhereDir% -c %Flavor% -f net9.0 -r win-x64 --self-contained
call dotnet publish %SWhereDir% -c %Flavor% -f net9.0 -r win-x64 --self-contained
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
