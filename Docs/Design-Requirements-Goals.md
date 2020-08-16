
## Requirements
The following set of requirements cannot be compromised. The order does not indicate a priority; but, the final system would be deemed a failure if we are unable to fulfill any one of them.

1) **Reproducible** - Core to any build system is the requirement that builds be **deterministic** and **reproducible**. No matter how well a system is designed and implemented, teams will not be able to utilize it unless they can trust that it will always produce the same result independent of who builds it, where they build it and when.

2) **Extensible** - A build system should be able to support the requirements of **all** projects. It should strive to work "out of the box" for a majority of scenarios, but must have an extensibility framework that allows build architects to write their own custom build logic when the built in functionality does not meet their needs.

3) **Isolation** - This is a uniquely important requirement for C++. This is a direct result of the issues present in the language issues outlined above. Isolated builds requires that one project cannot influence or be influenced by another build, intentionally or by accident, except through explicit structured channels.

## Goals
While the goals are not hard requirements, they are always kept at the forefront when making any design or implementation decision. These items are in priority order:

1) **Collaborative** - Writing code is very rarely done in isolation. The largest goal for this build system is to be able to work seamlessly within a team and with external dependencies.

2) **Simple** - When fulfilling the above requirements the secondary priority is always simplicity and usability. This means that the standard user will get the best experience possibly for both setup and usage. Some extra complexity is allowed in exchange for performance gains in the internal implementation and the extensibility framework.

3) **Fast** - The inner developer loop is very important to the productivity of an engineer. To this end, the build system should focus heavily on the performance of an incremental build and, to a lesser extent, ensure the full build is as fast as possible.

4) **Secure** - By its nature, an extensible framework opens itself up to security concerns when executing arbitrary code written by external teams. While this is the same concern present when consuming any open source project, and the community should take care to use only trusted sources for the projects their use, the build system will limit the functionality available to the build runtime to prevent access to the developers machine when not required.

5) **Customizable** - How a project is built is often a matter of personal preference (or legacy requirement). Where allowable, the build system should be customizable to allow for overriding default settings so it does not conflict with the ability to easily build single projects as a part of the greater ecosystem.