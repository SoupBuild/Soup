@echo off
SET ScriptsDir=%~dp0
SET DepsDir=%ScriptsDir%..\Dependencies
SET SourceDir=%ScriptsDir%..\Source

echo soup build %DepsDir%\Opal\UnitTests\
call soup build %DepsDir%\Opal\UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
echo soup run %DepsDir%\Opal\UnitTests\
call soup run %DepsDir%\Opal\UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%

echo soup build %SourceDir%\Build\Utilities.UnitTests\
call soup build %SourceDir%\Build\Utilities.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
echo soup run %SourceDir%\Build\Utilities.UnitTests\
call soup run %SourceDir%\Build\Utilities.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
