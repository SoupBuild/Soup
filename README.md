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

### Reproducible
Core to any build system is the requirement that a build must be deterministic and reproducible.

## Contributing
Soup is currently in active prototyping and testing. If you are interested in contributing to the project feel free to submit a PR or file an issue with your suggestion. Otherwise feel free to download the latest release to give it a try. We are always looking for feedback to be the best possibly solution for the largest audience.

## FAQ

### What about a package repository?
Absolutely, package management and a repository of publicly available packages is definitely on the backlog. Because we have limited time we have put this work off for now while ensuring that we can easily add this in the future. The general idea is that any directory reference can be replaced with a name/version pair that will be pulled from a registered feed and built with the same common properties as all local packages.
