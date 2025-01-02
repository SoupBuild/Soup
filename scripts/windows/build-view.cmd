@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET CodeDir=%RootDir%\code
SET SWhereDir=%CodeDir%\generate-sharp\soup-view

REM - Cleanup old publish to work around bug in publish
rmdir /S /Q %RootDir%\out\msbuild\bin\SoupView\%Flavor%\net9.0\win-x64\publish\

REM - Publish SWhere tool
echo dotnet publish %SWhereDir% -c %Flavor% -f net9.0 -r win-x64
dotnet publish %SWhereDir% -c %Flavor% -f net9.0 -r win-x64
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%