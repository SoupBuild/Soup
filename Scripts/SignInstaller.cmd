@echo off
SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET InstallerDir=%SourceDir%\Installer\SoupInstaller\msi

SET CertPath=C:\Users\mwasp\Downloads\soupbuild-shareddeploy-Sectigo-Code-Signing-20220219.pfx
SET Password=

signtool sign /tr http://timestamp.sectigo.com /td sha256 /fd sha256 /f %CertPath%  %InstallerDir%\*.msi