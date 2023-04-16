@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

SET SOUP_VERSION=0.33.1

tar -a -cf %OutDir%\soup-build-%SOUP_VERSION%-windows-x64.zip -C %RunDir% *