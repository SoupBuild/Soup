@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%../..
SET OutDir=%RootDir%/out
SET ReleaseDir=%OutDir%/release
SET RunDir=%OutDir%/run
SET CodeDir=%RootDir%/code
SET InstallerDir=%CodeDir%/installer/soup-installer

SET SOUP_VERSION=0.41.2

REM - Build MSI Installer
echo msbuild %InstallerDir% -p:Configuration=Release
msbuild %InstallerDir% -p:Configuration=Release
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%

REM - Generate zip release
tar -a -cf %ReleaseDir%/soup-build-%SOUP_VERSION%-windows-x64.zip -C %RunDir% *