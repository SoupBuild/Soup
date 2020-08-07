@echo off
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source

echo soup build %SourceDir%\Build\Execute.UnitTests\
call soup build %SourceDir%\Build\Execute.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
echo soup run %SourceDir%\Build\Execute.UnitTests\
call soup run %SourceDir%\Build\Execute.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%

echo soup build %SourceDir%\Build\Utilities.UnitTests\
call soup build %SourceDir%\Build\Utilities.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
echo soup run %SourceDir%\Build\Utilities.UnitTests\
call soup run %SourceDir%\Build\Utilities.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%

echo soup build %SourceDir%\Extensions\Compiler\Core.UnitTests\
call soup build %SourceDir%\Extensions\Compiler\Core.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
echo soup run %SourceDir%\Extensions\Compiler\Core.UnitTests\
call soup run %SourceDir%\Extensions\Compiler\Core.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%

echo soup build %SourceDir%\Extensions\Compiler\Clang.UnitTests\
call soup build %SourceDir%\Extensions\Compiler\Clang.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
echo soup run %SourceDir%\Extensions\Compiler\Clang.UnitTests\
call soup run %SourceDir%\Extensions\Compiler\Clang.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%

echo soup build %SourceDir%\Extensions\Compiler\MSVC.UnitTests\
call soup build %SourceDir%\Extensions\Compiler\MSVC.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
echo soup run %SourceDir%\Extensions\Compiler\MSVC.UnitTests\
call soup run %SourceDir%\Extensions\Compiler\MSVC.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%

echo soup build %SourceDir%\Client\Core.UnitTests\
call soup build %SourceDir%\Client\Core.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
echo soup run %SourceDir%\Client\Core.UnitTests\
call soup run %SourceDir%\Client\Core.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
