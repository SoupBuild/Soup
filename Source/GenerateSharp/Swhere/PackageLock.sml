Version: 4
Closures: {
	Root: {
		"C#": [
			{ Name: "Opal", Version: "../Opal/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup.Build.Utilities", Version: "../Utilities/", Build: "Build1", Tool: "Tool0" }
			{ Name: "Swhere", Version: "../Swhere", Build: "Build0", Tool: "Tool0" }
			{ Name: "Swhere.Core", Version: "../Swhere.Core/", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Soup.CSharp", Version: "0.11.1" }
		]
	}
	Build1: {
		Wren: [
			{ Name: "Soup.CSharp", Version: "0.11.1" }
			{ Name: "Soup.CSharp.Nuget", Version: "0.2.0" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}