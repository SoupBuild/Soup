Name: "Swhere.Core"
Language: "C#|0.1"
Version: "1.0.0"
Source: [
	"Nuget/NugetPackage.cs"
	"Nuget/NugetPackageDependency.cs"
	"Nuget/NugetPackageTargetFramework.cs"
	"Nuget/NugetPackageVersion.cs"
	"Nuget/NuspecDependency.cs"
	"Nuget/NuspecDependencyBase.cs"
	"Nuget/NuspecDependencyGroup.cs"
	"Nuget/NuspecMetadata.cs"
	"Nuget/NuspecPackage.cs"
	"DotNetSDKUtilities.cs"
	"NugetSDKUtilities.cs"
	"SwhereManager.cs"
	"VSWhereUtilities.cs"
	"WindowsSDKUtilities.cs"
]

Dependencies: {
	Runtime: [
		"../Utilities/"
		"../Opal/"
	]
}
