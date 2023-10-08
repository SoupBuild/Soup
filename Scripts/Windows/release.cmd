@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
SET SourceDir=%RootDir%\Source
SET InstallerDir=%SourceDir%\Installer\SoupInstaller

SET SOUP_VERSION=0.35.3

REM - Build MSI Installer
echo msbuild %InstallerDir% -p:Configuration=Release
call msbuild %InstallerDir% -p:Configuration=Release
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Generate zip release
tar -a -cf %OutDir%\soup-build-%SOUP_VERSION%-windows-x64.zip -C %RunDir% *