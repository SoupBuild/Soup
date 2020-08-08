# Developer Setup

## Requirements
* Windows 10
* Build Tools
  * [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/) with "Desktop development with c++" workload.
  * OR
  * [Build Tools For Visual Studio 2019](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019) with "c++ build tools" workload.
* [Latest Release](https://github.com/mwasplund/Soup/releases)

### OpenSSL Requirements
* Perl. We recommend Strawberry http://strawberryperl.com/
* Netwide Assembler, a.k.a. NASM, available from https://www.nasm.us

## Setup
Clone the repository and all submodules recursively.

```
git clone --recursive https://github.com/mwasplund/Soup.git
```

## Build

### Build openssl
using x64 Native Tools Command Prompt for VS 2019
```
cd Dependencies/openssl
PATH=%PATH%;"C:\Program Files\NASM"
perl Configure VC-WIN64A
nmake
nmake test
```

### Build the Client
```
cd Source/Client
soup build
```
