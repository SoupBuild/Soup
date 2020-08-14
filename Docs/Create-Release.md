# Create Release

## Bump Client version
Ensure that the client build number is updated in recipe.toml, VersionCommand.h and in the Script.cs.

Create a PR with this change.

## Build Client
Build the release client executable
```
build release
```

## Create the release artifacts
Zip up the contents of the bin folder into Soup.zip

[Soup]\Source\Client\CLI\out\bin\MSVC\release\win32\x64\


## Build the Installer
Open [Soup]\Source\Installer\Installer.sln in Visual Studio 2019

Rebuild solution

## Create a GitHub Release
Tag the new release
* title: "Soup 0.\*.\* (Alpha)"
* tag: "v0.\*.\*-alpha

Upload the Soup.zip and Soup.msi files.