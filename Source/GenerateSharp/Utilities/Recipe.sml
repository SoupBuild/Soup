Name: 'Soup.Utilities'
Language: 'C#|0'
Version: 1.0.0
TargetFramework: 'net9.0'
Dependencies: {
	Build: [
		'Soup|CSharp.Nuget@0'
	]
	Runtime: [
		'../Opal/'
	]
}
Nuget: {
	Dependencies: {
		Runtime: [
			{ Name: 'Antlr4.Runtime.Standard', Version: '4.13.1' }
		]
	}
}