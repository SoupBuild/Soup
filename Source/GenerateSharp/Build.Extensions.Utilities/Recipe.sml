Name: "Soup.Build.Extensions.Utilities"
Language: "C#|0.1"
Version: "0.3.0"
Source: [
	"Recipe/PackageReference.cs"
	"SharedOperations.cs"
]

Dependencies: {
	Runtime: [
		{ Reference: "Opal@1.1.0", }
		{ Reference: "Soup.Build.Extensions@0.3.0", }
	]
}
