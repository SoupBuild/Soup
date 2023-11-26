Version: 4
Closures: {
	Root: {
		"C++": [
			{ Name: "Samples.Cpp.ModuleDynamicLibrary.Application", Version: "../Application", Build: "Build0", Tool: "Tool0" }
			{ Name: "Samples.Cpp.ModuleDynamicLibrary.Library", Version: "../Library/", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Soup.Cpp", Version: "0.10.1" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}