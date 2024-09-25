# Create Release

## Bump Client version
Ensure that the client build number is updated in Recipe.sml, VersionCommand.h and in the Script.cs.

Create a PR with this change.

## Build Windows Release
```
./Scripts/Windows/build.cmd Release
./Scripts/Windows/soup-release.cmd version
./Scripts/Windows/SignRuntime.cmd
./Scripts/Windows/release.cmd
./Scripts/Windows/SignInstaller.cmd
```

## Build Linux Release
```
./Scripts/Linux/build
./Scripts/Linux/soup version
./Scripts/Linux/release
```

## Create a GitHub Release
Tag the new release
* title: "Soup 0.\*.\* (Beta)"
* tag: "v0.\*.\*-beta

Upload artifacts:
* soup-build-[version]-linux-[architecture].tar.gz
* soup-build-[version]-windows-[architecture].zip
* soup-build-[version]-windows-[architecture].msi