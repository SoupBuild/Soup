# C++ DirectX
This is a console application that demonstrates creating a complex graphics windows application.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/DirectX)

## Library/Recipe.sml
The Recipe file that defines the directX sample application. Note the special module partition hierarchy that explicitly sets the references for partition units. The application also copies over the shader files that will be compiled at runtime.
```
Name: "Samples.Cpp.DirectX"
Language: "C++|0.1"
Type: "Windows"
Version: "1.0.0"
Partitions: [
    { Source: "D3D12HelloTriangle.cpp", Imports: [ "DXSample.cpp", "DXSampleHelper.cpp", "Win32Application.cpp" ] }
    { Source: "DXSample.cpp", Imports: [ "DXSampleHelper.cpp" ] }
    { Source: "DXSampleHelper.cpp" }
    { Source: "Win32Application.cpp", Imports: [ "DXSample.cpp", "DXSampleHelper.cpp" ] }
]
Interface: "Interface.cpp"
Source: [
    "Main.cpp"
]
PlatformLibraries: [
    "D3D12.lib"
    "D3DCompiler.lib"
    "DXGI.lib"
]
RuntimeDependencies: [
    "Shaders.hlsl"
]
```

## Content
The remaining content assumes that you are familiar with creating a graphics application and windows. This is not meant as a graphics tutorial and is only present as a sample of a more complex scenario for building within Soup.