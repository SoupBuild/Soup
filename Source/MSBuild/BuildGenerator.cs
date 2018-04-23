﻿// <copyright company="Soup" file="BuildGenerator.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System;
	using System.Collections.Generic;
	using System.IO;
	using System.Text;
	using System.Xml.Serialization;

	/// <summary>
	/// The MSBuild generator
	/// </summary>
	public class BuildGenerator : IBuildGenerator
	{
		public string Name => "MSBuild";

		public void GenerateBuild(Recipe recipe, string packageDirectory, string targetDirectory)
		{
			var includeItems = new List<Item>();
			var sourceItems = new List<Item>();
			foreach (var file in PackageManager.FindSourceFiles(recipe, packageDirectory))
			{
				var filePath = $"$(PackageRoot)\\{file}";
				switch (Path.GetExtension(file))
				{
					case ".h":
						includeItems.Add(new ClIncludeItem(filePath));
						break;
					case ".cpp":
						sourceItems.Add(new ClCompileItem(filePath));
						break;
					default:
						throw new InvalidOperationException("A source file with unknown extension.");
				}
			}

			var project = CreateVS15LibraryTemplate(recipe, includeItems, sourceItems);

			var projectFileName = MSBuildConstants.VS2017ProjectName;
			var projectFilePath = Path.Combine(targetDirectory, projectFileName);
			using (var stream = new StreamWriter(File.Create(projectFilePath), Encoding.UTF8))
			{
				XmlSerializer projectSerializer = new XmlSerializer(
					typeof(Project),
					"http://schemas.microsoft.com/developer/msbuild/2003");
				projectSerializer.Serialize(stream, project, project.Namespaces);
			}
		}

		public void GenerateDependencies(
			Recipe recipe,
			string packageDirectory,
			string targetDirectory,
			string binaryDirectory,
			string objectDirectory)
		{
			var targetName = string.Format(
				Constants.LibraryTargetNameFormat,
				recipe.Name,
				recipe.Version,
				"$(Platform)",
				"$(Configuration)");

			var dependencyImports = new List<Import>();
			foreach (var dependency in recipe.Dependencies)
			{
				var dependencyBuildPath = PackageManager.BuildKitchenBuildPath("MSBuild", dependency);
				var dependencyIncludeFile = Path.Combine(dependencyBuildPath, MSBuildConstants.PackageIncludeFileName);

				dependencyImports.Add(new Import(dependencyIncludeFile));
			}

			var project = new Project()
			{
				Elements = new List<ProjectElement>()
				{
					new PropertyGroup()
					{
						Label = "PackageBuild",
						Properties = new List<Property>()
						{
							new Property("TargetName", targetName),
							new Property("PackageRoot", packageDirectory),
							new Property("BinDir", $"{binaryDirectory}\\"),
							new Property("ObjDir", $"{objectDirectory}\\"),
							new Property("IntermediateOutputPath", @"$(ObjDir)$(Platform)\$(Configuration)\"),
							new Property("OutputPath", @"$(BinDir)"),
							new Property("OutDir", @"$(OutputPath)"),
						},
					},
					new ImportGroup()
					{
						Label = "PackageDependencies",
						Imports = dependencyImports,
					}
				}
			};

			var propertiesFilePath = Path.Combine(targetDirectory, MSBuildConstants.PackagePropertiesFileName);
			using (var stream = new StreamWriter(File.Create(propertiesFilePath), Encoding.UTF8))
			{
				XmlSerializer projectSerializer = new XmlSerializer(
					typeof(Project),
					"http://schemas.microsoft.com/developer/msbuild/2003");
				projectSerializer.Serialize(stream, project, project.Namespaces);
			}
		}

		public void GenerateInclude(
			Recipe recipe,
			string targetDirectory,
			string includeDirectory)
		{
			var dependencies = new List<string>()
			{
				"%(AdditionalIncludeDirectories)"
			};

			foreach (var dependency in recipe.Dependencies)
			{
				var libraryName = string.Format(
					Constants.LibraryTargetNameFormat,
					recipe.Name,
					recipe.Version,
					"$(Configuration)",
					"$(Platform)");
				dependencies.Add($"{libraryName}.lib");
			}

			var project = new Project()
			{
				Elements = new List<ProjectElement>()
				{
					new ItemDefinitionGroup()
					{
						ClCompile = new ClCompile()
						{
							AdditionalIncludeDirectories = new AdditionalIncludeDirectories()
							{
								Includes = new List<string>()
								{
									"%(AdditionalIncludeDirectories)",
									includeDirectory,
								},
							},
						},
						Link = new Link()
						{
							AdditionalDependencies = new AdditionalDependencies()
							{
								Includes = dependencies,
							},
						},
					},
				}
			};

			// Ensure the target directory exists
			Directory.CreateDirectory(targetDirectory);

			// Create the file
			var propertiesFilePath = Path.Combine(targetDirectory, MSBuildConstants.PackageIncludeFileName);
			using (var stream = new StreamWriter(File.Create(propertiesFilePath), Encoding.UTF8))
			{
				XmlSerializer projectSerializer = new XmlSerializer(
					typeof(Project),
					"http://schemas.microsoft.com/developer/msbuild/2003");
				projectSerializer.Serialize(stream, project, project.Namespaces);
			}
		}

		private Project CreateVS15LibraryTemplate(Recipe recipe, List<Item> includeItems, List<Item> sourceItems)
		{
			return new Project()
			{
				DefaultTargets = "Build",
				ToolsVersion = "15.0",
				Elements = new List<ProjectElement>()
				{
					new ItemGroup()
					{
						Label = "ProjectConfigurations",
						Items = new List<Item>()
						{
							new ProjectConfiguration("Debug", "x64"),
							new ProjectConfiguration("Release", "x64"),
						},
					},
					new PropertyGroup()
					{
						Label = "Globals",
						Properties = new List<Property>()
						{
							new Property("VCProjectVersion", "15.0"),
							new Property("ProjectGuid", Guid.NewGuid().ToString("B").ToUpper()),
							new Property("Keyword", "Win32Proj"),
							new Property("RootNamespace", recipe.Name),
							new Property("WindowsTargetPlatformVersion", "10.0.16299.0"),
						},
					},
					new Import(MSBuildConstants.PackagePropertiesFileName),
					new Import(@"$(VCTargetsPath)\Microsoft.Cpp.Default.props"),
					new PropertyGroup()
					{
						Label = "Configuration",
						Properties = new List<Property>()
						{
							new Property("ConfigurationType", "StaticLibrary"),
							new Property("PlatformToolset", "v141"),
							new Property("CharacterSet", "Unicode"),
						},
					},
					new PropertyGroup()
					{
						Condition = "'$(Configuration)'=='Debug'",
						Label = "Configuration",
						Properties = new List<Property>()
						{
							new Property("UseDebugLibraries", "true"),
						},
					},
					new PropertyGroup()
					{
						Condition = "'$(Configuration)'=='Release'",
						Label = "Configuration",
						Properties = new List<Property>()
						{
							new Property("UseDebugLibraries", "false"),
							new Property("WholeProgramOptimization", "true"),
						},
					},
					new Import(@"$(VCTargetsPath)\Microsoft.Cpp.props"),
					new ImportGroup()
					{
						Label = "ExtensionSettings",
						Imports = new List<Import>(),
					},
					new ImportGroup()
					{
						Label = "Shared",
						Imports = new List<Import>(),
					},
					new ImportGroup()
					{
						Label = "PropertySheets",
						Imports = new List<Import>()
						{
							new Import()
							{
								Project = @"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props",
								Label = "LocalAppDataPlatform",
								Condition = @"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')",
							},
						},
					},
					new PropertyGroup()
					{
						Label = "UserMacros",
						Properties = new List<Property>(),
					},
					new PropertyGroup()
					{
						Condition = "'$(Configuration)'=='Debug'",
						Properties = new List<Property>()
						{
							new Property("LinkIncremental", "true"),
						},
					},
					new PropertyGroup()
					{
						Condition = "'$(Configuration)'=='Release'",
						Properties = new List<Property>()
						{
							new Property("LinkIncremental", "false"),
						},
					},
					new ItemDefinitionGroup()
					{
						ClCompile = new ClCompile()
						{
							// PrecompiledHeader = "Use",
							WarningLevel = "Level4",
							SDLCheck = true,
							PreprocessorDefinitions = "_LIB;%(PreprocessorDefinitions)",
							ConformanceMode = true,
						},
						Link = new Link()
						{
							SubSystem = "Windows",
							GenerateDebugInformation = true,
						},
					},
					new ItemDefinitionGroup()
					{
						Condition = "'$(Configuration)'=='Debug'",
						ClCompile = new ClCompile()
						{
							Optimization = "Disabled",
							PreprocessorDefinitions = "_DEBUG;%(PreprocessorDefinitions)",
						},
						Link = new Link(),
					},
					new ItemDefinitionGroup()
					{
						Condition = "'$(Configuration)'=='Release'",
						ClCompile = new ClCompile()
						{
							Optimization = "MaxSpeed",
							FunctionLevelLinking = true,
							IntrinsicFunctions = true,
							PreprocessorDefinitions = "NDEBUG;%(PreprocessorDefinitions)",
						},
						Link = new Link()
						{
							EnableCOMDATFolding = true,
							OptimizeReferences = true,
						},
					},
					new ItemGroup()
					{
						Items = includeItems,
					},
					new ItemGroup()
					{
						Items = sourceItems,
					},
					new Import(@"$(VCTargetsPath)\Microsoft.Cpp.targets"),
					new ImportGroup()
					{
						Label = "ExtensionTargets",
						Imports = new List<Import>(),
					},
				}
			};
		}
	}
}
