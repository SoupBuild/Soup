Version: 3
Closures: {
	Root: {
		Cpp: [
			{ Name: "CryptoPP", Version: "1.1.0", Build: "Build0" }
			{ Name: "Detours", Version: "4.0.9", Build: "Build0" }
			{ Name: "Monitor.Host", Version: "../../Monitor/Host/", Build: "Build0" }
			{ Name: "Monitor.Shared", Version: "../../Monitor/Shared/", Build: "Build0" }
			{ Name: "Opal", Version: "0.5.1", Build: "Build0" }
			{ Name: "printvaluetable", Version: "../PrintValueTable", Build: "Build0" }
			{ Name: "reflex", Version: "1.0.1", Build: "Build0" }
			{ Name: "Soup.Core", Version: "../../Client/Core/", Build: "Build1" }
			{ Name: "Soup.Test.Assert", Version: "0.3.1", Build: "Build0" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: "Soup.Cpp", Version: "0.4.0" }
		]
	}
	Build1: {
		CSharp: [
			{ Name: "Soup.Cpp", Version: "0.4.0" }
			{ Name: "Soup.Test.Cpp", Version: "0.4.0" }
		]
	}
}