
using Opal;
using Opal.System;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
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
            // Register the test mocks
            var testListener = new TestTraceListener();
            var processManager = new MockProcessManager();
            using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
            using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
            {

                // Register the mock compiler
                var compiler = new Mock.Compiler();

                // Register the test process manager

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

                // Verify expected logs
                Assert.Equal(
                    new List<string>()
                    {
                    },
                    testListener.GetMessages());

                // Verify expected process requests
                Assert.Equal(
                    new List<string>()
                    {
                        "GetCurrentProcessFileName",
                        "GetCurrentProcessFileName",
                    },
                    processManager.GetRequests());

                var expectedCompileArguments = new CompileArguments()
                {
                    ObjectDirectory = new Path("obj/"),
                    RootDirectory = new Path("C:/root/"),
                    SourceFiles = new List<Path>()
                    {
                        new Path("TestFile.cs"),
                    },
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
                        new Path("C:/Program Files/SoupBuild/Soup/mkdir.exe"),
                        "\"./obj/\"",
                        new List<Path>(),
                        new List<Path>()
                        {
                            new Path("./obj/"),
                        }),
                    new BuildOperation(
                        "MakeDir [./bin/]",
                        new Path("C:/root/"),
                        new Path("C:/Program Files/SoupBuild/Soup/mkdir.exe"),
                        "\"./bin/\"",
                        new List<Path>(),
                        new List<Path>()
                        {
                            new Path("./bin/"),
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
                    result.BuildOperations);

                Assert.Equal(
                    new List<Path>(),
                    result.LinkDependencies);

                Assert.Equal(
                    new List<Path>()
                    {
                    },
                    result.RuntimeDependencies);
            }
        }

        [Fact]
        public void Build_Library_MultipleFiles()
        {
            // Register the test mocks
            var testListener = new TestTraceListener();
            var processManager = new MockProcessManager();
            using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
            using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
            {
                // Register the mock compiler
                var compiler = new Mock.Compiler();

                // Setup the build arguments
                var arguments = new BuildArguments();
                arguments.TargetName = "Library";
                arguments.TargetType = BuildTargetType.StaticLibrary;
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

                var uut = new BuildEngine(compiler);
                var fileSystemState = new Runtime.FileSystemState();
                var buildState = new BuildState(new ValueTable(), fileSystemState);
                var result = uut.Execute(buildState, arguments);

                // Verify expected logs
                Assert.Equal(
                    new List<string>()
                    {
                    },
                    testListener.GetMessages());

                // Verify expected process requests
                Assert.Equal(
                    new List<string>()
                    {
                        "GetCurrentProcessFileName",
                        "GetCurrentProcessFileName",
                    },
                    processManager.GetRequests());

                // Setup the shared arguments
                var expectedCompileArguments = new CompileArguments()
                {
                    RootDirectory = new Path("C:/root/"),
                    ObjectDirectory = new Path("obj/"),
                    SourceFiles = new List<Path>()
                    {
                        new Path("TestFile1.cs"),
                        new Path("TestFile2.cs"),
                        new Path("TestFile3.cs"),
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
                        new Path("C:/Program Files/SoupBuild/Soup/mkdir.exe"),
                        "\"./obj/\"",
                        new List<Path>(),
					    new List<Path>()
                        {
                            new Path("./obj/"),
					    }),
				    new BuildOperation(
                        "MakeDir [./bin/]",
                        new Path("C:/root/"),
                        new Path("C:/Program Files/SoupBuild/Soup/mkdir.exe"),
                        "\"./bin/\"",
                        new List<Path>(),
					    new List<Path>()
                        {
                            new Path("./bin/"),
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
				    },
				    result.LinkDependencies);

                Assert.Equal(
                    new List<Path>(),
                    result.RuntimeDependencies);

                Assert.Equal(
                    new Path(),
                    result.TargetFile);
            }
        }
    }
}