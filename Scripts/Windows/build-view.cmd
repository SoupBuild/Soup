@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET SourceDir=%RootDir%\Source
SET SWhereDir=%SourceDir%\GenerateSharp\SoupView

REM - Cleanup old publish to work around bug in publish
rmdir /S /Q %RootDir%\out\bin\SoupView\%Flavor%\net8.0\win-x64\publish\

REM - Publish SWhere tool
echo dotnet publish %SWhereDir% -c %Flavor% -f net8.0 -r win-x64 --self-contained true
dotnet publish %SWhereDir% -c %Flavor% -f net8.0 -r win-x64 --self-contained true
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%