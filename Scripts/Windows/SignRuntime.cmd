@echo off
SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

SET CertPath=%1

signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Extensions\Soup.Cpp\0.4.3\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Extensions\Soup.CSharp\0.7.4\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\PackageManager\*.exe