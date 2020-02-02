# Architecture
The internal architecture is still in flux, however the general concepts should remain unchanged.

## Recipe
The Soup Recipe is a property bag defined in the [toml](https://github.com/toml-lang/toml) language. It contains a few special properties that will be used in the core build engine to recursively build all dependencies. The build engine will initialize the active build statue with a table named "Recipe" with all properties loaded from the file to grant access to the build extensions during execution.

## Build Engine
Soup builds are fairly simple at their core. There are three phases.

1) **Parse Recipe** - The Recipe toml file is read from disk.
2) **Build Dependencies** - The build engine recursively builds all transient dependencies.
3) **Build Extensions** - The build engine recursively builds all build extension DLLs. The engine will then invoke the "RegisterBuildExtension" method that is exported from the Extension DLL.
4) **Run Tasks** - The build engine will invoke all registered build tasks in their defined order (Note: this is still being worked on.) The Tasks can influence each other by reading and setting properties on the active state. A build task should not actually perform any commands itself, it will instead generate Build Commands which are self contained operation definitions with input/output files.
5)  **Run Commands** - The final stage of the build is to execute the build commands that were generated from the build tasks. These commands contain the input and output files that will be used to perform incremental builds. (Note: There is currently a very simple time-stamp based incremental build that relies on the compiler to print all included files. There is an open question if this can be replaced with a better system that monitors the filesystem for changes, possibly BuildXL).
