@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..\
SET ClientCoreSourceDir=Source\Client\Core\Source
SET REFLEX=C:\Users\mwasp\source\repos\RE-flex\bin\win64\reflex.exe

pushd %RootDir%
echo %REFLEX% %ClientCoreSourceDir%\SML\SMLParser.l -o %ClientCoreSourceDir%\SML\SMLParser.cpp
call %REFLEX% %ClientCoreSourceDir%\SML\SMLParser.l -o %ClientCoreSourceDir%\SML\SMLParser.cpp
popd