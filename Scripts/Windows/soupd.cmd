@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

SET SOUP_VERSION=0.34.0
SET COPY_VERSION=1.0.0
SET MKDIR_VERSION=1.0.0
SET SOUP_CPP_VERSION=0.8.2
SET SOUP_CSHARP_VERSION=0.9.0
SET SOUP_WREN_VERSION=0.2.0

REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\Soup\%SOUP_VERSION%\OZlIVjblazFuKXg-raWUNoGEnG4\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\Dev\Repos\Soup\Source\Tools\Copy\ %RunDir%\Soup\BuiltIn\copy\%COPY_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\Dev\Repos\Soup\out\Cpp\copy\%COPY_VERSION%\OZlIVjblazFuKXg-raWUNoGEnG4\ %RunDir%\Soup\BuiltIn\copy\%COPY_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\Dev\Repos\Soup\Source\Tools\Mkdir\ %RunDir%\Soup\BuiltIn\mkdir\%MKDIR_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\Dev\Repos\Soup\out\Cpp\mkdir\%MKDIR_VERSION%\OZlIVjblazFuKXg-raWUNoGEnG4\ %RunDir%\Soup\BuiltIn\mkdir\%MKDIR_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\.soup\packages\Wren\Soup.Cpp\%SOUP_CPP_VERSION%\ %RunDir%\Soup\BuiltIn\Soup.Cpp\%SOUP_CPP_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.Cpp\%SOUP_CPP_VERSION%\OZlIVjblazFuKXg-raWUNoGEnG4\ %RunDir%\Soup\BuiltIn\Soup.Cpp\%SOUP_CPP_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\.soup\packages\Wren\Soup.CSharp\%SOUP_CSHARP_VERSION%\ %RunDir%\Soup\BuiltIn\Soup.CSharp\%SOUP_CSHARP_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.CSharp\%SOUP_CSHARP_VERSION%\OZlIVjblazFuKXg-raWUNoGEnG4\ %RunDir%\Soup\BuiltIn\Soup.CSharp\%SOUP_CSHARP_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy C:\Users\mwasp\.soup\packages\Wren\Soup.Wren\%SOUP_WREN_VERSION%\ %RunDir%\Soup\BuiltIn\Soup.Wren\%SOUP_WREN_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy C:\Users\mwasp\.soup\out\Wren\Soup.Wren\%SOUP_WREN_VERSION%\OZlIVjblazFuKXg-raWUNoGEnG4\ %RunDir%\Soup\BuiltIn\Soup.Wren\%SOUP_WREN_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Debug\net6.0-windows10.0.17763.0\win-x64\publish\ %RunDir%\Soup\PackageManager\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Debug\net6.0\win-x64\publish\ %RunDir%\ swhere.exe /NJH /NJS /NDL > NUL

%RunDir%\Soup.cmd %*