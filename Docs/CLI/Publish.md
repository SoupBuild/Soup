# Publish
## Overview
Pack the current recipe folder and publish its contents to the official feed.
```
soup publish <path>
```

`path` - An optional parameter that directly follows the publish command. If present this specifies the directory to look for a Recipe file to load. If not present then the command will use the current active directory.

## Examples
Publish the latest version of the package in the current folder.
```
soup publish
```

Publish the latest version in the MyProject Recipe file.
```
soup publish ./Code/MyProject/
```