Name: "Soup.PackageManager.Core"
Language: "C#|0.1"
Version: "1.0.0"
Source: [
	"ClosureManager.cs"
	"IAuthenticationManager.cs"
	"IClosureManager.cs"
	"InitializeCommand.cs"
	"IZipArchive.cs"
	"IZipManager.cs"
	"PackageManager.cs"
]

Dependencies: {
	Runtime: [
		"../Api.Client/"
		"../Utilities/"
		"../Opal/"
	]
}
