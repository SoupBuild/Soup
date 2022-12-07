Name: "Soup.Build.Extensions"
Language: "C#|0.1"
Version: "0.4.1"
Source: [
	"BuildOperation.cs"
	"IBuildStateExtensions.cs"
	"IValueExtensions.cs"
	"IValueListExtensions.cs"
	"IValueTableExtensions.cs"
]

Dependencies: {
	Runtime: [
		{ Reference: "Opal@1.1.0" }
		{ Reference: "Soup.Build@0.2.0", ExcludeRuntime: true }
	]
}