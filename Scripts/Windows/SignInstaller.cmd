@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out

SET SIGN_COMMAND=signtool sign /n "Open Source Developer, Matthew Asplund" /t http://time.certum.pl /fd sha1 /v

SET SignFiles=%OutDir%\soup-build-0.39.0-windows-x64.msi

%SIGN_COMMAND% %SignFiles%