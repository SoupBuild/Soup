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
			'mwasplund|Soup.CSharp': { Version: 0.15.0 }
		}
	}
	Build1: {
		Wren: {
			'mwasplund|Soup.CSharp': { Version: 0.15.0 }
			'mwasplund|Soup.CSharp.Nuget': { Version: 0.5.0 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}