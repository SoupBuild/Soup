// <copyright file="OperationGraphGeneratorUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Runtime.UnitTests
{
	[Collection("Opal")]
	public class OperationGraphGeneratorUnitTests
	{
		[Fact]
		public void CreateOperation_NoInputOrOutput()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>();
			var writeAccessList = new List<Path>();
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff",
				new Path("DoStuff.exe"),
				"do stuff",
				new Path("C:/WorkingDir/"),
				new List<Path>(),
				new List<Path>());

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"DIAG: Write Access Subset:",
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_NoAccess_ReadAccessDenied()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>();
			var writeAccessList = new List<Path>();
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			var exception = Assert.Throws<InvalidOperationException>(
				() =>
				{
					uut.CreateOperation(
						"Do Stuff",
						new Path("DoStuff.exe"),
						"do stuff",
						new Path("C:/WorkingDir/"),
						new List<Path>()
						{
							new Path("ReadFile.txt"),
						},
						new List<Path>());
				});
			Assert.Equal("Operation does not have permission to read requested input.", exception.Message);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"ERRO: File access denied: ./ReadFile.txt",
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_NoAccess_WriteAccessDenied()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>();
			var writeAccessList = new List<Path>();
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			var exception = Assert.Throws<InvalidOperationException>(
				() =>
				{
					uut.CreateOperation(
						"Do Stuff",
						new Path("DoStuff.exe"),
						"do stuff",
						new Path("C:/WorkingDir/"),
						new List<Path>(),
						new List<Path>()
						{
							new Path("WriteFile.txt"),
						});
				});
			Assert.Equal("Operation does not have permission to write requested output.", exception.Message);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"ERRO: File access denied: ./WriteFile.txt",
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_Access_ReadAccessDenied()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/")
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/")
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			var exception = Assert.Throws<InvalidOperationException>(
				() =>
				{
					uut.CreateOperation(
						"Do Stuff",
						new Path("DoStuff.exe"),
						"do stuff",
						new Path("C:/WorkingDir/"),
						new List<Path>()
						{
							new Path("ReadFile.txt"),
						},
						new List<Path>());
				});
			Assert.Equal("Operation does not have permission to read requested input.", exception.Message);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"ERRO: File access denied: ./ReadFile.txt",
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_Access_WriteAccessDenied()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/")
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/")
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			var exception = Assert.Throws<InvalidOperationException>(
				() =>
				{
					uut.CreateOperation(
						"Do Stuff",
						new Path("DoStuff.exe"),
						"do stuff",
						new Path("C:/WorkingDir/"),
						new List<Path>(),
						new List<Path>()
						{
							new Path("WriteFile.txt"),
						});
				});
			Assert.Equal("Operation does not have permission to write requested output.", exception.Message);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"ERRO: File access denied: ./WriteFile.txt",
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_Access_ReadAccessRelative()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/")
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/")
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff",
				new Path("DoStuff.exe"),
				"do stuff",
				new Path("C:/WorkingDir/"),
				new List<Path>()
				{
					new Path("./ReadAccess/ReadFile.txt"),
				},
				new List<Path>());

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"DIAG: C:/WorkingDir/ReadAccess/",
					"DIAG: Write Access Subset:"
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_Access_WriteAccessRelative()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/")
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/")
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff",
				new Path("DoStuff.exe"),
				"do stuff",
				new Path("C:/WorkingDir/"),
				new List<Path>(),
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile.txt"),
				});

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_Access_ReadAccessRelative_SubFolder()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/")
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/")
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff",
				new Path("DoStuff.exe"),
				"do stuff",
				new Path("C:/WorkingDir/"),
				new List<Path>()
				{
					new Path("./ReadAccess/SubFolder/ReadFile.txt"),
				},
				new List<Path>());

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"DIAG: C:/WorkingDir/ReadAccess/",
					"DIAG: Write Access Subset:"
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_Access_WriteAccessRelative_SubFolder()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/")
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/")
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff",
				new Path("DoStuff.exe"),
				"do stuff",
				new Path("C:/WorkingDir/"),
				new List<Path>(),
				new List<Path>()
				{
					new Path("./WriteAccess/SubFolder/WriteFile.txt"),
				});

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_CircularDependencies_ReadWrite()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/"),
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/"),
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			var exception = Assert.Throws<InvalidOperationException>(
				() =>
				{
					uut.CreateOperation(
						"Do Stuff",
						new Path("DoStuff.exe"),
						"do stuff",
						new Path("C:/WorkingDir/"),
						new List<Path>()
						{
							new Path("File.txt"),
						},
						new List<Path>()
						{
							new Path("File.txt"),
						});
				});

			Assert.Equal("", exception.Message);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"DIAG: Write Access Subset:",
				},
				testListener.GetMessages());
		}


		[Fact]
		public void BuildGraph_NoNodes()
		{
			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/")
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/")
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			var graph = uut.BuildGraph();

			Assert.Equal(new Dictionary<OperationId, OperationInfo>(), graph.Operations);
			Assert.Equal(new List<OperationId>(), graph.RootOperationIds);
			Assert.Equal(new List<(FileId, Path)>(), graph.ReferencedFiles);
		}

		[Fact]
		public void BuildGraph_SingleNodes()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/")
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/")
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff",
				new Path("DoStuff.exe"),
				"do stuff",
				new Path("C:/WorkingDir/"),
				new List<Path>()
				{
					new Path("./ReadAccess/ReadFile.txt"),
				},
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile.txt"),
				});

			var graph = uut.BuildGraph();

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff",
					"DIAG: Read Access Subset:",
					"DIAG: C:/WorkingDir/ReadAccess/",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
				},
				testListener.GetMessages());

			Assert.Equal(
				new Dictionary<OperationId, OperationInfo>()
				{
					{ 
						new OperationId(1),
						new OperationInfo()
						{
							Id = new OperationId(1),
							Title = "Do Stuff",
							Command = new CommandInfo()
							{
								Executable = new Path("DoStuff.exe"),
								Arguments = "do stuff",
								WorkingDirectory = new Path("C:/WorkingDir/"),
							},
							DeclaredInput = new List<FileId>() { new FileId(1), },
							DeclaredOutput = new List<FileId>() { new FileId(2), },
							ReadAccess = new List<FileId>() { new FileId(3), },
							WriteAccess = new List<FileId>() { new FileId(4), },
							DependencyCount = 1,
						}
					},
				},
				graph.Operations);
			Assert.Equal(
				new List<OperationId>()
				{
					new OperationId(1),
				},
				graph.RootOperationIds);
			Assert.Equal(
				new List<(FileId, Path)>(),
				graph.ReferencedFiles);
		}

		[Fact]
		public void BuildGraph_DependencyNodes_DirectFileRead()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/"),
				new Path("C:/WorkingDir/WriteAccess/"),
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/"),
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff 1",
				new Path("DoStuff.exe"),
				"do stuff 1",
				new Path("C:/WorkingDir/"),
				new List<Path>()
				{
					new Path("./ReadAccess/ReadFile.txt"),
				},
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile1.txt"),
				});

			uut.CreateOperation(
				"Do Stuff 2",
				new Path("DoStuff.exe"),
				"do stuff 2",
				new Path("C:/WorkingDir/"),
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile1.txt"),
				},
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile2.txt"),
				});

			var graph = uut.BuildGraph();

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff 1",
					"DIAG: Read Access Subset:",
					"DIAG: C:/WorkingDir/ReadAccess/",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
					"DIAG: Create Operation: Do Stuff 2",
					"DIAG: Read Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
				},
				testListener.GetMessages());

			Assert.Equal(
				new Dictionary<OperationId, OperationInfo>()
				{
					{
						new OperationId(1),
						new OperationInfo()
						{
							Id = new OperationId(1),
							Title = "Do Stuff 1",
							Command = new CommandInfo()
							{
								Executable = new Path("DoStuff.exe"),
								Arguments = "do stuff 1",
								WorkingDirectory = new Path("C:/WorkingDir/"),
							},
							DeclaredInput = new List<FileId>() { new FileId(1), },
							DeclaredOutput = new List<FileId>() { new FileId(2), },
							ReadAccess = new List<FileId>() { new FileId(3), },
							WriteAccess = new List<FileId>() { new FileId(4), },
							DependencyCount = 1,
							Children = new List<OperationId>()
							{
								new OperationId(2),
							},
						}
					},
					{
						new OperationId(2),
						new OperationInfo()
						{
							Id = new OperationId(2),
							Title = "Do Stuff 2",
							Command = new CommandInfo()
							{
								Executable = new Path("DoStuff.exe"),
								Arguments = "do stuff 2",
								WorkingDirectory = new Path("C:/WorkingDir/"),
							},
							DeclaredInput = new List<FileId>() { new FileId(2), },
							DeclaredOutput = new List<FileId>() { new FileId(5), },
							ReadAccess = new List<FileId>() { new FileId(4), },
							WriteAccess = new List<FileId>() { new FileId(4), },
							DependencyCount = 1,
						}
					},
				},
				graph.Operations);
			Assert.Equal(
				new List<OperationId>()
				{
					new OperationId(1),
				},
				graph.RootOperationIds);
			Assert.Equal(
				new List<(FileId, Path)>(),
				graph.ReferencedFiles);
		}

		[Fact]
		public void BuildGraph_DependencyNodes_SubFolder()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/"),
				new Path("C:/WorkingDir/WriteAccess/"),
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/"),
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff 1",
				new Path("DoStuff.exe"),
				"do stuff 1",
				new Path("C:/WorkingDir/"),
				new List<Path>(),
				new List<Path>()
				{
					new Path("./WriteAccess/Folder/"),
				});

			uut.CreateOperation(
				"Do Stuff 2",
				new Path("DoStuff.exe"),
				"do stuff 2",
				new Path("C:/WorkingDir/"),
				new List<Path>(),
				new List<Path>()
				{
					new Path("./WriteAccess/Folder/WriteFile1.txt"),
				});

			var graph = uut.BuildGraph();

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff 1",
					"DIAG: Read Access Subset:",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
					"DIAG: Create Operation: Do Stuff 2",
					"DIAG: Read Access Subset:",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
				},
				testListener.GetMessages());

			Assert.Equal(
				new Dictionary<OperationId, OperationInfo>()
				{
					{
						new OperationId(1),
						new OperationInfo()
						{
							Id = new OperationId(1),
							Title = "Do Stuff 1",
							Command = new CommandInfo()
							{
								Executable = new Path("DoStuff.exe"),
								Arguments = "do stuff 1",
								WorkingDirectory = new Path("C:/WorkingDir/"),
							},
							DeclaredInput = new List<FileId>() { },
							DeclaredOutput = new List<FileId>() { new FileId(1), },
							ReadAccess = new List<FileId>() { },
							WriteAccess = new List<FileId>() { new FileId(2), },
							DependencyCount = 1,
							Children = new List<OperationId>()
							{
								new OperationId(2),
							},
						}
					},
					{
						new OperationId(2),
						new OperationInfo()
						{
							Id = new OperationId(2),
							Title = "Do Stuff 2",
							Command = new CommandInfo()
							{
								Executable = new Path("DoStuff.exe"),
								Arguments = "do stuff 2",
								WorkingDirectory = new Path("C:/WorkingDir/"),
							},
							DeclaredInput = new List<FileId>() { },
							DeclaredOutput = new List<FileId>() { new FileId(3), },
							ReadAccess = new List<FileId>() { },
							WriteAccess = new List<FileId>() { new FileId(2), },
							DependencyCount = 1,
						}
					},
				},
				graph.Operations);
			Assert.Equal(
				new List<OperationId>()
				{
					new OperationId(1),
				},
				graph.RootOperationIds);
			Assert.Equal(
				new List<(FileId, Path)>(),
				graph.ReferencedFiles);
		}

		[Fact]
		public void BuildGraph_DependencyNodes_FlattenGraph()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/ReadAccess/"),
				new Path("C:/WorkingDir/WriteAccess/"),
			};
			var writeAccessList = new List<Path>()
			{
				new Path("C:/WorkingDir/WriteAccess/"),
			};
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			uut.CreateOperation(
				"Do Stuff 1",
				new Path("DoStuff.exe"),
				"do stuff 1",
				new Path("C:/WorkingDir/"),
				new List<Path>()
				{
					new Path("./ReadAccess/ReadFile.txt"),
				},
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile1.txt"),
				});

			uut.CreateOperation(
				"Do Stuff 2",
				new Path("DoStuff.exe"),
				"do stuff 2",
				new Path("C:/WorkingDir/"),
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile1.txt"),
				},
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile2.txt"),
				});

			// Create a third operation that reads the output of the other two
			// The graph builder should flatten these references to be dependent on the single second node that already
			// is dependent on the first node.
			uut.CreateOperation(
				"Do Stuff 3",
				new Path("DoStuff.exe"),
				"do stuff 3",
				new Path("C:/WorkingDir/"),
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile1.txt"),
					new Path("./WriteAccess/WriteFile2.txt"),
				},
				new List<Path>()
				{
					new Path("./WriteAccess/WriteFile3.txt"),
				});

			var graph = uut.BuildGraph();

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Create Operation: Do Stuff 1",
					"DIAG: Read Access Subset:",
					"DIAG: C:/WorkingDir/ReadAccess/",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
					"DIAG: Create Operation: Do Stuff 2",
					"DIAG: Read Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
					"DIAG: Create Operation: Do Stuff 3",
					"DIAG: Read Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
				},
				testListener.GetMessages());

			Assert.Equal(
				new Dictionary<OperationId, OperationInfo>()
				{
					{
						new OperationId(1),
						new OperationInfo()
						{
							Id = new OperationId(1),
							Title = "Do Stuff 1",
							Command = new CommandInfo()
							{
								Executable = new Path("DoStuff.exe"),
								Arguments = "do stuff 1",
								WorkingDirectory = new Path("C:/WorkingDir/"),
							},
							DeclaredInput = new List<FileId>() { new FileId(1), },
							DeclaredOutput = new List<FileId>() { new FileId(2), },
							ReadAccess = new List<FileId>() { new FileId(3), },
							WriteAccess = new List<FileId>() { new FileId(4), },
							DependencyCount = 1,
							Children = new List<OperationId>()
							{
								new OperationId(2),
							},
						}
					},
					{
						new OperationId(2),
						new OperationInfo()
						{
							Id = new OperationId(2),
							Title = "Do Stuff 2",
							Command = new CommandInfo()
							{
								Executable = new Path("DoStuff.exe"),
								Arguments = "do stuff 2",
								WorkingDirectory = new Path("C:/WorkingDir/"),
							},
							DeclaredInput = new List<FileId>() { new FileId(2), },
							DeclaredOutput = new List<FileId>() { new FileId(5), },
							ReadAccess = new List<FileId>() { new FileId(4), },
							WriteAccess = new List<FileId>() { new FileId(4), },
							DependencyCount = 1,
							Children = new List<OperationId>()
							{
								new OperationId(3),
							},
						}
					},
					{
						new OperationId(3),
						new OperationInfo()
						{
							Id = new OperationId(3),
							Title = "Do Stuff 3",
							Command = new CommandInfo()
							{
								Executable = new Path("DoStuff.exe"),
								Arguments = "do stuff 3",
								WorkingDirectory = new Path("C:/WorkingDir/"),
							},
							DeclaredInput = new List<FileId>() { new FileId(2), new FileId(5), },
							DeclaredOutput = new List<FileId>() { new FileId(6), },
							ReadAccess = new List<FileId>() { new FileId(4), },
							WriteAccess = new List<FileId>() { new FileId(4), },
							DependencyCount = 1,
						}
					},
				},
				graph.Operations);
			Assert.Equal(
				new List<OperationId>()
				{
					new OperationId(1),
				},
				graph.RootOperationIds);
			Assert.Equal(
				new List<(FileId, Path)>(),
				graph.ReferencedFiles);
		}
	}
}
