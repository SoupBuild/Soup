Name: "Soup.Core"
Language: "C++|0.1"
Version: "0.1.1"
Defines: [
	# "LOCAL_DEBUG"
	# "TRACE_SYSTEM_ACCESS"
]
IncludePaths: [
	"Source/"
	"C:/Users/mwasp/Dev/Repos/wren/src/include"
]
Interface: "Source/Module.cpp"
Source: [
	"Source/Recipe/LanguageReferenceParser.cpp"
	"Source/SML/SMLParser.cpp"
	"Source/ValueTable/Value.cpp"
]

Dependencies: {
	Build: [
		# "Soup.Test.Cpp@0.4.0"
	]
	Runtime: [
		"Opal@0.5.0"
		"reflex@1.0.2"
		"../../Monitor/Host/"
		"C:/Users/mwasp/Dev/Repos/wren/src"
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
