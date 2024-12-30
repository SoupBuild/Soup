Name: 'Soup.Core'
Language: 'C++|0'
Version: 0.1.1
Defines: [
	# 'TRACE_SYSTEM_ACCESS'
	# 'TRACE_FILE_SYSTEM_STATE'
]
IncludePaths: [
	'source/'
]
Interface: 'source/Module.cpp'
Source: [
	'source/recipe/LanguageReferenceParser.cpp'
	'source/sml/SMLParser.cpp'
	'source/value-table/Value.cpp'
]
Dependencies: {
	Build: [
		'mwasplund|Soup.Test.Cpp@0'
	]
	Runtime: [
		'mwasplund|Opal@0'
		'mwasplund|reflex@1'
		'../../monitor/host/'
		'mwasplund|wren@1'
		'mwasplund|CryptoPP@1'
	]
	Test: [
		'mwasplund|Soup.Test.Assert@0'
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