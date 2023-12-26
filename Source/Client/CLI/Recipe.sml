Name: "Soup"
Version: "0.37.0"
Language: "C++|0"

Type: "Executable"

Source: [
	"Source/Main.cpp"
]
IncludePaths: [
	"Source/Commands/"
	"Source/Options/"
]

Dependencies: {
	# Ensure the core build extensions are runtime dependencies
	Runtime: [
		"../Core/"
		"../../Generate/"
		"../../Monitor/Host/"
	]
	Other: [
		"../../Tools/Copy/"
		"../../Tools/Mkdir/"
		"[Wren]mwasplund|Soup.Cpp@0"
		"[Wren]mwasplund|Soup.CSharp@0"
		"[Wren]mwasplund|Soup.Wren@0"
	]
	Build: [
		# TODO: "Soup.Test.Cpp@0.4.0"
	]
	Test: [
		# TODO: "Soup.Test.Assert@0.3.0"
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
