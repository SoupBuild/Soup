Name: 'Monitor.Host'
Language: (C++@0)
Version: 1.0.0
Defines: [
	# 'TRACE_DETOUR_SERVER'
	# 'TRACE_MONITOR_HOST'
]
Interface: 'Module.cpp'
Source: []
Dependencies: {
	Runtime: [
		'mwasplund|Detours@4'
		'../Shared/'
		'mwasplund|Opal@0'
	]
}
