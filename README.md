# Soup Build

## Quick Links
* [Soup Build Website](https://www.soupbuild.com/)
* [Getting Started](./Docs/Getting-Started.md)
* [Command Line Interface (CLI)](./Docs/CLI.md)
* [Samples](./Docs/Samples.md)
* [Architecture](./Docs/Architecture.md)
* [C++ Modules Posts of Interest](./Docs/Posts-Of-Interest.md)

## Overview
Soup is a build system that was created with the primary goal of simplifying the development and sharing of C++ code. Soup was built from the ground up with C++20 Modules as a core principal to support inter-project references. By sharing a single module interface between the individual packages the build system can ensure that the internals of one project will not "leak" into and out of external dependencies.

Soup utilizes a combination of a declarative frontend Recipe along with the ability to write native C++ Extensions to inject custom logic into the build runtime. Most developers will work entirely within the declarative Recipe that allows for a simple to understand and author build definition, however the full power of C++ is available to the build system engineers that need it.

The clean isolation of individual projects and the ability to write custom build logic that will be integrated into the shared runtime work together to allow for easy authoring of a package that can be shared with other teams or organizations through a public source based [Package Manager](https://www.soupbuild.com/).

More Information:
* [Design Proposal](./Docs/Proposal.md) - An in depth writeup of the original justification for the project.
* [Architecture](./Docs/Architecture.md) - An up to date breakdown of the Build Runtime and Design.
* [Design Requirements and Goals](./Docs/Design-Requirements-Goals.md) - An up to date set of design goals taken from the original design proposal.
* [Command Line Interface (CLI)](./Docs/CLI.md) - The help documentation for the Soup Command line application.

## Release Status
Soup is currently in an `Alpha` stage. This means that anyone is welcome to play around with the source code or the releases on GitHub, however there may be breaking changes in the internal structures and definitions as work is completed and therefor it should not be used in any production capacity. The design is stabilizing over time and will enter a [Beta](https://github.com/mwasplund/Soup/milestone/1) when we believe there will be no more breaking changes or security concerns that would block a first release.

## Contributing
Soup is currently in active prototyping and testing. If you are interested in contributing to the project feel free to submit a PR or download the latest release and file an issue with suggestions or bugs. :smile:.
* [Getting Started](./Docs/Getting-Started.md)
* [Samples](./Docs/Samples.md)
* [Developer Setup](./Docs/Developer-Setup.md)
* [Create Release](./Docs/Create-Release.md)
