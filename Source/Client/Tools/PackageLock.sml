Version: 5
Closures: {
	Root: {
		"C++": {
			"Monitor.Host": { Version: "../../Monitor/Host/", Build: "Build0", Tool: "Tool0" }
			"Monitor.Shared": { Version: "../../Monitor/Shared/", Build: "Build0", Tool: "Tool0" }
			"mwasplund|CryptoPP": { Version: "1.2.1", Build: "Build0", Tool: "Tool0" }
			"mwasplund|Detours": { Version: "4.0.11", Build: "Build0", Tool: "Tool0" }
			"mwasplund|json11": { Version: "1.1.0", Build: "Build0", Tool: "Tool0" }
			"mwasplund|Opal": { Version: "0.9.5", Build: "Build0", Tool: "Tool0" }
			"mwasplund|reflex": { Version: "1.0.3", Build: "Build0", Tool: "Tool0" }
			"mwasplund|wren": { Version: "1.0.4", Build: "Build0", Tool: "Tool0" }
			"Soup.Core": { Version: "../Core/", Build: "Build0", Tool: "Tool0" }
			SoupTools: { Version: "../Tools", Build: "Build0", Tool: "Tool0" }
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