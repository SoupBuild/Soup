# Install
## Overview
Install a package from the public feed.
```
soup install <path> name<@version>
```

`path` - An optional parameter that directly follows the install command. If present this specifies the directory to look for a Recipe file to load. If not present then the command will use the current active directory.

`name<@version>` - A parameter to specify the name and optional version of the package you would like to install.

## Examples
Install the latest `json11` package and register it in the MyProject Recipe file.
```
soup install ./Code/MyProject/ json11
```

Install version `1.1.0` of the `json11` package and register it in the current folder Recipe file.
```
soup install json11@1.1.0
```