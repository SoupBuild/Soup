// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml.Serialization;

namespace Soup.VisualStudioBuild
{
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

			var projectFileName = Constants.VS2017ProjectName;
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
			string outDir)
		{
			var userConfig = Singleton<LocalUserConfig>.Instance;
			var dependencyImports = new List<Import>();
			var dependencyReferences = new List<Item>();
			foreach (var dependency in recipe.Dependencies)
			{
				var dependencyFolder = PackageManager.BuildPackageVersionPath(dependency.Name, dependency.Version);
				var dependencyBuildPath = Path.Combine(userConfig.PackageStore, dependencyFolder, Constants.BuildFolderName);
				var dependencyIncludefile = Path.Combine(dependencyBuildPath, Constants.PackageIncludeFileName);
				var dependencyProjectfile = Path.Combine(dependencyBuildPath, Constants.VS2017ProjectName);

				dependencyImports.Add(new Import(dependencyIncludefile));
				dependencyReferences.Add(new ProjectReference(dependencyProjectfile));
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
							new Property("PackageRoot", packageDirectory),
							new Property("BinDir", $"{outDir}bin\\"),
							new Property("ObjDir", $"{outDir}obj\\"),
							new Property("IntermediateOutputPath", @"$(ObjDir)$(Platform)\$(Configuration)\"),
							new Property("OutputPath", @"$(BinDir)$(Platform)\$(Configuration)\"),
							new Property("OutDir", @"$(OutputPath)"),
						},
					},
					new ImportGroup()
					{
						Label = "PackageDependenciesIncludes",
						Imports = dependencyImports,
					},
					new ItemGroup()
					{
						Label = "PackageDependenciesReferences",
						Items = dependencyReferences,
					},
				}
			};

			var propertiesFilePath = Path.Combine(targetDirectory, Constants.PackagePropertiesFileName);
			using (var stream = new StreamWriter(File.Create(propertiesFilePath), Encoding.UTF8))
			{
				XmlSerializer projectSerializer = new XmlSerializer(
					typeof(Project),
					"http://schemas.microsoft.com/developer/msbuild/2003");
				projectSerializer.Serialize(stream, project, project.Namespaces);
			}
		}

		public void GenerateInclude(Recipe recipe, string packageDirectory, string targetDirectory)
		{
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
									"$(MSBuildThisFileDirectory)",
								},
							},
						},
					},
				}
			};

			// Ensure the target directory exists
			Directory.CreateDirectory(targetDirectory);
			
			// Create the file
			var propertiesFilePath = Path.Combine(targetDirectory, Constants.PackageIncludeFileName);
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
					new Import(Constants.PackagePropertiesFileName),
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
