
using Opal;
using Opal.System;
using Soup.Build.Runtime;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.CSharp.Compiler.UnitTests
{
	public class BuildEngineUnitTests
	{
		[Fact]
		public void Initialize_Success()
		{
			var compiler = new Mock.Compiler();
			var uut = new BuildEngine(compiler);
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
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Program";
				arguments.TargetType = BuildTargetType.Executable;
				arguments.WorkingDirectory = new Path("C:/root/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.SourceFiles = new List<Path>()
				{
					new Path("TestFile.cs"),
				};
				arguments.OptimizationLevel = BuildOptimizationLevel.None;
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				var uut = new BuildEngine(compiler);
				var fileSystemState = new FileSystemState();
				var buildState = new BuildState(new ValueTable(), fileSystemState);
				var result = uut.Execute(buildState, arguments);

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
					},
					testListener.GetMessages());

				var expectedCompileArguments = new CompileArguments()
				{
					Target = new Path("./bin/Program.mock.dll"),
					ReferenceTarget = new Path("./bin/ref/Program.mock.dll"),
					TargetType = LinkTarget.Executable,
					ObjectDirectory = new Path("obj/"),
					RootDirectory = new Path("C:/root/"),
					SourceFiles = new List<Path>()
					{
						new Path("TestFile.cs"),
					},
					ReferenceLibraries = new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.a"),
						new Path("../OtherModule2.mock.a"),
					},
					NullableState = NullableState.Enabled,
			};

				// Verify expected compiler calls
				var val = compiler.GetCompileRequests()[0];
				var areEqual = val == expectedCompileArguments;
				var areEqual2 = val.ObjectDirectory == expectedCompileArguments.ObjectDirectory;
				Assert.Equal(
					new List<CompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());

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
					result.BuildOperations);

				Assert.Equal(
					new List<Path>(),
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("./bin/Program.mock.dll"),
					},
					result.RuntimeDependencies);
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
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Library";
				arguments.TargetType = BuildTargetType.Library;
				arguments.WorkingDirectory = new Path("C:/root/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.SourceFiles = new List<Path>()
				{
					new Path("TestFile1.cs"),
					new Path("TestFile2.cs"),
					new Path("TestFile3.cs"),
				};
				arguments.OptimizationLevel = BuildOptimizationLevel.Size;
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};
				arguments.NullableState = BuildNullableState.Disabled;

				var uut = new BuildEngine(compiler);
				var fileSystemState = new Runtime.FileSystemState();
				var buildState = new BuildState(new ValueTable(), fileSystemState);
				var result = uut.Execute(buildState, arguments);

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
						new Path("TestFile1.cs"),
						new Path("TestFile2.cs"),
						new Path("TestFile3.cs"),
					},
					ReferenceLibraries = new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.a"),
						new Path("../OtherModule2.mock.a"),
					},
					NullableState = NullableState.Disabled,
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
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					result.BuildOperations);

				Assert.Equal(
					new List<Path>()
					{
						new Path("bin/ref/Library.mock.dll"),
					},
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("bin/Library.mock.dll"),
					},
					result.RuntimeDependencies);

				Assert.Equal(
					new Path("bin/Library.mock.dll"),
					result.TargetFile);
			}
		}
	}
}