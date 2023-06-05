SETLOCAL
SET ScriptsDir=%~dp0
SET RootDir=%ScriptsDir%..\..
SET OutDir=%RootDir%\out

pushd %OutDir%
start msiexec /package soup-build-0.34.0-windows-x64.msi /passive
popd