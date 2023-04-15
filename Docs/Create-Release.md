# Create Release

## Bump Client version
Ensure that the client build number is updated in Recipe.sml, VersionCommand.h and in the Script.cs.

Create a PR with this change.

## Build Windows Release
```
./Scripts/Windows/build.cmd Release
./Scripts/Windows/soup.cmd version
./Scripts/Windows/release.cmd
```

Manually build ./Source/Installer/Installer.sln

## Build Linux Release
```
./Scripts/Linux/build
./Scripts/Linux/soup version
./Scripts/Linux/release
```

## Create a GitHub Release
Tag the new release
* title: "Soup 0.\*.\* (Alpha)"
* tag: "v0.\*.\*-alpha

Upload artifacts:
* soup-build-[version]-linux-[architecture].tar.gz
* soup-build-[version]-windows-[architecture].zip
* soup-build-[version]-windows-[architecture].msi