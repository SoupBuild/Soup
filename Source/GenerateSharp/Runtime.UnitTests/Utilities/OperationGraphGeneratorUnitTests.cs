// <copyright file="OperationGraphGeneratorUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Runtime.UnitTests
{
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
					"DIAG: Read Access Subset:",
					"DIAG: Write Access Subset:",
				},
				testListener.GetMessages());
		}

		[Fact]
		public void CreateOperation_NoAccess_ReadAccessDenied()
		{
			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>();
			var writeAccessList = new List<Path>();
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			Assert.Throws<InvalidOperationException>(
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
		}

		[Fact]
		public void CreateOperation_NoAccess_WriteAccessDenied()
		{
			var fileSystemState = new FileSystemState();
			var readAccessList = new List<Path>();
			var writeAccessList = new List<Path>();
			var uut = new OperationGraphGenerator(fileSystemState, readAccessList, writeAccessList);

			Assert.Throws<InvalidOperationException>(
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
		}

		[Fact]
		public void CreateOperation_Access_ReadAccessDenied()
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

			Assert.Throws<InvalidOperationException>(
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
		}

		[Fact]
		public void CreateOperation_Access_WriteAccessDenied()
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

			Assert.Throws<InvalidOperationException>(
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
					"DIAG: Allow C:/WorkingDir/ReadAccess/ : C:/WorkingDir/ReadAccess/ReadFile.txt",
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
					"DIAG: Read Access Subset:",
					"DIAG: Allow C:/WorkingDir/WriteAccess/ : C:/WorkingDir/WriteAccess/WriteFile.txt",
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
					"DIAG: Allow C:/WorkingDir/ReadAccess/ : C:/WorkingDir/ReadAccess/SubFolder/ReadFile.txt",
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
					"DIAG: Read Access Subset:",
					"DIAG: Allow C:/WorkingDir/WriteAccess/ : C:/WorkingDir/WriteAccess/SubFolder/WriteFile.txt",
					"DIAG: Write Access Subset:",
					"DIAG: C:/WorkingDir/WriteAccess/",
				},
				testListener.GetMessages());
		}
	}
}
