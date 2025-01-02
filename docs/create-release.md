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

### Update WinGet
* Fork [winget-pkgs](https://github.com/microsoft/winget-pkgs)
* Install WinGet Create

    ```winget install wingetcreate```
* Create an update to the manifest
    
    ```wingetcreate update SoupBuild.Soup --urls https://github.com/soup-build/soup/releases/download/v[VERSION]/soup-build-[VERSION]-windows-x64.msi -v [VERSION]```
* Validate manifest

    ```
    winget validate --manifest .\manifests\s\SoupBuild\Soup\[VERSION]\
    winget install --manifest .\manifests\s\SoupBuild\Soup\[VERSION]\
    ```

* Create a PR into the main repository

## Build Linux Release
```
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