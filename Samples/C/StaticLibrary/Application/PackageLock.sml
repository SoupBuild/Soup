Version: 4
Closures: {
	Root: {
		C: [
			{ Name: "Samples.C.StaticLibrary.Application", Version: "../Application", Build: "Build0", Tool: "Tool0" }
			{ Name: "Samples.C.StaticLibrary.Library", Version: "../Library/", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Soup.C", Version: "0.1.0" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}