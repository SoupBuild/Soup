Version: 5
Closures: {
	Root: {
		'C++': {
			'Samples.SimpleBuildExtension.Executable': { Version: '../Executable', Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Samples.Cpp.BuildExtension.Extension': { Version: '../Extension/' }
			'mwasplund|Soup.Cpp': { Version: '0.12.0' }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: '1.1.0' }
			'mwasplund|mkdir': { Version: '1.1.0' }
		}
	}
}