Name: "Samples.Cpp.BuildExtension.Extension"
Language: "C#|0.1"
Version: "1.0.0"
Source: [
	"CustomBuildTask.cs"
]

Dependencies: {
	Runtime: [
		{ Reference: "Soup.Build@0.2.0", ExcludeRuntime: true }
		{ Reference: "Soup.Build.Extensions@0.4.0" }
		{ Reference: "Opal@1.1.0" }
	]
}