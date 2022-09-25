// A sample Monitor that logs all access to disk
Name: "LogMonitor"
Language: "C++|0.1"
Version: "1.0.0"
Type: "Executable"
Source: [
	"Main.cpp"
]

Dependencies: {
	Runtime: [
		"Detours@4.0.9"
		"Opal@0.5.0"
		"../Shared/"
	]
}
