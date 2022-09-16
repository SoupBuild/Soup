Name: "Soup.Build.Generate"
Language: "C#|0.1"
Version: "1.0.0"
Type: "Executable"
Source: [
	"Contracts/ValueTableManager.cs"
	"Contracts/ValueTableReader.cs"
	"Contracts/ValueTableWriter.cs"
	"OperationGraph/OperationGraphManager.cs"
	"OperationGraph/OperationGraphReader.cs"
	"OperationGraph/OperationGraphWriter.cs"
	"Recipe/Recipe.cs"
	"Recipe/RecipeExtensions.cs"
	"BuildConstants.cs"
	"BuildGenerateEngine.cs"
	"BuildTaskManager.cs"
	"ExtensionLoadContext.cs"
	"Program.cs"
]
NuGetPackages: [
	"Microsoft.Extensions.DependencyInjection@5.0.1"
]

Dependencies: {
	Runtime: [
		"../Build/"
		"../Opal/"
		"../Runtime/"
		"../Utilities/"
		"../../../../runtime/src/libraries/Microsoft.Extensions.DependencyInjection/src/"
		"../../../../runtime/src/libraries/Microsoft.Extensions.DependencyInjection.Abstractions/src/"
	]
}