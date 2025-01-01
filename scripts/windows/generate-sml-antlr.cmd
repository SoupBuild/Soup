@echo off
SETLOCAL
SET Flavor=%1
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET CodeDir=%RootDir%\code
SET GrammarDir=%CodeDir%\grammar
SET CSharpDir=%CodeDir%\generate-sharp\utilities\sml\grammar
SET JAVA="C:\Program Files\Microsoft\jdk-11.0.23.9-hotspot\bin\java.exe"
SET ANLTR=%JAVA% -jar %UserProfile%\Desktop\antlr-4.11.1-complete.jar

pushd %GrammarDir%
echo %ANLTR% -Dlanguage=CSharp SML.g4 -o %CSharpDir% -no-listener -visitor
call %ANLTR% -Dlanguage=CSharp SML.g4 -o %CSharpDir% -no-listener -visitor
popd