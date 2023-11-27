
using Opal;
using Soup.Build.Utilities;
using System.Collections.Generic;

namespace Soup.View.ViewModels;

public static class DesignData
{
	public static DependencyGraphViewModel ExampleDependencyGraph { get; } =
		new DependencyGraphViewModel()
		{
			SelectedProject = new ProjectDetailsViewModel(
				"TestProject",
				new Path("C:/Test/Project/Recipe.sml")),
		};

	public static TaskGraphViewModel ExampleTaskGraph { get; } =
		new TaskGraphViewModel()
		{
			SelectedTask = new TaskDetailsViewModel(
				new ValueTable()),
		};

	public static OperationGraphViewModel ExampleOperationGraph { get; } =
		new OperationGraphViewModel()
		{
			SelectedOperation = new OperationDetailsViewModel(
				new FileSystemState(),
				new OperationInfo(
					new OperationId(123),
					"Operation123",
					new CommandInfo(),
					new List<FileId>(),
					new List<FileId>(),
					new List<FileId>(),
					new List<FileId>()),
				null),
		};
}