Name: "Soup.Core"
Language: "C++|0"
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
		# TODO: "Soup.Test.Cpp@0.6.0"
	]
	Runtime: [
		"mwasplund|Opal@0"
		"mwasplund|reflex@1"
		"../../Monitor/Host/"
		"mwasplund|wren@1"
		"mwasplund|CryptoPP@1"
	]
	Test: [
		# TODO: "Soup.Test.Assert@0.3"
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
