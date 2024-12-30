Version: 5
Closures: {
	Root: {
		'C++': {
			'Samples.Cpp.ModuleDynamicLibrary.Application': { Version: './', Build: 'Build0', Tool: 'Tool0' }
			'Samples.Cpp.ModuleDynamicLibrary.Library': { Version: '../Library/', Build: 'Build0', Tool: 'Tool0' }
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