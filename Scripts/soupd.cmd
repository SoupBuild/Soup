@echo off
SET ScriptsDir=%~dp0
SET BinaryDir=%ScriptsDir%..\Source\Client\out\bin
REM - Use a copy of the final binary in case we are re-buiding itself
copy %BinaryDir%\MSVC\Debug\Soup.exe %BinaryDir%\Soup.exe > nul
copy %ClientDir%\LocalUserConfig.json %BinaryDir%\LocalUserConfig.json > nul
%BinaryDir%\Soup.exe %*