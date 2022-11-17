Version: 4
Closures: {
	Root: {
		"C#": [
			{ Name: "Opal", Version: "1.3.0", Build: "Build2" }
			{ Name: "Soup.Build", Version: "0.2.0", Build: "Build2" }
			{ Name: "Soup.Build.Extensions", Version: "0.4.1", Build: "Build2" }
			{ Name: "Soup.Build.Extensions.Utilities", Version: "0.4.1", Build: "Build2" }
			{ Name: "Soup.Cpp", Version: "0.4.0", Build: "Build2" }
			{ Name: "Soup.Cpp.Compiler", Version: "0.5.0", Build: "Build2" }
			{ Name: "Soup.Cpp.Compiler.MSVC", Version: "0.5.0", Build: "Build2" }
			{ Name: "Soup.CSharp", Version: "0.7.0", Build: "Build2" }
			{ Name: "Soup.CSharp.Compiler", Version: "0.6.0", Build: "Build2" }
			{ Name: "Soup.CSharp.Compiler.Roslyn", Version: "0.6.0", Build: "Build2" }
		]
		"C++": [
			{ Name: "copy", Version: "../../Tools/Copy/", Build: "Build0" }
			{ Name: "CryptoPP", Version: "1.2.0", Build: "Build0" }
			{ Name: "Detours", Version: "4.0.9", Build: "Build0" }
			{ Name: "mkdir", Version: "../../Tools/Mkdir/", Build: "Build0" }
			{ Name: "Monitor.Host", Version: "../../Monitor/Host/", Build: "Build0" }
			{ Name: "Monitor.Shared", Version: "../../Monitor/Shared/", Build: "Build0" }
			{ Name: "Opal", Version: "0.5.1", Build: "Build0" }
			{ Name: "reflex", Version: "1.0.1", Build: "Build0" }
			{ Name: "Soup", Version: "../CLI", Build: "Build0" }
			{ Name: "Soup.Core", Version: "../Core/", Build: "Build1" }
			{ Name: "Soup.Test.Assert", Version: "0.3.1", Build: "Build0" }
		]
	}
	Build0: {
		"C#": [
			{ Name: "Soup.Cpp", Version: "0.4.0" }
		]
	}
	Build1: {
		"C#": [
			{ Name: "Soup.Cpp", Version: "0.4.0" }
			{ Name: "Soup.Test.Cpp", Version: "0.4.0" }
		]
	}
	Build2: {
		"C#": [
			{ Name: "Soup.CSharp", Version: "0.7.0" }
		]
	}
}