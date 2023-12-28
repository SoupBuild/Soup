@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
SET SourceDir=%RootDir%\Source
SET GlobalPackagesDir=%UserProfile%\.soup\packages
SET GlobalOutDir=%UserProfile%\.soup\out

SET ConfigHash=Oltq7cGwk0Rbgy1I-3mCMDDE5yM

SET PKG_OWNER=mwasplund

SET SOUP_VERSION=0.37.0
SET COPY_VERSION=1.0.0
SET MKDIR_VERSION=1.0.0
SET SOUP_WREN_VERSION=0.4.1

REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %ScriptsDir%\Install\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\Local\Soup\%SOUP_VERSION%\%ConfigHash%\bin\ %RunDir%\Soup\ /MIR /NJH /NJS /NDL > NUL

robocopy %SourceDir%\Tools\Copy\ %RunDir%\Soup\BuiltIn\%PKG_OWNER%\copy\%COPY_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\Local\copy\%COPY_VERSION%\%ConfigHash%\ %RunDir%\Soup\BuiltIn\%PKG_OWNER%\copy\%COPY_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %SourceDir%\Tools\Mkdir\ %RunDir%\Soup\BuiltIn\%PKG_OWNER%\mkdir\%MKDIR_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy %OutDir%\Cpp\Local\mkdir\%MKDIR_VERSION%\%ConfigHash%\ %RunDir%\Soup\BuiltIn\%PKG_OWNER%\mkdir\%MKDIR_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %GlobalPackagesDir%\Wren\%PKG_OWNER%\Soup.Wren\%SOUP_WREN_VERSION%\ %RunDir%\Soup\BuiltIn\%PKG_OWNER%\Soup.Wren\%SOUP_WREN_VERSION%\ Recipe.sml /NJH /NJS /NDL > NUL
robocopy %GlobalOutDir%\Wren\%PKG_OWNER%\Soup.Wren\%SOUP_WREN_VERSION%\%ConfigHash%\ %RunDir%\Soup\BuiltIn\%PKG_OWNER%\Soup.Wren\%SOUP_WREN_VERSION%\out\ /MIR /NJH /NJS /NDL > NUL

robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Release\net8.0\win-x64\publish\ %RunDir%\Soup\PackageManager\ /MIR /NJH /NJS /NDL > NUL

robocopy %OutDir%\msbuild\bin\SoupView\Release\net8.0\win-x64\publish\ %RunDir%\Soup\View\ /MIR /NJH /NJS /NDL > NUL

robocopy %OutDir%\msbuild\bin\Swhere\Release\net8.0\win-x64\publish\ %RunDir%\ swhere.exe /NJH /NJS /NDL > NUL

%RunDir%\Soup.cmd %*