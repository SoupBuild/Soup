@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out

%OutDir%\msbuild\bin\Swhere\release\net6.0\win-x64\publish\swhere.exe %*