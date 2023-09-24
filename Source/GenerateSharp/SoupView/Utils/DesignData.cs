
using Soup.Build.Utilities;
using Opal;
using Soup.Build;

namespace Soup.View.ViewModels;

public static class DesignData
{
	public static DependencyGraphViewModel ExampleDependencyGraph { get; } =
		new DependencyGraphViewModel()
		{
			IsMenuOpen = true,
			SelectedProject = new ProjectDetailsViewModel(
				new Recipe("TestProject", new LanguageReference("C++", new SemanticVersion(1)))
				{
					Version = new SemanticVersion(1, 2, 3),
				},
				new Path("C:/Test/Project/Recipe.sml")),
		};
}