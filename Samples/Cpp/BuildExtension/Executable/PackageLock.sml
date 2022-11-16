Version: 3
Closures: {
	Root: {
		Cpp: [
			{ Name: "Samples.SimpleBuildExtension.Executable", Version: "../Executable", Build: "Build0" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: "Soup.Cpp", Version: "0.4.0" }
			{ Name: "Samples.Cpp.BuildExtension.Extension", Version: "../Extension/" }
		]
	}
}