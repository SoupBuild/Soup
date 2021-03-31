@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %OutDir%\Soup\MSVC\debug\win32\x64\bin\ %RunDir%\ /MIR
robocopy %RootDir%\Source\GenerateSharp\Generate\bin\Debug\net5.0\ %RunDir%\Generate\ /MIR
%RunDir%\Soup.exe %*