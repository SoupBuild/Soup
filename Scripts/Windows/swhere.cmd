@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out

%OutDir%\msbuild\bin\Swhere\Release\net7.0\win-x64\publish\swhere.exe %*