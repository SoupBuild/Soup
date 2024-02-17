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
		# 'mwasplund|Soup.Test.Cpp@0'
	]
	Runtime: [
		"Opal@0.5"
		"reflex@1.0"
		"../../Monitor/Host/"
		"wren@1.0"
		"CryptoPP@1.1"
	]
	Test: [
		"Soup.Test.Assert@0.3"
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
