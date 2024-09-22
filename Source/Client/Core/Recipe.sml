Name: 'Soup.Core'
Language: (C++@0)
Version: 0.1.1
Defines: [
	# 'TRACE_SYSTEM_ACCESS'
	# 'TRACE_FILE_SYSTEM_STATE'
]
IncludePaths: [
	'Source/'
]
Interface: 'Source/Module.cpp'
Source: [
	'Source/Recipe/LanguageReferenceParser.cpp'
	'Source/SML/SMLParser.cpp'
	'Source/ValueTable/Value.cpp'
]
Dependencies: {
	Build: [
		# 'mwasplund|Soup.Test.Cpp@0'
	]
	Runtime: [
		'mwasplund|Opal@0'
		'mwasplund|reflex@1'
		'../../Monitor/Host/'
		'mwasplund|wren@1'
		'mwasplund|CryptoPP@1'
	]
	Test: [
		'mwasplund|Soup.Test.Assert@0'
	]
}
Tests: {
	Source: [
		'UnitTests/gen/Main.cpp'
	]
	IncludePaths: [
		'UnitTests/'
	]
}