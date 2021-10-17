# Install
## Overview
Install packages from the public feed.
```
soup install <path> [-p name<@version>]
```

`path` - An optional parameter that directly follows the install command. If present this specifies the directory to look for a Recipe file to load. If not present then the command will use the current active directory.

`-p name<@version>` - An optional parameter to specify the name and optional version of the package you would like to install. If not present then the install command will install the packages already specified in the Recipe.

## Examples
Install all of the current defined external dependency packages.
```
soup install
```

Install the latest `json11` package and register it in the MyProject Recipe file.
```
soup install MyProject/ -p json11
```

Install version `1.0.1` of the `json11` package and register it in the current folder Recipe file.
```
soup install -p json11@1.0.1
```