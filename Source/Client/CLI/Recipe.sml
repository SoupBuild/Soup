Name: "Soup"
Version: "0.31.1"
Language: "C++|0.1"

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
		# "../../Tools/Copy/"
		# "../../Tools/Mkdir/"
		"../../Monitor/Host/"
	]
	Other: [
		# "Wren|Soup.Cpp@0.6.2"
		# "Wren|Soup.CSharp@0.8.0"
		# "Wren|Soup.Wren@0.1.2"
	]
	Build: [
		# TODO: "Soup.Test.Cpp@0.4.0"
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
