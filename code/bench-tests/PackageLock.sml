Version: 5
Closures: {
	Root: {
		'C++': {
			'Monitor.Host': { Version: '../monitor/host/', Build: 'Build0', Tool: 'Tool0' }
			'Monitor.Shared': { Version: '../monitor/shared/', Build: 'Build0', Tool: 'Tool0' }
			'Soup.BenchTests': { Version: './', Build: 'Build0', Tool: 'Tool0' }
			'Soup.Core': { Version: '../client/core/', Build: 'Build1', Tool: 'Tool0' }
			'mwasplund|CryptoPP': { Version: 1.2.4, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Detours': { Version: 4.0.12, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Opal': { Version: 0.11.5, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Soup.Test.Assert': { Version: 0.4.2, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|reflex': { Version: 1.0.5, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|wren': { Version: 1.0.5, Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Soup|Cpp': { Version: 0.13.2 }
		}
	}
	Build1: {
		Wren: {
			'Soup|Cpp': { Version: 0.13.2 }
			'mwasplund|Soup.Test.Cpp': { Version: 0.11.0 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}