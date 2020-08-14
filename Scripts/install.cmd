SET ScriptsDir=%~dp0
SET SourceDir=%ScriptsDir%..\Source
SET InstallerDir=%SourceDir%\Installer\SoupInstaller\msi
pushd %InstallerDir%
start msiexec /package Soup.msi /passive
popd