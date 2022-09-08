@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\C++\Soup\0.19.4\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.Generate\Release\net6.0\win-x64\publish\ %RunDir%\Soup\Generate\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\packages\C#\Soup.Cpp\0.2.2\out\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin\ %RunDir%\Soup\Extensions\Soup.Cpp\0.2.2\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\packages\C#\Soup.CSharp\0.5.2\out\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin %RunDir%\Soup\Extensions\Soup.CSharp\0.5.2\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Release\net6.0\win-x64\publish\ %RunDir%\Soup\PackageManager\ Soup.Build.PackageManager.exe /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Release\net6.0\win-x64\publish\ %RunDir%\ swhere.exe /NJH /NJS /NDL > NUL
%RunDir%\Soup.cmd %*