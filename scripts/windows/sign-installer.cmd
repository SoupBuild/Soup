@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out

SET SOUP_VERSION=0.41.3

SET SIGN_COMMAND=signtool sign /n "Open Source Developer, Matthew Asplund" /t http://time.certum.pl /fd sha1 /v

SET SignFiles=%OutDir%\release\soup-build-%SOUP_VERSION%-windows-x64.msi

%SIGN_COMMAND% %SignFiles%