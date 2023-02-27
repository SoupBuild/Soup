Version: 4
Closures: {
	Root: {
		"C++": [
			{ Name: "Detours", Version: "4.0.9", Build: "BuildSet0" }
			{ Name: "Monitor.Client", Version: "../Client", Build: "BuildSet0" }
			{ Name: "Monitor.Shared", Version: "../Shared/", Build: "BuildSet0" }
			{ Name: "Opal", Version: "0.9.0", Build: "BuildSet0" }
		]
	}
	BuildSet0: {
		"C++": [
			{ Name: "copy", Version: "../../Tools/Copy/" }
			{ Name: "mkdir", Version: "../../Tools/Mkdir/" }
		]
		Wren: [
			{ Name: "Soup.Cpp", Version: "0.7.0" }
		]
	}
}