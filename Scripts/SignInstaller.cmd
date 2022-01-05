@echo off
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET InstallerDir=%SourceDir%\Installer\SoupInstaller\msi

SET CertPath=[SECRET]
SET Password=[SECRET]

signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath% /p %Password% %InstallerDir%\*.msi