Version: 4
Closures: {
	Root: {
		"C++": [
			{ Name: "copy", Version: "../../Tools/Copy/", Build: "Build0", Tool: "Tool0" }
			{ Name: "CryptoPP", Version: "1.2.1", Build: "Build0", Tool: "Tool0" }
			{ Name: "Detours", Version: "4.0.11", Build: "Build0", Tool: "Tool0" }
			{ Name: "mkdir", Version: "../../Tools/Mkdir/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Monitor.Host", Version: "../../Monitor/Host/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Monitor.Shared", Version: "../../Monitor/Shared/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Opal", Version: "0.9.5", Build: "Build0", Tool: "Tool0" }
			{ Name: "reflex", Version: "1.0.3", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup", Version: "../CLI", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup.Core", Version: "../Core/", Build: "Build1", Tool: "Tool0" }
			{ Name: "Soup.Generate", Version: "../../Generate/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup.Test.Assert", Version: "0.3.1", Build: "Build0", Tool: "Tool0" }
			{ Name: "wren", Version: "1.0.3", Build: "Build0", Tool: "Tool0" }
		]
		Wren: [
			{ Name: "Soup.Build.Utils", Version: "0.4.0", Build: "Build2", Tool: "Tool0" }
			{ Name: "Soup.Cpp", Version: "0.8.2", Build: "Build2", Tool: "Tool0" }
			{ Name: "Soup.Cpp.Compiler", Version: "0.7.0", Build: "Build2", Tool: "Tool0" }
			{ Name: "Soup.Cpp.Compiler.GCC", Version: "0.1.1", Build: "Build2", Tool: "Tool0" }
			{ Name: "Soup.Cpp.Compiler.MSVC", Version: "0.7.0", Build: "Build2", Tool: "Tool0" }
			{ Name: "Soup.CSharp", Version: "0.9.0", Build: "Build2", Tool: "Tool0" }
			{ Name: "Soup.CSharp.Compiler", Version: "0.8.0", Build: "Build2", Tool: "Tool0" }
			{ Name: "Soup.CSharp.Compiler.Roslyn", Version: "0.8.0", Build: "Build2", Tool: "Tool0" }
			{ Name: "Soup.Wren", Version: "0.2.0", Build: "Build2", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Soup.Cpp", Version: "0.9.1" }
		]
	}
	Build1: {
		Wren: [
			{ Name: "Soup.Cpp", Version: "0.9.1" }
			{ Name: "Soup.Test.Cpp", Version: "0.9.0" }
		]
	}
	Build2: {
		Wren: [
			{ Name: "Soup.Wren", Version: "0.2.0" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}