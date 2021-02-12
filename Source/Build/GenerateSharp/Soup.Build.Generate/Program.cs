using Soup.Build;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;

namespace Soup.Build.Generate
{
    class Program
    {
        static Assembly LoadPlugin(string relativePath)
        {
            // Navigate up to the solution root
            string root = Path.GetFullPath(Path.Combine(
                Path.GetDirectoryName(
                    Path.GetDirectoryName(
                        Path.GetDirectoryName(
                            Path.GetDirectoryName(
                                Path.GetDirectoryName(typeof(Program).Assembly.Location)))))));

            string pluginLocation = Path.GetFullPath(Path.Combine(root, relativePath.Replace('\\', Path.DirectorySeparatorChar)));
            Console.WriteLine($"Loading commands from: {pluginLocation}");
            ExtensionLoadContext loadContext = new ExtensionLoadContext(pluginLocation);
            return loadContext.LoadFromAssemblyName(new AssemblyName(Path.GetFileNameWithoutExtension(pluginLocation)));
        }

        static IEnumerable<IBuildTask> CreateCommands(Assembly assembly)
        {
            foreach (Type type in assembly.GetTypes())
            {
                if (type.IsClass &&
                    type.IsPublic &&
                    !type.IsAbstract &&
                    typeof(IBuildTask).IsAssignableFrom(type))
                {
                    var result = (IBuildTask)Activator.CreateInstance(type);
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
