Name: "Samples.Cpp.DynamicLibrary.Library"
Language: "C++|0"
Version: "1.0.0"
Type: "DynamicLibrary"
Defines: [
	"EXPORT_LIBRARY"
]
Source: [
	"Library.cpp"
]
IncludePaths: [
	"public/"
]
PublicHeaders: [
	{
		Root: "public/"
		Files: [
			"Library.h"
		]
	}
]