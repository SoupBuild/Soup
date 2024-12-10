@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out
SET MSBuildDir=%OutDir%\msbuild

%MSBuildDir%\bin\Swhere\Release\net8.0\win-x64\publish\swhere.exe %*