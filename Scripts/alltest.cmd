@echo off
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source\

pushd %SourceDir%\Compiler\Clang.UnitTests\
call soup build
call soup run
popd

pushd %SourceDir%\Compiler\MSVC.UnitTests\
call soup build
call soup run
popd

pushd %SourceDir%\Core.UnitTests\
call soup build
call soup run
popd