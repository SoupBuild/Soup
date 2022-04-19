@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

SET CertPath=C:\Users\mwasp\Downloads\soupbuild-shareddeploy-Sectigo-Code-Signing-20220219.pfx
SET Password=

signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\Extensions\Soup.Cpp\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\Generate\Extensions\Soup.CSharp\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\PackageManager\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Soup\PackageManager\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Swhere\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %RunDir%\Swhere\*.exe