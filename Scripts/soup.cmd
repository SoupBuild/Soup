@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\C++\Soup\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.Generate\Release\net6.0\ %RunDir%\Soup\Generate\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\packages\C#\Soup.Cpp\0.1.0\out\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin\ %RunDir%\Soup\Generate\Extensions\Soup.Cpp\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\packages\C#\Soup.CSharp\0.3.1\out\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin %RunDir%\Soup\Generate\Extensions\Soup.CSharp\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Release\net6-windows10.0.17763.0\ %RunDir%\Soup\PackageManager\ /MIR  /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Release\net6-windows10.0.17763.0\ %RunDir%\Swhere\ /MIR  /NJH /NJS /NDL > NUL
%RunDir%\Soup.cmd %*