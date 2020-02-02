# Command Line Interface (CLI)
Soup is at its core a command line application. The CLI is designed to be as clean and simple as possible while still allowing for easy execution of common build related tasks.

```
soup <command> [arguments]
```

## Build
### Overview
Build a recipe and all transient dependencies.
```
soup build <directory> [-flavor <name>|-force]
```

`directory` - An optional parameter that directly follows the build command. If present this specifies the directory to look for a recipe file to build. If not present then the build command will use the current active directory.

`-flavor <name>` - An optional parameter to specify the build flavor. Common values include `debug` or `release`. If not present the build will default to `release`.

`-force` - An optional parameter that forces the build to ignore incremental state and rebuild the world.

### Examples
Build a Recipe in the current directory for release.
```
soup build
```

Build a debug Recipe in a different directory.
```
soup build C:\Code\MyProject\ -flavor debug
```

## Init
### Overview
Initialize a new console application project in the current active directory.
```
soup init
```

### Examples
The one and only way to use this command.
```
soup init
```

## Version
### Overview
Print the version of the Soup executable.
```
soup version
```

### Examples
The one and only way to use this command.
```
soup version
```