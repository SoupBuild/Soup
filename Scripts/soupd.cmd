@echo off
SET ScriptsDir=%~dp0
SET OutDir=%ClientDir%..\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %OutDir%\Soup\MSVC\debug\win32\x64\bin\ %RunDir%\ /MIR
%RunDir%\Soup.exe %*