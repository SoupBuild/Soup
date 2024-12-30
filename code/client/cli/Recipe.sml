Name: 'Soup'
Version: 0.41.2
Language: 'C++|0'
Type: 'Executable'
Source: [
	'source/main.cpp'
]
IncludePaths: [
	'source/commands/'
	'source/options/'
]
Dependencies: {
	# Ensure the core build extensions are runtime dependencies
	Runtime: [
		'../core/'
		'../../generate/'
		'../../monitor/host/'
	]
	Other: [
		'../../tools/copy/'
		'../../tools/mkdir/'
		'[Wren]Soup|Wren@0'
	]
	Build: [
		# TODO: 'Soup.Test.Cpp@0.4.0'
	]
	Test: [
		# TODO: 'Soup.Test.Assert@0.3.0'
	]
}
Tests: {
	Source: [
		'tests/gen/main.cpp'
	]
	IncludePaths: [
		'tests/'
	]
}