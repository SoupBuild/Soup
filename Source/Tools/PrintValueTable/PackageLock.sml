Version: 5
Closures: {
	Root: {
		C: {
			'mwasplund|libseccomp': { Version: 2.5.8, Build: 'Build2', Tool: 'Tool0' }
		}
		'C++': {
			'Monitor.Host': { Version: '../../Monitor/Host/', Build: 'Build0', Tool: 'Tool0' }
			'Monitor.Shared': { Version: '../../Monitor/Shared/', Build: 'Build0', Tool: 'Tool0' }
			'Soup.Core': { Version: '../../Client/Core/', Build: 'Build1', Tool: 'Tool0' }
			'mwasplund|CryptoPP': { Version: 1.2.4, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Detours': { Version: 4.0.12, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Opal': { Version: 0.11.5, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|Soup.Test.Assert': { Version: 0.4.2, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|reflex': { Version: 1.0.5, Build: 'Build0', Tool: 'Tool0' }
			'mwasplund|wren': { Version: 1.0.5, Build: 'Build0', Tool: 'Tool0' }
			printvaluetable: { Version: './', Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'mwasplund|Soup.Cpp': { Version: 0.13.2 }
		}
	}
	Build1: {
		Wren: {
			'mwasplund|Soup.Cpp': { Version: 0.13.2 }
			'mwasplund|Soup.Test.Cpp': { Version: 0.12.0 }
		}
	}
	Build2: {
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