Version: 4
Closures: {
	Root: {
		"C#": [
			{ Name: "Samples.CSharp.BuildExtension.Executable", Version: "../Executable", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		"C#": [
			{ Name: "Samples.CSharp.BuildExtension.Extension", Version: "../Extension/" }
		]
		Wren: [
			{ Name: "Soup.CSharp", Version: "0.9.0" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}