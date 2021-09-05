@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %OutDir%\C++\Soup\d6cd8e54c2437463cfea37201849278a10a3fb82df1565fbc685206e25f5\bin\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %RootDir%\Source\GenerateSharp\Generate\bin\Debug\net5.0\ %RunDir%\Generate\ /MIR  /NJH /NJS /NDL > NUL
robocopy %RootDir%\Source\GenerateSharp\PackageManager\bin\Debug\net5-windows10.0.17763.0\ %RunDir%\PackageManager\ /MIR  /NJH /NJS /NDL > NUL
%RunDir%\Soup.exe %*