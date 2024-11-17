Version: 5
Closures: {
	Root: {
		C: {
			'mwasplund|libseccomp': { Version: 2.5.7, Build: 'Build1', Tool: 'Tool0' }
		}
		'C++': {
			'Monitor.Client': { Version: './', Build: 'Build0', Tool: 'Tool0' }
			'Monitor.Shared': { Version: '../Shared/', Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Detours': { Version: 4.0.12, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Opal': { Version: 0.11.3, Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'mwasplund|Soup.Cpp': { Version: 0.13.2 }
		}
	}
	Build1: {
		Wren: {
			'mwasplund|Soup.C': { Version: 0.4.1 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}