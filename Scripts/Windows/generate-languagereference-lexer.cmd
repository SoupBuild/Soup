@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..\
SET ClientCoreSourceDir=Source\Client\Core\Source
SET REFLEX=%UserProfile%\source\repos\RE-flex\bin\win64\reflex.exe

pushd %RootDir%
echo %REFLEX% %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.l -o %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.cpp
call %REFLEX% %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.l -o %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.cpp
popd