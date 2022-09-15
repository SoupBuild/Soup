Name: "Monitor.Host"
Language: "C++|0.1"
Version: "1.0.0"
Defines: [
	// "TRACE_DETOUR_SERVER"
]
Interface: "Module.cpp"

Dependencies: {
	Runtime: [
		"Detours@4.0.3"
		"../Shared/"
		"Opal@0.4.1"
	]
}
