# Developer Setup

## Requirements

### Windows
* Windows 11
* Build Tools
  * [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) with "Desktop development with c++" workload.
  * OR
  * [Build Tools For Visual Studio 2022](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) with "c++ build tools" workload.
* [Latest Release](https://github.com/SoupBuild/Soup/releases)

### Linux
* Verified on Ubuntu 22.04.1 LTS WSL
* Install GCC 12
  ```
  sudo apt install g++-12
  ```
* Install .Net SDK 6.0
  ```
  sudo apt install dotnet-sdk-6.0
  ```
* [Latest Release](https://github.com/SoupBuild/Soup/releases)

## Setup
Clone the repository and all submodules recursively.

```
git clone --recursive https://github.com/SoupBuild/Soup.git
```

## Build

### Build the Client
```
cd code/client/cli
soup restore
soup build
```
