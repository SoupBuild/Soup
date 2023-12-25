Version: 5
Closures: {
	Root: {
		"C++": {
			"Samples.Cpp.ConsoleApplication": { Version: "../ConsoleApplication", Build: "Build0", Tool: "Tool0" }
		}
	}
	Build0: {
		Wren: {
			"mwasplund|Soup.Cpp": { Version: "0.11.0" }
		}
	}
	Tool0: {
		"C++": {
			"mwasplund|copy": { Version: "1.0.0" }
			"mwasplund|mkdir": { Version: "1.0.0" }
		}
	}
}