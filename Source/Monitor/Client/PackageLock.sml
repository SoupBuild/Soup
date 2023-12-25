Version: 5
Closures: {
	Root: {
		"C++": {
			"Monitor.Client": { Version: "../Client", Build: "Build0", Tool: "Tool0" }
			"Monitor.Shared": { Version: "../Shared/", Build: "Build0", Tool: "Tool0" }
			"mwasplund|Detours": { Version: "4.0.11", Build: "Build0", Tool: "Tool0" }
			"mwasplund|Opal": { Version: "0.9.5", Build: "Build0", Tool: "Tool0" }
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