Name: "Soup"
Version: "0.29.0"
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
		"../../Tools/Copy/"
		"../../Tools/Mkdir/"
		"../../Monitor/Host/"
	]
	Other: [
		# "../../GenerateSharp/Generate/"
		"Wren|Soup.Cpp@0.6.1"
		"Wren|Soup.CSharp@0.8.0"
		"Wren|Soup.Wren@0.1.0"
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
