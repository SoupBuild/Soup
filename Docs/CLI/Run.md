# Run
## Overview
Run a recipe that was previously built using the provided configuration parameters.
```
soup run <path> [-flavor <name>] [-args <arg1> <arg2> etc.]
```

`path` - An optional parameter that directly follows the build command. If present this specifies the directory to look for a Recipe file to build. If not present then the command will use the current active directory.

`-flavor <name>` - An optional parameter to specify the build flavor. Common values include `Debug` or `Release`.

`-args` - An optional parameter signals that all of the following arguments are meant to be passed into the child executable.

## Examples
Run a Recipe in the current directory for debug with default configurations.
```
soup run
```

Run a release Recipe in a different directory and pass in a single parameter to the child executable.
```
soup run ./Code/MyProject/ -flavor release -args MyParameter
```
