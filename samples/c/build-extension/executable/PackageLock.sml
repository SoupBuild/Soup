Version: 5
Closures: {
	Root: {
		C: {
			'Samples.C.SimpleBuildExtension.Executable': { Version: './', Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Samples.C.BuildExtension.Extension': { Version: '../extension/' }
			'Soup|C': { Version: 0.4.1 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}