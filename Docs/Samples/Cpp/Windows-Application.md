# C++ Windows Application
This is a windows application that demonstrates creating a GUI windows application.

[Source](https://github.com/SoupBuild/Soup/tree/main/Samples/Cpp/WindowsApplication)

## Recipe.sml
The Recipe file that defines the sample application.
```
Name: 'Samples.Cpp.WindowsApplication'
Language: (C++@0)
Type: 'Windows'
Version: 1.0.0
Resources: 'WindowsProject.rc'
```

## PackageLock.sml
The package lock that was generated to capture the unique dependencies required to build this project.
```sml
Version: 5
Closures: {
	Root: {
		'C++': {
			'Samples.Cpp.WindowsApplication': { Version: '../WindowsApplication', Build: 'Build0', Tool: 'Tool0' }
		}
	}
	Build0: {
		Wren: {
			'Soup|Cpp': { Version: 0.13.2 }
		}
	}
	Tool0: {
		'C++': {
			'mwasplund|copy': { Version: 1.1.0 }
			'mwasplund|mkdir': { Version: 1.1.0 }
		}
	}
}
```

## Content
The remaining content assumes that you are familiar with creating a windows applications. This is not meant as a windows tutorial and is only present as a sample of a more complex scenario for building within Soup.