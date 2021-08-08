@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %OutDir%\C++\Soup\fa11c194b8df931b2241afc7196c809fb2cc5692fd7e38a0ae2c176e63fd7ceb\bin\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %RootDir%\Source\GenerateSharp\Generate\bin\Release\net5.0\publish\ %RunDir%\Generate\ /MIR /NJH /NJS /NDL > NUL
%RunDir%\Soup.exe %*