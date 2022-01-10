# Restore
## Overview
Restore packages from the public feed.
```
soup restore <path>
```

`path` - An optional parameter that directly follows the restore command. If present this specifies the directory to look for a Recipe file to load. If not present then the command will use the current active directory.

## Examples
Install all of the current defined external dependency packages.
```
soup restore
```

Install all packages in the MyProject Recipe file.
```
soup restore MyProject/
```