@echo off
if not defined CONFIG (
    set CONFIG=Release
)
SET ScriptsDir=%~dp0
dotnet %ScriptsDir%..\Source\Client\bin\%CONFIG%\netcoreapp2.0\Soup.dll %*