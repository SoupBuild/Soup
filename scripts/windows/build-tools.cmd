@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET SourceDir=%RootDir%\Source
SET ToolsDir=%SourceDir%\Client\Tools

REM - Build Tools
echo soup build %ToolsDir% -flavor %Flavor%
call soup build %ToolsDir% -flavor %Flavor%
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
