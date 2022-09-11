@echo off
SET Flavor=%1
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET ClientCoreSourceDir=%SourceDir%\Client\Core\Source
SET REFLEX=C:\Users\mwasp\Downloads\reflex-3.2.11\reflex\bin\win64\reflex.exe

echo %REFLEX% %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.l -o %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.cpp
call %REFLEX% %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.l -o %ClientCoreSourceDir%\Recipe\LanguageReferenceParser.cpp