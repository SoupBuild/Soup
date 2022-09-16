@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\C++\Soup\0.20.1\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.Generate\Release\net6.0\win-x64\publish\ %RunDir%\Soup\Generate\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\C#\Soup.Cpp\0.3.0\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin\ %RunDir%\Soup\Extensions\Soup.Cpp\0.3.0\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\C#\Soup.CSharp\0.6.0\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin\ %RunDir%\Soup\Extensions\Soup.CSharp\0.6.0\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Release\net6.0\win-x64\publish\ %RunDir%\Soup\PackageManager\ Soup.Build.PackageManager.exe /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Release\net6.0\win-x64\publish\ %RunDir%\ swhere.exe /NJH /NJS /NDL > NUL
%RunDir%\Soup.cmd %*