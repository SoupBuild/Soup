@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

SET CertPath=%1

signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\Extensions\Soup.Cpp\0.2.2\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\Extensions\Soup.CSharp\0.5.1\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\PackageManager\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\PackageManager\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Swhere\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Swhere\*.exe