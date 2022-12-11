Version: 4
Closures: {
	Root: {
		"C++": [
			{ Name: "CryptoPP", Version: "1.2.1", Build: "Build0" }
			{ Name: "Detours", Version: "4.0.9", Build: "Build0" }
			{ Name: "Monitor.Host", Version: "../../Monitor/Host/", Build: "Build0" }
			{ Name: "Monitor.Shared", Version: "../../Monitor/Shared/", Build: "Build0" }
			{ Name: "Opal", Version: "0.8.2", Build: "Build0" }
			{ Name: "printresults", Version: "../PrintResults", Build: "Build0" }
			{ Name: "reflex", Version: "1.0.2", Build: "Build0" }
			{ Name: "Soup.Core", Version: "../../Client/Core/", Build: "Build1" }
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
}