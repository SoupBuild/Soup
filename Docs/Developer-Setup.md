# Developer Setup

## Requirements
* Windows 10
* Build Tools
  * [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) with "Desktop development with c++" workload.
  * OR
  * [Build Tools For Visual Studio 2022](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) with "c++ build tools" workload.
* [Latest Release](https://github.com/SoupBuild/Soup/releases)

### OpenSSL Requirements
* Perl. We recommend Strawberry https://strawberryperl.com/download/5.30.3.1/strawberry-perl-5.30.3.1-64bit.msi
* Netwide Assembler, a.k.a. NASM, available from https://www.nasm.us

## Setup
Clone the repository and all submodules recursively.

```
git clone --recursive https://github.com/mwasplund/Soup.git
```

## Build

### Build openssl
using x64 Native Tools Command Prompt for VS 2022
```
cd Dependencies/openssl
PATH=%PATH%;"C:\NASM"
perl Configure VC-WIN64A
nmake
nmake test
```

### Build the Client
```
cd Source/Client
soup build
```
