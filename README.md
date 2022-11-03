# Soup Build

## Quick Links
* [Official Site](https://www.soupbuild.com/)
* [Getting Started](./Docs/Getting-Started.md)
* [Command Line Interface (CLI)](./Docs/CLI.md)
* [Samples](./Docs/Samples.md)

## Overview
Soup is a build system for developing software as a community, with a primary focus on developer velocity. Soup was built from the ground up to support guaranteed minimum incremental builds while allowing for reusability of both build logic and runtime code.

Soup utilizes a three phased approach to building code. A **declarative** frontend [Recipe](./Docs/Architecture/Recipe.md) tells the build **what** to build. A **generate** layer uses [Extensions](./Docs/Architecture/Build-Extension.md) to define **how** to build. And finally, the **evaluate** engine runs the build [Operations](./Docs/Architecture/Build-Operation.md) to invoke the required build tooling.

The clean isolation of individual projects and the ability to write custom build logic that will be integrated into the shared runtime work together to allow for easy authoring of a package that can be shared with other teams or organizations through a public source based [Package Manager](https://www.soupbuild.com/).

### More Information:
* [Design Requirements and Goals](./Docs/Design-Requirements-Goals.md) - An up to date set of design goals taken from the original design proposal.
* [Architecture](./Docs/Architecture.md) - An up to date breakdown of the Build Runtime and Design.
* [Command Line Interface (CLI)](./Docs/CLI.md) - The help documentation for the Soup Command line application.

## Release Status
Soup is currently in an `Alpha` stage. This means that anyone is welcome to play around with the source code or the releases on GitHub, however there may be breaking changes in the internal structures and definitions as work is completed and therefor it should not be used in any production capacity. The design is stabilizing over time and will enter a [Beta](https://github.com/mwasplund/Soup/milestone/1) when we believe there will be no more breaking changes or security concerns that would block a first release.

## Contributing
Soup is currently in active prototyping and testing. If you are interested in contributing to the project feel free to submit a PR or download the latest release and file an issue with suggestions or bugs. :smile:.
* [Getting Started](./Docs/Getting-Started.md)
* [Samples](./Docs/Samples.md)
* [Developer Setup](./Docs/Developer-Setup.md)
