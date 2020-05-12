# Another Build System
You are right to be skeptical. Why do we need yet another competing solution for building our code that will fragment our community even further? I have spent a majority of my career as a software engineer trying to convince others not to reinvent what can instead be can borrowed and extended from others. I think that is why it has taken me so long to put down in words my reasoning for creating this project. However, I truly believe that we are at a unique point in the life span of C++ that we can finally create a build system that resolves all of the major issues holding back C++ from being a best in class experience for everyone, from the programmer writing their very first "Hello World!" to the most weathered of coders.

With C++ 20 coming out this year we will finally be getting our hands on the long awaited (and controversial) Modules support. It is this feature that will allow C++ builds to finally have a clean binary separation between individual projects that will open the door to fixing many of the problems present in C++ today. At the same time, migrating our code to support Modules will require a substantial amount of work, which means this is the ideal time to consider a major shift in what tooling we use as a community. In this post I outline a general overview of the key issues present in building and sharing our code today and then present a new build system that leverages Modules at its core to create a new way to collaborate around the open source community.

## Sharing Code
At its core C++ is quite simple to build. The most basic executable can be created using one source file with a single call to the compiler of your choice in your favorite command line. However, as you pile on a more and more requirements the complexity becomes unmanageable and the need to automate our builds becomes apparent. Beyond the normal dependency graph complexity that will be present in any programming language, C++ has extra issues that make it especially hard to build and even more so to share those builds with others in the community. There are three primary issues that make C++ a hard language to collaborate around; it has multiple compiler implementations, it is a compiled language and it inherited the legacy of the C preprocessor.

### Specification
Unlike many other languages out there today that have both a language specification and a single first party implementation, the C++ language is primarily a specification, and has no first party compiler. This means that we get to have multiple compilers from different vendors that allows for targeting many different architectures. It also means that if I want to share my code with the C++ community as a whole I have to verify that I do not have any platform specific logic and have a unique setup for each compiler to ensure the build works correctly. This is not too difficult of a problem to handle with a good build system, but does require some integration work to support new compiler vendors. This is also perhaps where C++ has made the greatest strides with the continued evolution of the standard library.

### Compiled
The overhead of having many different compiler implementations is compounded by the fact that C++ is compiled directly to the assembly for the target machine that will execute the code. C++ puts no constraints on how a compiler does this mapping which means that the [Application Binary Interface (ABI)](https://en.wikipedia.org/wiki/Application_binary_interface) between two compilers (and sometimes between versions of the same compiler) are not compatible with each other. This means we have to ensure that all of our objects were generated using the same compiler or take special care to work around this issue using strict design practices. There have been multiple approaches in the past to combat binary compatibility issues when sharing C++ code.

Perhaps the oldest way to share native code is to pre-build the binaries and distribute a single dynamic library along with a set of public header files. One way to get around the binary compatibility issue is to only expose a C style public binary layer that takes advantage of the fact that C does have a standardized naming convention. This requires that all C++ implementation code be wrapped in a public C layer and if a client wishes to use modern C++ concepts the C layer can then be wrapped in another C++ layer that is compiled within the consumer project. A second pattern that allows for the distribution of pre-built native binaries is to expose a single C style entry point and from then on use only interfaces when communicating across the boundary (Note: beware of exceptions or standard library objects passing over the boundary!). While not technically a requirement that C++ interface definition have a standard ABI, Microsoft has effectively standardized this approach through the sheer number of projects that utilize it through [COM](https://en.wikipedia.org/wiki/Component_Object_Model). Both of these approaches will produce fully compatible binaries that can be distributed to others, however the overhead of either approach is often not worth the effort unless your shared component is very large (i.e. Boost).

Another approach to binary compatibility is to have no binaries at all. To work around binary compatibility issues some communities of smaller projects have started embedding both the definition and implementation into header only libraries. When including the headers in your project you are effectively building the project for them. Due to the constraint that you must now place all of your source in your public headers these headers can grow unwieldy and will be unmanageable for large projects. These large headers can also have a negative impact on build performance as re-parsed multiple times in every translation unit that consumes them.

A relatively new approach to consuming external dependencies is through package managers. A package manager distributes either the raw source along with the build definition required to integrate with your project and as long as your two systems are compatible it will automatically inject the child dependency into your build or pre-download the pre-built binaries that were carefully cataloged to have the same compiler, architecture and configurations. This approach works well, but does require that the package manager is able to generate the required build definitions to be used by consumers or be directly integrated within a build system.

### Preprocessor
The preprocessor is, until now, a point of failure that could not be protected against by any build system when integrating with external source. Until C++ 20 the only way to share a symbol definition was to place it in a header file and have that header file be included by both the implementation and all of the translation units that wish to use it. This can lead to unforeseen compatibility issues when a header is included with a different set of preprocessor definitions defined from what were present when compiling the implementation. At best this will result in a compiler or linker error, and at worst you will have a fun [one definition rule](https://en.wikipedia.org/wiki/One_Definition_Rule) violation to track down! This is where Modules shine, and the primary driver behind why I believe we can finally make C++ the best open source, collaberitive language!

# The New Build

## Requirements 
It is not enough to say that Modules will solve all of our problems. We will also have to define clear priorities for a collaboration first build system.

### Reproducible builds
### Dead simple setup
### Fully extensible

## Overview

### ideal

Ideally we should be able to resolve a dependency to three items
 1) the public symbol definitions to compile against
 2) the collection of symbol files to link against
 3) the collection of runtime dependencies that are needed at runtime
 
## Epochs
Another major issue with sharing code between different projects is incompatible language standards. In general it is straightforward to pull source that targets an earlier versions of the language into a project with a newer version (i.e unless the `C++ 11` code uses a removed standard library feature it will compile fine with a `C++ 14` compiler). This means that header only libraries must have compile time checks for different language versions and pre-compiled libraries already stay away from the standard library.


