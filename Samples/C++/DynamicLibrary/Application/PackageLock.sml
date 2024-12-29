Version: 5
Closures: {
	Root: {
		'C++': {
			'Samples.Cpp.DynamicLibrary.Application': { Version: './', Build: 'Build0', Tool: 'Tool0' }
			'Samples.Cpp.DynamicLibrary.Library': { Version: '../Library/', Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Soup|Cpp': { Version: 0.13.2 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}