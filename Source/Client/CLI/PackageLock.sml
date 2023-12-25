Version: 5
Closures: {
	Root: {
		"C++": {
			copy: { Version: "../../Tools/Copy/", Build: "Build0", Tool: "Tool0" }
			mkdir: { Version: "../../Tools/Mkdir/", Build: "Build0", Tool: "Tool0" }
			"Monitor.Host": { Version: "../../Monitor/Host/", Build: "Build0", Tool: "Tool0" }
			"Monitor.Shared": { Version: "../../Monitor/Shared/", Build: "Build0", Tool: "Tool0" }
			"mwasplund|CryptoPP": { Version: "1.2.1", Build: "Build0", Tool: "Tool0" }
			"mwasplund|Detours": { Version: "4.0.11", Build: "Build0", Tool: "Tool0" }
			"mwasplund|Opal": { Version: "0.9.5", Build: "Build0", Tool: "Tool0" }
			"mwasplund|reflex": { Version: "1.0.3", Build: "Build0", Tool: "Tool0" }
			"mwasplund|wren": { Version: "1.0.4", Build: "Build0", Tool: "Tool0" }
			Soup: { Version: "./", Build: "Build0", Tool: "Tool0" }
			"Soup.Core": { Version: "../Core/", Build: "Build0", Tool: "Tool0" }
			"Soup.Generate": { Version: "../../Generate/", Build: "Build0", Tool: "Tool0" }
		}
		Wren: {
			"mwasplund|Soup.Build.Utils": { Version: "0.6.0", Build: "Build1", Tool: "Tool0" }
			"mwasplund|Soup.Cpp": { Version: "0.8.2", Build: "Build1", Tool: "Tool0" }
			"mwasplund|Soup.Cpp.Compiler": { Version: "0.10.0", Build: "Build1", Tool: "Tool0" }
			"mwasplund|Soup.Cpp.Compiler.GCC": { Version: "0.3.0", Build: "Build1", Tool: "Tool0" }
			"mwasplund|Soup.Cpp.Compiler.MSVC": { Version: "0.9.0", Build: "Build1", Tool: "Tool0" }
			"mwasplund|Soup.CSharp": { Version: "0.9.0", Build: "Build1", Tool: "Tool0" }
			"mwasplund|Soup.CSharp.Compiler": { Version: "0.10.0", Build: "Build1", Tool: "Tool0" }
			"mwasplund|Soup.CSharp.Compiler.Roslyn": { Version: "0.11.0", Build: "Build1", Tool: "Tool0" }
			"mwasplund|Soup.Wren": { Version: "0.3.0", Build: "Build1", Tool: "Tool0" }
		}
	}
	Build0: {
		Wren: {
			"mwasplund|Soup.Cpp": { Version: "0.11.0" }
		}
	}
	Build1: {
		Wren: {
			"mwasplund|Soup.Wren": { Version: "0.3.0" }
		}
	}
	Tool0: {
		"C++": {
			"mwasplund|copy": { Version: "1.0.0" }
			"mwasplund|mkdir": { Version: "1.0.0" }
		}
	}
}