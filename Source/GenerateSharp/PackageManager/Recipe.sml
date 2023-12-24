Name: "Soup.PackageManager"
Language: "C#|0"
Version: "1.0.0"
Type: "Executable"
Source: [
	"AuthenticationManager.cs"
	"CompressionZipArchive.cs"
	"CompressionZipManager.cs"
	"Program.cs"
	"TokenCache.cs"
]

Dependencies: {
	Build: [
		"mwasplund|Soup.CSharp.Nuget@0"
	]
	Runtime: [
		"../PackageManager.Core/"
		"../Utilities/"
		"../Opal/"
	]
}

Nuget: {
	Dependencies: {
		"Runtime": [
			{ Name: "Microsoft.Identity.Client", Version: "4.48.1" }
			{ Name: "System.Security.Cryptography.ProtectedData", Version: "6.0.0" }
		]
	}
}