@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\Soup\0.30.0\txTMowfPh1V3rPmbvNBmBW9Z8Jg\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.Cpp\0.6.2\txTMowfPh1V3rPmbvNBmBW9Z8Jg\script\ %RunDir%\Soup\Extensions\Soup.Cpp\0.6.2\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.CSharp\0.8.0\txTMowfPh1V3rPmbvNBmBW9Z8Jg\script\ %RunDir%\Soup\Extensions\Soup.CSharp\0.8.0\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.Wren\0.1.2\txTMowfPh1V3rPmbvNBmBW9Z8Jg\script\ %RunDir%\Soup\Extensions\Soup.Wren\0.1.2\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Release\net6.0-windows10.0.17763.0\win-x64\publish\ %RunDir%\Soup\PackageManager\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Release\net6.0\win-x64\publish\ %RunDir%\ swhere.exe /NJH /NJS /NDL > NUL
%RunDir%\Soup.cmd %*