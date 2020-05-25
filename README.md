# Soup Build

## Quick Links
* [Soup Build](https://www.soupbuild.com/)
* [Getting Started](./Docs/Getting-Started.md)
* [Command Line Interface (CLI)](./Docs/CLI.md)
* [Samples](./Docs/Samples.md)
* [Architecture](./Docs/Architecture.md)

## Overview
Soup is a build system that was created to simplify many aspects of developing C++. Soup was built from the ground up with C++20 Modules as a core principle for inter-project references. By only sharing a single module interface between the individual packages we can ensure that the internals of one project will not "leak" into other downstream dependencies. Soup also uses "just in time" compiled binaries to implement build extensions. These two aspects together allows for easy authoring of a project with custom build steps that can be shared with other teams or organizations (Package Manager!) and will allow for the ability to introduce breaking syntax changes in future versions of C++ while maintaining interface level compatibility (Epochs!).

## Design Goals

### Reproducible
Core to any build system is the requirement that a build be deterministic and reproducible. By integrating the entire build system as extensions of a simple core build authoring engine a Soup build will easily be able to recreate the exact environment that was used to compile past versions of any project. (Note: While Soup holds this requirement for it's own Build Extensions, it will fall on the community to ensure that we keep this goal top of mind when writing custom builds).

### Shareable
Because C++ is a fully compiled language with ABI compatibility issues, sharing libraries has always been painful. By writing the entire build system in an integrated extension mechanism that relies on the ability to compile C++ extensions at build time all packages can be shared through **sou**rce **p**ackages. This will allow for seamless integration with the package manager.

### Simplicity
Soups primary goal is to make building C++ simple. It strives to have the smallest number of steps to get a new project up and running, while having an extensibility framework to support the complexity of large projects with unique requirements.

### Isolation
A common problem with C++ builds today is the leaking of one individual components internals into downstream dependencies through header includes. Soup natively supports binary module interfaces as the default mechanism for sharing public symbols between components. This alleviates the need to match preprocessor definitions between a project and it's dependencies. The isolation also allows for better compartmentalization of components which leads to better architected code with a clean separation between public and internal symbols.

## Contributing
Soup is currently in active prototyping and testing. If you are interested in contributing to the project feel free to submit a PR or file an issue with a suggestion/bug. Otherwise download the latest release to give it a try! We are always open to feedback, good or bad :smile:.
* [Getting Started](./Docs/Getting-Started.md)
* [Samples](./Docs/Samples.md)
