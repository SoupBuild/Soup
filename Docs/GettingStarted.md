# Getting Started
Note: The end goal is to have Soup running on all platforms building for any target, however there is a heavy dependency on C++20 Modules which has had the best luck on the latest MSVC compiler so we currently only support windows development.

Note2: While clang will work, there is still work to auto-detect an installation so MSVC is the suggested target compiler.

## Requirements
* [Visual Studio 2019 ](https://visualstudio.microsoft.com/downloads/) with "Desktop development with c++" workload
* [Latest Release](https://github.com/mwasplund/Soup/releases)

## Setup
Place the Soup release somewhere safe and add it to your path in a command prompt.

```
set PATH=%PATH%;C:\Soup\bin\
```
Or add to Path System Environment Variable to always have it available!

## Create First Project

Run Initialize command to create a new project!
```
mkdir MyProject
cd MyProject
soup init
```

## Build First Project

```
soup build
```

## Run First Project

```
soup run
```