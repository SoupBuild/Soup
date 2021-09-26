@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out

%OutDir%\msbuild\bin\Swhere\Release\net5-windows10.0.17763.0\swhere.exe %*