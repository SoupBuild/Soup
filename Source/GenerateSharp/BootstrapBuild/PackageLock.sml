Version: 5
Closures: {
	Root: {
		'C#': {
			Opal: { Version: '../Opal/', Build: 'Build0', Tool: 'Tool0' }
			'Soup.Utilities': { Version: '../Utilities/', Build: 'Build1', Tool: 'Tool0' }
			Swhere: { Version: './', Build: 'Build0', Tool: 'Tool0' }
			'Swhere.Core': { Version: '../Swhere.Core/', Build: 'Build0', Tool: 'Tool0' }
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