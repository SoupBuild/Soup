@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
SET SourceDir=%RootDir%\Source
SET GlobalPackagesDir=%UserProfile%\.soup\packages
SET GlobalOutDir=%UserProfile%\.soup\out

SET ConfigHash=OZlIVjblazFuKXg-raWUNoGEnG4

SET SOUP_VERSION=0.35.4
SET COPY_VERSION=1.0.0
SET MKDIR_VERSION=1.0.0
SET SOUP_CPP_VERSION=0.8.2
SET SOUP_CSHARP_VERSION=0.9.0
SET SOUP_WREN_VERSION=0.2.0

REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\Soup\%SOUP_VERSION%\%ConfigHash%\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL

robocopy %SourceDir%\Tools\Copy\ %RunDir%\Soup\BuiltIn\copy\%COPY_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\copy\%COPY_VERSION%\%ConfigHash%\ %RunDir%\Soup\BuiltIn\copy\%COPY_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %SourceDir%\Tools\Mkdir\ %RunDir%\Soup\BuiltIn\mkdir\%MKDIR_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\mkdir\%MKDIR_VERSION%\%ConfigHash%\ %RunDir%\Soup\BuiltIn\mkdir\%MKDIR_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %GlobalPackagesDir%\Wren\Soup.Cpp\%SOUP_CPP_VERSION%\ %RunDir%\Soup\BuiltIn\Soup.Cpp\%SOUP_CPP_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy %GlobalOutDir%\Wren\Soup.Cpp\%SOUP_CPP_VERSION%\%ConfigHash%\ %RunDir%\Soup\BuiltIn\Soup.Cpp\%SOUP_CPP_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %GlobalPackagesDir%\Wren\Soup.CSharp\%SOUP_CSHARP_VERSION%\ %RunDir%\Soup\BuiltIn\Soup.CSharp\%SOUP_CSHARP_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy %GlobalOutDir%\Wren\Soup.CSharp\%SOUP_CSHARP_VERSION%\%ConfigHash%\ %RunDir%\Soup\BuiltIn\Soup.CSharp\%SOUP_CSHARP_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %GlobalPackagesDir%\Wren\Soup.Wren\%SOUP_WREN_VERSION%\ %RunDir%\Soup\BuiltIn\Soup.Wren\%SOUP_WREN_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy %GlobalOutDir%\Wren\Soup.Wren\%SOUP_WREN_VERSION%\%ConfigHash%\ %RunDir%\Soup\BuiltIn\Soup.Wren\%SOUP_WREN_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Debug\net8.0\win-x64\publish\ %RunDir%\Soup\PackageManager\ /MIR /NJH /NJS /NDL > NUL

robocopy %OutDir%\msbuild\bin\SoupView\Debug\net8.0\win-x64\publish\ %RunDir%\Soup\View\ /MIR /NJH /NJS /NDL > NUL

robocopy %OutDir%\msbuild\bin\Swhere\Debug\net8.0\win-x64\publish\ %RunDir%\ swhere.exe /NJH /NJS /NDL > NUL

%RunDir%\Soup.cmd %*