Version: 4
Closures: {
	Root: {
		"C++": [
			{ Name: "Samples.SimpleBuildExtension.Executable", Version: "./", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Samples.Cpp.BuildExtension.Extension", Version: "../Extension/" }
			{ Name: "Soup.Cpp", Version: "0.9.0" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}