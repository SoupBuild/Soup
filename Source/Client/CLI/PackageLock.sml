Version: 2
Closures: {
	Root: {
		C#: [
			{ Name: "Opal", Version: "1.0.3", Build: "Build2", }
			{ Name: "Soup.Build", Version: "0.2.0", Build: "Build2", }
			{ Name: "Soup.Build.Extensions", Version: "0.2.0", Build: "Build2", }
			{ Name: "Soup.Build.Extensions.Utilities", Version: "0.2.1", Build: "Build3", }
			{ Name: "Soup.Cpp", Version: "0.2.2", Build: "Build2", }
			{ Name: "Soup.Cpp.Compiler", Version: "0.3.1", Build: "Build2", }
			{ Name: "Soup.Cpp.Compiler.MSVC", Version: "0.3.1", Build: "Build2", }
			{ Name: "Soup.CSharp", Version: "0.5.2", Build: "Build2", }
			{ Name: "Soup.CSharp.Compiler", Version: "0.4.2", Build: "Build2", }
			{ Name: "Soup.CSharp.Compiler.Roslyn", Version: "0.4.1", Build: "Build2", }
			{ Name: "Soup.Test.Cpp", Version: "0.2.1", Build: "Build2", }
		]
		C++: [
			{ Name: "copy", Version: "../../Tools/Copy/", Build: "Build0", }
			{ Name: "CryptoPP", Version: "1.0.2", Build: "Build0", }
			{ Name: "Detours", Version: "4.0.3", Build: "Build0", }
			{ Name: "json11", Version: "1.0.2", Build: "Build0", }
			{ Name: "mkdir", Version: "../../Tools/Mkdir/", Build: "Build0", }
			{ Name: "Monitor.Host", Version: "../../Monitor/Host/", Build: "Build0", }
			{ Name: "Monitor.Shared", Version: "../../Monitor/Shared/", Build: "Build0", }
			{ Name: "Opal", Version: "0.4.1", Build: "Build0", }
			{ Name: "reflex", Version: "1.0.1", Build: "Build0", }
			{ Name: "Soup", Version: "../CLI", Build: "Build0", }
			{ Name: "Soup.Core", Version: "../Core/", Build: "Build1", }
			{ Name: "Soup.Test.Assert", Version: "0.2.1", Build: "Build0", }
		]
	}
	Build0: {
		C#: [
			{ Name: "C++", Version: "0.1.0", }
		]
	}
	Build1: {
		C#: [
			{ Name: "C++", Version: "0.1.0", }
			{ Name: "Soup.Test.Cpp", Version: "0.2.1", }
		]
	}
	Build2: {
		C#: [
			{ Name: "C#", Version: "0.1.0", }
		]
	}
	Build3: {
		C#: [
			{ Name: "C#", Version: "0.0.0", }
		]
	}
}
