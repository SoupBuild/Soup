@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\Soup\0.31.0\J3mu4cpISw6nDaCPED8gkqZ-q84\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\Dev\Repos\Soup\Source\Tools\Copy\ %RunDir%\Soup\BuiltIn\copy\1.0.0\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\Dev\Repos\Soup\out\Cpp\copy\1.0.0\J3mu4cpISw6nDaCPED8gkqZ-q84\ %RunDir%\Soup\BuiltIn\copy\1.0.0\out\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\Dev\Repos\Soup\Source\Tools\Mkdir\ %RunDir%\Soup\BuiltIn\mkdir\1.0.0\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\Dev\Repos\Soup\out\Cpp\mkdir\1.0.0\J3mu4cpISw6nDaCPED8gkqZ-q84\ %RunDir%\Soup\BuiltIn\mkdir\1.0.0\out\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\.soup\packages\Wren\Soup.Cpp\0.7.0\ %RunDir%\Soup\BuiltIn\Soup.Cpp\0.7.0\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.Cpp\0.7.0\J3mu4cpISw6nDaCPED8gkqZ-q84\ %RunDir%\Soup\BuiltIn\Soup.Cpp\0.7.0\out\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\.soup\packages\Wren\Soup.CSharp\0.9.0\ %RunDir%\Soup\BuiltIn\Soup.CSharp\0.9.0\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.CSharp\0.9.0\J3mu4cpISw6nDaCPED8gkqZ-q84\ %RunDir%\Soup\BuiltIn\Soup.CSharp\0.9.0\out\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\.soup\packages\Wren\Soup.Wren\0.1.2\ %RunDir%\Soup\BuiltIn\Soup.Wren\0.1.2\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.Wren\0.1.2\J3mu4cpISw6nDaCPED8gkqZ-q84\ %RunDir%\Soup\BuiltIn\Soup.Wren\0.1.2\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Debug\net6.0-windows10.0.17763.0\win-x64\publish\ %RunDir%\Soup\PackageManager\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Debug\net6.0\win-x64\publish\ %RunDir%\ swhere.exe /NJH /NJS /NDL > NUL

%RunDir%\Soup.cmd %*