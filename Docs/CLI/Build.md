# Build
## Overview
Build a recipe and all transitive dependencies.
```
soup build <directory> [-flavor <name>|-force]
```

`directory` - An optional parameter that directly follows the build command. If present this specifies the directory to look for a recipe file to build. If not present then the build command will use the current active directory.

`-flavor <name>` - An optional parameter to specify the build flavor. Common values include `debug` or `release`. If not present the build will default to `release`.

`-force` - An optional parameter that forces the build to ignore incremental state and rebuild the world.

## Examples
Build a Recipe in the current directory for release.
```
soup build
```

Build a debug Recipe in a different directory.
```
soup build C:\Code\MyProject\ -flavor debug
```
