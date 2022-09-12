@echo off
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\
SET ClientCoreSourceDir=Source\Client\Core\Source
SET REFLEX=C:\Users\mwasp\Downloads\reflex-3.2.11\reflex\bin\win64\reflex.exe

pushd %RootDir%
echo %REFLEX% %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.l -o %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.cpp
call %REFLEX% %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.l -o %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.cpp
popd