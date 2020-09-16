@echo off
SET ScriptsDir=%~dp0
SET ClientDir=%ScriptsDir%..\Source\Client\CLI
SET OutDir=%ClientDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %OutDir%\MSVC\release\win32\x64\bin\ %RunDir%\ /MIR
%RunDir%\Soup.exe %*