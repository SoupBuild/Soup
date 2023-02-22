Version: 4
Closures: {
	Root: {
		"C++": [
			{ Name: "copy", Version: "../../Tools/Copy/", Build: "Build0" }
			{ Name: "CryptoPP", Version: "1.2.1", Build: "Build0" }
			{ Name: "Detours", Version: "4.0.9", Build: "Build0" }
			{ Name: "mkdir", Version: "../../Tools/Mkdir/", Build: "Build0" }
			{ Name: "Monitor.Host", Version: "../../Monitor/Host/", Build: "Build0" }
			{ Name: "Monitor.Shared", Version: "../../Monitor/Shared/", Build: "Build0" }
			{ Name: "Opal", Version: "0.9.0", Build: "Build0" }
			{ Name: "reflex", Version: "1.0.2", Build: "Build0" }
			{ Name: "Soup", Version: "../CLI", Build: "Build0" }
			{ Name: "Soup.Core", Version: "../Core/", Build: "Build2" }
			{ Name: "Soup.Generate", Version: "../../Generate/", Build: "Build0" }
			{ Name: "Soup.Test.Assert", Version: "0.3.1", Build: "Build0" }
			{ Name: "wren", Version: "1.0.0", Build: "Build0" }
		]
		Wren: [
			{ Name: "Soup.Build.Utils", Version: "0.3.0", Build: "Build1" }
			{ Name: "Soup.Cpp", Version: "0.6.2", Build: "Build1" }
			{ Name: "Soup.Cpp.Compiler", Version: "0.6.1", Build: "Build1" }
			{ Name: "Soup.Cpp.Compiler.MSVC", Version: "0.6.0", Build: "Build1" }
			{ Name: "Soup.CSharp", Version: "0.9.0", Build: "Build1" }
			{ Name: "Soup.CSharp.Compiler", Version: "0.8.0", Build: "Build1" }
			{ Name: "Soup.CSharp.Compiler.Roslyn", Version: "0.8.0", Build: "Build1" }
			{ Name: "Soup.Wren", Version: "0.1.2", Build: "Build1" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Soup.Cpp", Version: "0.6.2" }
		]
	}
	Build1: {
		"C++": [
			{ Name: "copy", Version: "../../Tools/Copy/" }
			{ Name: "mkdir", Version: "../../Tools/Mkdir/" }
		]
		Wren: [
			{ Name: "Soup.Wren", Version: "0.1.2" }
		]
	}
	Build2: {
		Wren: [
			{ Name: "Soup.Cpp", Version: "0.6.2" }
			{ Name: "Soup.Test.Cpp", Version: "0.5.2" }
		]
	}
}