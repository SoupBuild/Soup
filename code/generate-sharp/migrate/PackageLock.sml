Version: 5
Closures: {
	Root: {
		'C#': {
			Migrate: { Version: './', Build: 'Build0', Tool: 'Tool0' }
			Opal: { Version: '../opal/', Build: 'Build0', Tool: 'Tool0' }
			'Soup.Utilities': { Version: '../utilities/', Build: 'Build1', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Soup|CSharp': { Version: 0.15.2 }
		}
	}
	Build1: {
		Wren: {
			'Soup|CSharp': { Version: 0.15.2 }
			'Soup|CSharp.Nuget': { Version: 0.5.2 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}