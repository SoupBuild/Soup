Name: "Soup"
Version: "0.24.0"
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
	// Ensure the core build extensions are runtime dependencies
	Runtime: [
		"../Core/"
		"../../Tools/Copy/"
		"../../Tools/Mkdir/"
		"../../Monitor/Host/"
	]
	Other: [
		// "../../GenerateSharp/Generate/"
		"C#|Soup.Cpp@0.4.0"
		"C#|Soup.CSharp@0.7.0"
	]
	Build: [
		// TODO: "Soup.Test.Cpp@0.4.0"
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
