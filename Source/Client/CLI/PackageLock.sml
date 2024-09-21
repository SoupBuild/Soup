Version: 5
Closures: {
	Root: {
		'C++': {
			copy: { Version: '../../Tools/Copy/', Build: 'Build0', Tool: 'Tool0' }
			mkdir: { Version: '../../Tools/Mkdir/', Build: 'Build0', Tool: 'Tool0' }
			'Monitor.Host': { Version: '../../Monitor/Host/', Build: 'Build0', Tool: 'Tool0' }
			'Monitor.Shared': { Version: '../../Monitor/Shared/', Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|CryptoPP': { Version: 1.2.2, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Detours': { Version: 4.0.12, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Opal': { Version: 0.11.3, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|reflex': { Version: 1.0.4, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Soup.Test.Assert': { Version: 0.4.2, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|wren': { Version: 1.0.5, Build: 'Build0', Tool: 'Tool0' }
			Soup: { Version: '../CLI', Build: 'Build0', Tool: 'Tool0' }
			'Soup.Core': { Version: '../Core/', Build: 'Build1', Tool: 'Tool0' }
			'Soup.Generate': { Version: '../../Generate/', Build: 'Build0', Tool: 'Tool0' }
		}
		Wren: {
			'mwasplund|Soup.Build.Utils': { Version: 0.7.0, Build: 'Build2', Tool: 'Tool0' }
			'mwasplund|Soup.Wren': { Version: 0.4.1, Build: 'Build2', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'mwasplund|Soup.Cpp': { Version: 0.13.1 }
		}
	}
	Build1: {
		Wren: {
			'mwasplund|Soup.Cpp': { Version: 0.13.1 }
			'mwasplund|Soup.Test.Cpp': { Version: 0.12.0 }
		}
	}
	Build2: {
		Wren: {
			'mwasplund|Soup.Wren': { Version: 0.4.1 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}