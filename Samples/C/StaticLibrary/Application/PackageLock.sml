Version: 5
Closures: {
	Root: {
		C: {
			'Samples.C.StaticLibrary.Application': { Version: '../Application', Build: 'Build0', Tool: 'Tool0' }
			'Samples.C.StaticLibrary.Library': { Version: '../Library/', Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'mwasplund|Soup.C': { Version: 0.4.0 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}