Version: 4
Closures: {
	Root: {
		C: [
			{ Name: "Samples.C.SimpleBuildExtension.Executable", Version: "./", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Samples.C.BuildExtension.Extension", Version: "../Extension/" }
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