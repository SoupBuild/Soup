@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run
REM - Use a copy of the final binary in case we are re-buiding itself
robocopy %OutDir%\C++\Soup\3d80717e7d2695f0a7aa5c38e2849624beeaa5714331ca96898fe4b8b8023fe\bin\ %RunDir%\ /MIR /NJH /NJS /NDL > NUL
robocopy %ScriptsDir%\Install\ %RunDir%\ /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.Generate\Debug\net5.0\ %RunDir%\Generate\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\C#\Soup.Cpp\3d80717e7d2695f0a7aa5c38e2849624beeaa5714331ca96898fe4b8b8023fe\bin %RunDir%\Generate\Extensions\Soup.Cpp\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\C#\Soup.CSharp\3d80717e7d2695f0a7aa5c38e2849624beeaa5714331ca96898fe4b8b8023fe\bin %RunDir%\Generate\Extensions\Soup.CSharp\ /MIR /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Soup.Build.PackageManager\Debug\net5-windows10.0.17763.0\ %RunDir%\PackageManager\ /MIR  /NJH /NJS /NDL > NUL
robocopy %OutDir%\msbuild\bin\Swhere\Debug\net5-windows10.0.17763.0\ %RunDir%\Swhere\ /MIR  /NJH /NJS /NDL > NUL
%RunDir%\Soup.exe %*