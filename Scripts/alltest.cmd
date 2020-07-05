@echo off
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source

echo %SourceDir%\Build\Execute.UnitTests\
pushd %SourceDir%\Build\Execute.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
call soup build
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
call soup run
if %ERRORLEVEL% NEQ 0 (
    echo Failed
    popd
    exit /B %ERRORLEVEL%
)
popd

echo %SourceDir%\Build\Utilities.UnitTests\
pushd %SourceDir%\Build\Utilities.UnitTests\
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
call soup build
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
call soup run
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
popd

echo %SourceDir%\Extensions\Compiler\Core.UnitTests\
pushd %SourceDir%\Extensions\Compiler\Core.UnitTests\
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
call soup build
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
call soup run
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
popd

echo %SourceDir%\Extensions\Compiler\Clang.UnitTests\
pushd %SourceDir%\Extensions\Compiler\Clang.UnitTests\
if %ERRORLEVEL% NEQ 0 exit /B %ERRORLEVEL%
call soup build
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
call soup run
if %ERRORLEVEL% NEQ 0 (
    echo Failed
    popd
    exit /B %ERRORLEVEL%
)
popd

echo %SourceDir%\Extensions\Compiler\MSVC.UnitTests\
pushd %SourceDir%\Extensions\Compiler\MSVC.UnitTests\
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
call soup build
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
call soup run
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
popd

echo %SourceDir%\Client\Core.UnitTests\
pushd %SourceDir%\Client\Core.UnitTests\
if %ERRORLEVEL% NEQ  0 exit /B %ERRORLEVEL%
call soup build
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
call soup run
if %ERRORLEVEL% NEQ 0 (
    popd
    exit /B %ERRORLEVEL%
)
popd