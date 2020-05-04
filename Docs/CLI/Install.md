# Install
## Overview
Install packages from the public feed.
```
soup install [<name>[@<version>]]
```

`<name>[@<version>]` - An optional parameter to specify the name and optional version of the package you would like to install. If not present then the install command will install all of the packages already in the recipe.

## Examples
Install all of the current external dependencies and build extension packages.
```
soup install
```

Install the latest `json11` package.
```
soup install json11
```

Install version `1.0.0` of the `json11` package.
```
soup install json11@1.0.0
```