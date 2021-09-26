// <copyright file="BuildTaskUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.CSharp.Compiler;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.CSharp.UnitTests
{
	public class BuildTaskUnitTests
	{
		[Fact]
		public void Initialize_Success()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				var buildState = new MockBuildState();
				var factory = new ValueFactory();
				var uut = new BuildTask(buildState, factory);
			}
		}

		[Fact]
		public void Build_Executable()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Setup the input build state
				var buildState = new MockBuildState();
				var state = buildState.ActiveState;

				// Setup build table
				var buildTable = new ValueTable();
				state.Add("Build", new Value(buildTable));
				buildTable.Add("TargetName", new Value("Program"));
				buildTable.Add("TargetType", new Value((long)BuildTargetType.Executable));
				buildTable.Add("WorkingDirectory", new Value("C:/root/"));
				buildTable.Add("ObjectDirectory", new Value("obj/"));
				buildTable.Add("BinaryDirectory", new Value("bin/"));
				buildTable.Add(
					"Source",
					new Value(new ValueList()
						{
							new Value("TestFile.cs"),
						}));

				// Setup parameters table
				var parametersTable = new ValueTable();
				state.Add("Parameters", new Value(parametersTable));
				parametersTable.Add("Architecture", new Value("x64"));
				parametersTable.Add("Compiler", new Value("MOCK"));

				// Register the mock compiler
				var compiler = new Compiler.Mock.Compiler();
				var compilerFactory = new Dictionary<string, Func<IValueTable, ICompiler>>();
				compilerFactory.Add("MOCK", (IValueTable state) => { return compiler; });

				var factory = new ValueFactory();
				var uut = new BuildTask(buildState, factory, compilerFactory);

				uut.Execute();

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Build Generate Done"
					},
					testListener.GetMessages());

				var expectedCompileArguments = new CompileArguments()
				{
					Target = new Path("./bin/Program.mock.dll"),
					ReferenceTarget = new Path("./bin/ref/Program.mock.dll"),
					TargetType = LinkTarget.Executable,
					RootDirectory = new Path("C:/root/"),
					ObjectDirectory = new Path("obj/"),
					SourceFiles = new List<Path>()
					{
						new Path("TestFile.cs")
					},
				};

				// Verify expected compiler calls
				Assert.Equal(
					new List<CompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());

				// Verify build state
				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/root/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/root/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/ref/]",
						new Path("C:/root/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/ref/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/ref/"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("./InputFile.in"),
						},
						new List<Path>()
						{
							new Path("./OutputFile.out"),
						}),
					new BuildOperation(
						"WriteFile [./bin/Program.runtimeconfig.json]",
						new Path("C:/root/"),
						new Path("./writefile.exe"),
						@"""./bin/Program.runtimeconfig.json"" ""{
  ""runtimeOptions"": {
    ""tfm"": ""net5.0"",
    ""framework"": {
      ""name"": ""Microsoft.NETCore.App"",
      ""version"": ""5.0.0""
    }
  }
}""",
				new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/Program.runtimeconfig.json"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					buildState.GetBuildOperations());
			}
		}

		[Fact]
		public void Build_Library_MultipleFiles()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Setup the input build state
				var buildState = new MockBuildState();
				var state = buildState.ActiveState;

				// Setup build table
				var buildTable = new ValueTable();
				state.Add("Build", new Value(buildTable));
				buildTable.Add("TargetName", new Value("Library"));
				buildTable.Add("TargetType", new Value((long)BuildTargetType.Library));
				buildTable.Add("WorkingDirectory", new Value("C:/root/"));
				buildTable.Add("ObjectDirectory", new Value("obj/"));
				buildTable.Add("BinaryDirectory", new Value("bin/"));
				buildTable.Add("Source", new Value(new ValueList()
				{
					new Value("TestFile1.cpp"),
					new Value("TestFile2.cpp"),
					new Value("TestFile3.cpp"),
				}));
				buildTable.Add("IncludeDirectories", new Value(new ValueList()
				{
					new Value("Folder"),
					new Value("AnotherFolder/Sub"),
				}));
				buildTable.Add("ModuleDependencies", new Value(new ValueList()
				{
					new Value("../Other/bin/OtherModule1.mock.bmi"),
					new Value("../OtherModule2.mock.bmi"),
				}));
				buildTable.Add("OptimizationLevel", new Value((long)BuildOptimizationLevel.None));

				// Setup parameters table
				var parametersTable = new ValueTable();
				state.Add("Parameters", new Value(parametersTable));
				parametersTable.Add("Architecture", new Value("x64"));
				parametersTable.Add("Compiler", new Value("MOCK"));

				// Register the mock compiler
				var compiler = new Compiler.Mock.Compiler();
				var compilerFactory = new Dictionary<string, Func<IValueTable, ICompiler>>();
				compilerFactory.Add("MOCK", (IValueTable state) => { return compiler; });

				var factory = new ValueFactory();
				var uut = new BuildTask(buildState, factory, compilerFactory);

				uut.Execute();

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Build Generate Done",
					},
					testListener.GetMessages());

				// Setup the shared arguments
				var expectedCompileArguments = new CompileArguments()
				{
					Target = new Path("./bin/Library.mock.dll"),
					ReferenceTarget = new Path("./bin/ref/Library.mock.dll"),
					RootDirectory = new Path("C:/root/"),
					ObjectDirectory = new Path("obj/"),
					SourceFiles = new List<Path>()
					{
						new Path("TestFile1.cpp"),
						new Path("TestFile2.cpp"),
						new Path("TestFile3.cpp"),
					},
				};

				// Verify expected compiler calls
				Assert.Equal(
					new List<CompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());

				// Verify build state
				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/root/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/root/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/ref/]",
						new Path("C:/root/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/ref/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/ref/"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("./InputFile.in"),
						},
						new List<Path>()
						{
							new Path("./OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					buildState.GetBuildOperations());
			}
		}
	}
}
