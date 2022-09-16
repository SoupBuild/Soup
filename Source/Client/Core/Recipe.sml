Name: "Soup.Core"
Language: "C++|0.1"
Version: "0.1.1"
Defines: [
	// "LOCAL_DEBUG"
	// "TRACE_SYSTEM_ACCESS"
]
IncludePaths: [
	"Source/"
]
Interface: "Source/Module.cpp"
Source: [
	"Source/Recipe/LanguageReferenceParser.cpp"
	"Source/SML/SMLParser.cpp"
	"Source/ValueTable/Value.cpp"
]

Dependencies: {
	Build: [
		"Soup.Test.Cpp@0.3.0"
	]
	Runtime: [
		"Opal@0.4.1"
		"reflex@1.0.1"
		"../../Monitor/Host/"
		"CryptoPP@1.0.2"
	]
	Test: [
		"Soup.Test.Assert@0.2.1"
	]
}

Tests: {
	Source: [
		"UnitTests/gen/Main.cpp"
	]
	IncludePaths: [
		"UnitTests/"
	]
}
