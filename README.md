# Soup

## Overview
Soup is a build authoring system that aims to be the core framework for the open source community to build and share components.

## Design Goals

### Simplicity
Building C++ should be as simple as running build. We strive to have the least steps to get a simple project up and running, while at the same time having the extensibility to support the complexity of large projects.

### Isolation
A common problem with C++ builds today is the leaking of one individual components internals into downstream dependencies. We natively support binary module interface as the default consumption mechanism for sharing public symbols with downstream dependencies. This alleviates the need to setup fragile builds that rely on exact preprocessor definitions and exposed macro issues through header includes.

### Shareable
Because C++ is a fully compiled language (not interpreted nor Just In-Time Compiled) sharing libraries has always been painful. By writing the entire build system in an integrated extension mechanism all packages can be shared through a simple source drop (similar to how JavaScript is shared through NPM).

## Contributing
Soup is currently in active prototyping and testing. If you are interested in contributing to the project feel free to submit a PR or file an issue with your suggestion. Otherwise feel free to download the latest release to give it a try. We are always looking for feedback to be the best possibly solution for the largest audience.
