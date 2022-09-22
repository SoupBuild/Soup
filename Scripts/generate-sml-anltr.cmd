@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\
SET GrammarSourceDir=%RootDir%\Source\Grammar
SET CSharpSourceDir=%RootDir%\Source\GenerateSharp\Utilities\SML\Grammar
SET JAVA="C:\Program Files\Microsoft\jdk-11.0.12.7-hotspot\bin\java.exe"
SET ANLTR=%JAVA% -jar C:\Users\mwasp\Desktop\antlr-4.11.1-complete.jar

pushd %GrammarSourceDir%
echo %ANLTR% -Dlanguage=CSharp SML.g4 -o %CSharpSourceDir% -no-listener -visitor
call %ANLTR% -Dlanguage=CSharp SML.g4 -o %CSharpSourceDir% -no-listener -visitor
popd