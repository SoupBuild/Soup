Version: 5
Closures: {
	Root: {
		C: {
			'Samples.C.SimpleBuildExtension.Executable': { Version: '../Executable', Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Samples.C.BuildExtension.Extension': { Version: '../Extension/' }
			'mwasplund|Soup.C': { Version: 0.4.0 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}