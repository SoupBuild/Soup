@echo off
SET ScriptsDir=%~dp0
SET ClientDir=%ScriptsDir%..\Source\Client\CLI
SET OutDir=%ClientDir%\out
SET BinaryDir=%OutDir%\bin
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %BinaryDir%\MSVC\debug\win32\x64\ %RunDir%\ /MIR
%RunDir%\Soup.exe %*