Name: "Soup.Build.Utilities"
Language: "C#|0.1"
Version: "1.0.0"
Source: [
	"LocalUserConfig/LocalUserConfig.cs"
	"LocalUserConfig/LocalUserConfigExtensions.cs"
	"LocalUserConfig/SDKConfig.cs"
	"OperationGraph/OperationGraph.cs"
	"OperationGraph/OperationGraphManager.cs"
	"OperationGraph/OperationGraphReader.cs"
	"OperationGraph/OperationGraphWriter.cs"
	"OperationGraph/OperationInfo.cs"
	"OperationGraph/OperationResult.cs"
	"OperationGraph/OperationResults.cs"
	"OperationGraph/OperationResultsManager.cs"
	"OperationGraph/OperationResultsReader.cs"
	"PackageLock/PackageLock.cs"
	"PackageLock/PackageLockExtensions.cs"
	"Recipe/LanguageReference.cs"
	"Recipe/PackageReference.cs"
	"Recipe/Recipe.cs"
	"Recipe/RecipeExtensions.cs"
	"SML/Grammar/SMLBaseVisitor.cs"
	"SML/Grammar/SMLExtensions.cs"
	"SML/Grammar/SMLLexer.cs"
	"SML/Grammar/SMLParser.cs"
	"SML/Grammar/SMLVisitor.cs"
	"SML/SMLArray.cs"
	"SML/SMLArrayValue.cs"
	"SML/SMLBooleanValue.cs"
	"SML/SMLDocument.cs"
	"SML/SMLFloatValue.cs"
	"SML/SMLIntegerValue.cs"
	"SML/SMLManager.cs"
	"SML/SMLStringValue.cs"
	"SML/SMLTable.cs"
	"SML/SMLTableValue.cs"
	"SML/SMLToken.cs"
	"SML/SMLValue.cs"
	"SML/SMLValueTableVisitor.cs"
	"ValueTable/Value.cs"
	"ValueTable/ValueList.cs"
	"ValueTable/ValueTable.cs"
	"ValueTable/ValueTableManager.cs"
	"ValueTable/ValueTableReader.cs"
	"ValueTable/ValueTableWriter.cs"
	"ValueTable/ValueType.cs"
	"BuildConstants.cs"
	"FileSystemState.cs"
	"HandledException.cs"
]

Dependencies: {
	Build: [
		"Soup.CSharp.Nuget@0.1"
	]
	Runtime: [
		"../Opal/"
	]
}

Nuget: {
	Dependencies: {
		"Runtime": [
			{ Name: "Antlr4.Runtime.Standard", Version: "4.11.1" }
		]
	}
}