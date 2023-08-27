# Target
## Overview
Print the unique target directory for a recipe that would be built using the provided configuration parameters.
```
soup target <path> [-flavor <name>]
```

`path` - An optional parameter that directly follows the build command. If present this specifies the directory to look for a Recipe file to build. If not present then the command will use the current active directory.

`-flavor <name>` - An optional parameter to specify the build flavor. Common values include `Debug` or `Release`.

## Examples
Print the target directory for a Recipe in the current directory for debug with default configurations.
```
soup target
```

Print the target directory for a release Recipe in a different directory with the release flavor.
```
soup target ./Code/MyProject/ -flavor release
```
