Version: 5
Closures: {
	Root: {
		'C++': {
			mkdir: { Version: '../Mkdir', Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Opal': { Version: 0.11.5, Build: 'Build0', Tool: 'Tool0' }
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