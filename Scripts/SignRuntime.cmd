@echo off
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..
SET OutDir=%RootDir%\out
SET RunDir=%OutDir%\run

SET CertPath=[SECRET]
SET Password=[SECRET]

signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Soup\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Soup\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Soup\Generate\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Soup\Generate\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Soup\Generate\Extensions\Soup.Cpp\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Soup\Generate\Extensions\Soup.CSharp\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Soup\PackageManager\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Soup\PackageManager\*.exe
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Swhere\*.dll
signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %RunDir%\Swhere\*.exe