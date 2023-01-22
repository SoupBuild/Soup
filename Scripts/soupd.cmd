@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\Soup\0.29.0\J3mu4cpISw6nDaCPED8gkqZ-q84\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.Cpp\0.6.0\J3mu4cpISw6nDaCPED8gkqZ-q84\script\ %RunDir%\Soup\Extensions\Soup.Cpp\0.6.0\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.CSharp\0.8.0\J3mu4cpISw6nDaCPED8gkqZ-q84\script\ %RunDir%\Soup\Extensions\Soup.CSharp\0.8.0\ /MIR /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.Wren\0.1.0\J3mu4cpISw6nDaCPED8gkqZ-q84\script\ %RunDir%\Soup\Extensions\Soup.Wren\0.1.0\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Debug\net6.0-windows10.0.17763.0\win-x64\publish\ %RunDir%\Soup\PackageManager\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Debug\net6.0\win-x64\publish\ %RunDir%\ swhere.exe /NJH /NJS /NDL > NUL
%RunDir%\Soup.cmd %*