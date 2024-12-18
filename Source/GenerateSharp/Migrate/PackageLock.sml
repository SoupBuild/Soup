Version: 5
Closures: {
	Root: {
		'C#': {
			Migrate: { Version: './', Build: 'Build0', Tool: 'Tool0' }
			Opal: { Version: '../Opal/', Build: 'Build0', Tool: 'Tool0' }
			'Soup.Utilities': { Version: '../Utilities/', Build: 'Build1', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Soup|CSharp': { Version: 0.15.1 }
		}
	}
	Build1: {
		Wren: {
			'Soup|CSharp': { Version: 0.15.1 }
			'Soup|CSharp.Nuget': { Version: 0.5.1 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}