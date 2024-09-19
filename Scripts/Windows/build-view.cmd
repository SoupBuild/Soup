@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET SourceDir=%RootDir%\Source
SET SWhereDir=%SourceDir%\GenerateSharp\SoupView
SET OutDir=%RootDir%\out

REM - Cleanup old publish to work around bug in publish
rmdir /S /Q %OutDir%\msbuild\bin\SoupView\%Flavor%\net8.0\win-x64\publish\

REM - Build SWhere tool
echo dotnet publish %SWhereDir% -c %Flavor% -f net8.0 -r win-x64 --self-contained
call dotnet publish %SWhereDir% -c %Flavor% -f net8.0 -r win-x64 --self-contained
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
