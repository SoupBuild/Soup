using Microsoft.Extensions.DependencyInjection;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;

namespace Soup.Build.Generate
{
	class Program
	{
		

		static IEnumerable<IBuildTask> CreateCommands(Assembly assembly)
		{
			var serviceCollection = new ServiceCollection();
			var activeState = new ValueTable();
			var sharedState = new ValueTable();
			serviceCollection.AddSingleton(typeof(IBuildState), new BuildState(activeState, sharedState));
			var serviceProvider = serviceCollection.BuildServiceProvider();

			foreach (Type type in assembly.GetTypes())
			{
				if (type.IsClass &&
					type.IsPublic &&
					!type.IsAbstract &&
					typeof(IBuildTask).IsAssignableFrom(type))
				{
					var result = (IBuildTask)ActivatorUtilities.CreateInstance(serviceProvider, type);
					yield return result;
				}
			}
		}

		static void Main(string[] args)
		{
			Console.WriteLine("Hello World!");

			string[] pluginPaths = new string[]
			{
				"Soup.Cpp/bin/Debug/net5.0/Soup.Cpp.dll"
			};

			IEnumerable<IBuildTask> commands = pluginPaths.SelectMany(pluginPath =>
			{
				Assembly pluginAssembly = LoadPlugin(pluginPath);
				return CreateCommands(pluginAssembly);
			}).ToList();

			var command = commands.FirstOrDefault();
			if (command == null)
			{
				Console.WriteLine("No such command is known.");
				return;
			}

			command.Execute();
		}
	}
}
