@echo off
SET ScriptsDir=%~dp0
SET ClientDir=%ScriptsDir%..\Source\Client\CLI
SET OutDir=%ClientDir%\out
SET BinaryDir=%OutDir%\bin
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
xcopy /s /Y %BinaryDir%\MSVC\debug\win32\x64\* %RunDir%\ > nul
copy %ClientDir%\LocalUserConfig.json %RunDir%\LocalUserConfig.json > nul
%RunDir%\Soup.exe %*