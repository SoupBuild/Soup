@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %OutDir%\C++\Soup\1281a639dd5d393781f4188942c6bc05544b7c2c0124eae4cc59236248e36\bin\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %ScriptsDir%\Install\ %RunDir%\ /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.Generate\Release\net5.0\ %RunDir%\Generate\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Release\net5-windows10.0.17763.0\ %RunDir%\PackageManager\ /MIR  /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Release\net5-windows10.0.17763.0\ %RunDir%\Swhere\ /MIR  /NJH /NJS /NDL > NUL
%RunDir%\Soup.exe %*