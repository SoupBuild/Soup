Version: 4
Closures: {
	Root: {
		"C#": [
			{ Name: "Opal", Version: "../Opal/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup.Api.Client", Version: "../Api.Client/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup.Build.Utilities", Version: "../Utilities/", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup.PackageManager", Version: "./", Build: "Build0", Tool: "Tool0" }
			{ Name: "Soup.PackageManager.Core", Version: "../PackageManager.Core/", Build: "Build0", Tool: "Tool0" }
		]
	}
	Build0: {
		Wren: [
			{ Name: "Soup.CSharp", Version: "0.11.1" }
			{ Name: "Soup.CSharp.Nuget", Version: "0.1.0" }
		]
	}
	Tool0: {
		"C++": [
			{ Name: "copy", Version: "1.0.0" }
			{ Name: "mkdir", Version: "1.0.0" }
		]
	}
}