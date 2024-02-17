Name: 'Monitor.Host'
Language: 'C++|0'
Version: '1.0.0'
Defines: [
	# 'TRACE_DETOUR_SERVER'
]
Interface: 'Module.cpp'

Dependencies: {
	Runtime: [
		'mwasplund|Detours@4'
		'../Shared/'
		'mwasplund|Opal@0'
	]
}
