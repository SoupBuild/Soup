# Create Release

## Bump Client version
Ensure that the client build number is updated in Recipe.sml, VersionCommand.h and in the Script.cs.

Create a PR with this change.

## Build Windows Release
```
./scripts/windows/build.cmd Release
./scripts/windows/soup-release.cmd version
./scripts/windows/sign-runtime.cmd
./scripts/windows/release.cmd
./scripts/windows/sign-installer.cmd
```

## Build Linux Release
```cmd
./scripts/linux/build
./scripts/linux/soup version
./scripts/linux/release
```

## Create a GitHub Release
Tag the new release
* title: "Soup 0.\*.\* (Beta)"
* tag: "v0.\*.\*-beta

Upload artifacts:
* soup-build-[version]-linux-[architecture].tar.gz
* soup-build-[version]-windows-[architecture].zip
* soup-build-[version]-windows-[architecture].msi

## Update WinGet
* Fork [winget-pkgs](https://github.com/microsoft/winget-pkgs)
* Install WinGet Create

    ```cmd
    winget install wingetcreate
    ```
* Create an update to the manifest
    
    ```cmd
    wingetcreate update SoupBuild.Soup --urls https://github.com/soup-build/soup/releases/download/v[version]/soup-build-[version]-windows-x64.msi -v [version]
    ```
* Validate manifest

    ```cmd
    winget validate --manifest .\manifests\s\SoupBuild\Soup\[version]\
    winget install --manifest .\manifests\s\SoupBuild\Soup\[version]\
    ```
* Create a PR into the main repository