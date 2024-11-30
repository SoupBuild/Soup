Version: 5
Closures: {
	Root: {
		'C++': {
			'Samples.SimpleBuildExtension.Executable': { Version: './', Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Samples.Cpp.BuildExtension.Extension': { Version: '../Extension/' }
			'Soup|Cpp': { Version: 0.13.2 }
		}
	}
	Tool0: {
		'C++': {
			'Samples.SimpleBuildExtension.Tool': { Version: '../Tool/' }
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}