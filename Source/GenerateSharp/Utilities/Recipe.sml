Name: "Soup.Build.Utilities"
Language: "C#|0.1"
Version: "1.0.0"
Source: [
	"BuildOperation.cs"
	"IValueTableExtensions.cs"
	"MockBuildState.cs"
	"PackageReference.cs"
	"SharedOperations.cs"
]

Dependencies: {
	Runtime: [
		"../Contracts/"
		"../Opal/"
		"../Runtime/"
	]
}
