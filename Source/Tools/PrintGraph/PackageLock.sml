Version: 4
Closures: {
	Root: {
		"C++": [
			{ Name: "CryptoPP", Version: "1.2.1", Build: "Build0", Tool: "Tool0" }
			{ Name: "Detours", Version: "4.0.11", Build: "Build0", Tool: "Tool0" }
			{ Name: "Monitor.Host", Version: "../../Monitor/Host/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Monitor.Shared", Version: "../../Monitor/Shared/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Opal", Version: "0.9.5", Build: "Build0", Tool: "Tool0" }
			{ Name: "printgraph", Version: "../PrintGraph", Build: "Build0", Tool: "Tool0" }
			{ Name: "reflex", Version: "1.0.3", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup.Core", Version: "../../Client/Core/", Build: "Build1", Tool: "Tool0" }
			{ Name: "Soup.Test.Assert", Version: "0.3.1", Build: "Build0", Tool: "Tool0" }
			{ Name: "wren", Version: "1.0.4", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Soup.Cpp", Version: "0.10.1" }
		]
	}
	Build1: {
		Wren: [
			{ Name: "Soup.Cpp", Version: "0.10.1" }
			{ Name: "Soup.Test.Cpp", Version: "0.9.0" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}