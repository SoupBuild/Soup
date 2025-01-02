# View
## Overview
Launch the Soup View utility GUI for visualizing the build dependency, tasks and operation graphs.
```
soup view <path>
```

`path` - An optional parameter that directly follows the build command. If present this specifies the directory to look for a Recipe file to build. If not present then the command will use the current active directory.

## Examples
Launch the tool in the current directory for debug with default configurations.
```
soup view
```