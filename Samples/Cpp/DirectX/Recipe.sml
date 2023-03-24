Name: "Samples.Cpp.DirectX"
Language: "C++|0"
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