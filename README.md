# Soup Build

## Quick Links
* [Official Site](https://www.soupbuild.com/)
* [Getting Started](./Docs/Getting-Started.md)
* [Command Line Interface (CLI)](./Docs/CLI.md)
* [Samples](./Docs/Samples.md)

## Overview
Soup is a cross platform build system for developing software as a community, with a primary focus on developer velocity. Soup was built from the ground up to support guaranteed minimum incremental builds while allowing for reusability of both build logic and runtime code.

The system utilizes a three phased approach to building code. A **declarative** frontend [Recipe](./Docs/Architecture/Recipe.md) tells the build **what** to build. A **generate** layer uses [Extensions](./Docs/Architecture/Build-Extension.md) to define **how** to build. And finally, the **evaluate** engine executes the build [Operations](./Docs/Architecture/Build-Operation.md) to generate the desired output.

The clean isolation individual projects, coupled with the ability to write custom build logic work together to allow for easy authoring of a package that can be shared with other teams or organizations through a public source based [Package Manager](https://www.soupbuild.com/).

The entire system is programming language agnostic and allows for easy integration with any language. This allows for developers to learn one solution and apply that knowledge to any project regardless of the languages it uses.

### More Information:
* [Design Requirements and Goals](./Docs/Design-Requirements-Goals.md) - The core goals and requirements for the system.
* [Architecture](./Docs/Architecture.md) - An up to date breakdown of the system Runtime and Design.
* [Command Line Interface (CLI)](./Docs/CLI.md) - The help documentation for the Soup Command line application.
* [Simple Minimal Language (SML)](./Docs/SML.md) - The documentation for the Declarative language used by Soup Recipes.

## Release Status
Soup is currently in an `Beta` stage. This means that anyone is welcome to play around with the source code or the releases on GitHub, however there may be breaking changes in the internal structures and definitions as work is completed. It is therefor recommended not to be used in any production capacity. The design is stabilizing over time and [Version 1](https://github.com/soup-build/soup/milestone/2) will be released when we believe there will be no more breaking changes or security concerns that would block a first release.

## Contributing
Soup is currently in active prototyping and testing. If you are interested in contributing to the project feel free to submit a PR or download the latest release and file an issue with suggestions or bugs. :smile:.
* [Getting Started](./Docs/Getting-Started.md)
* [Samples](./Docs/Samples.md)
* [Developer Setup](./Docs/Developer-Setup.md)
