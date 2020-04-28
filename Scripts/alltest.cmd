@echo off
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source\

pushd %SourceDir%\Extensions\Compiler\Clang.UnitTests\
call soup build
call soup run
popd

pushd %SourceDir%\Extensions\Compiler\MSVC.UnitTests\
call soup build
call soup run
popd

pushd %SourceDir%\Client\Core.UnitTests\
call soup build
call soup run
popd