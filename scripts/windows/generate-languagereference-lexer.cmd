@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..\
SET ClientCoreDir=code\client\core\source
SET REFLEX=%UserProfile%\source\repos\RE-flex\bin\win64\reflex.exe

pushd %RootDir%
echo %REFLEX% %ClientCoreDir%\recipe\LanguageReferenceParser.l -o %ClientCoreDir%\recipe\LanguageReferenceParser.cpp
call %REFLEX% %ClientCoreDir%\recipe\LanguageReferenceParser.l -o %ClientCoreDir%\recipe\LanguageReferenceParser.cpp
popd