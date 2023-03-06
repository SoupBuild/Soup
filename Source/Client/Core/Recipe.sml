Name: "Soup.Core"
Language: "C++|0.1"
Version: "0.1.1"
Defines: [
	# "LOCAL_DEBUG"
	# "TRACE_SYSTEM_ACCESS"
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
		"Soup.Test.Cpp@0.6.0"
	]
	Runtime: [
		"Opal@0.5.0"
		"reflex@1.0.2"
		"../../Monitor/Host/"
		"wren@1.0.0"
		"CryptoPP@1.1.0"
	]
	Test: [
		"Soup.Test.Assert@0.3.0"
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
