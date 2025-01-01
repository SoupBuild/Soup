SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET CodeDir=%RootDir%\code
SET InstallerDir=%CodeDir%\installer\soup-installer\msi
pushd %InstallerDir%
start msiexec /uninstall Soup.msi
popd