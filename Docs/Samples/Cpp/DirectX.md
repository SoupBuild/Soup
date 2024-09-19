# C++ DirectX
This is a desktop application that demonstrates creating a complex graphics windows application.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/DirectX)

## Recipe.sml
The Recipe file that defines the directX sample application. Note the special module partition hierarchy that explicitly sets the references for partition units. The application also copies over the shader files that will be compiled at runtime.
```sml
Name: 'Samples.Cpp.DirectX'
Language: (C++@0)
Type: 'Windows'
Version: 1.0.0
Partitions: [
  { Source: 'D3D12HelloTriangle.cpp', Imports: [ 'DXSample.cpp', 'DXSampleHelper.cpp', 'Win32Application.cpp' ] }
  { Source: 'DXSample.cpp', Imports: [ 'DXSampleHelper.cpp' ] }
  { Source: 'DXSampleHelper.cpp' }
  { Source: 'Win32Application.cpp', Imports: [ 'DXSample.cpp', 'DXSampleHelper.cpp' ] }
]
Interface: 'Interface.cpp'
Source: [
  'Main.cpp'
]
PlatformLibraries: [
  'D3D12.lib'
  'D3DCompiler.lib'
  'DXGI.lib'
]
RuntimeDependencies: [
  'Shaders.hlsl'
]
```

## PackageLock.sml
The package lock that was generated to capture the unique dependencies required to build this project.
```sml
Version: 5
Closures: {
  Root: {
    'C++': {
      'Samples.Cpp.DirectX': { Version: './', Build: 'Build0', Tool: 'Tool0' }
    }
  }
  Build0: {
    Wren: {
      'mwasplund|Soup.Cpp': { Version: '0.13.0' }
    }
  }
  Tool0: {
    'C++': {
      'mwasplund|copy': { Version: '1.1.0' }
      'mwasplund|mkdir': { Version: '1.1.0' }
    }
  }
}
```

## Content
The remaining content assumes that you are familiar with creating a graphics application and windows. This is not meant as a graphics tutorial and is only present as a sample of a more complex scenario for building within Soup.