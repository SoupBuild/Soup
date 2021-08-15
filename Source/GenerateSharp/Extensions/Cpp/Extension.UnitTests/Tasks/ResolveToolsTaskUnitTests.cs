// <copyright file="ResolveToolsTaskUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System.Collections.Generic;
using System.Text;
using Xunit;

namespace Soup.Build.Cpp.UnitTests
{
	public class ResolveToolsTaskUnitTests
	{
		[Fact]
		public void Initialize_Success()
		{
			var buildState = new MockBuildState();
			var uut = new ResolveToolsTask(buildState);
		}

		[Fact]
		public void Execute()
		{
			// Register the test systems
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			{
				// Setup the input build state
				var buildState = new MockBuildState();
				var state = buildState.ActiveState;

				// Set the sdks
				var sdks = new ValueList();
				sdks.Add(new Value(new ValueTable()
				{
					{ "Name", new Value("MSVC") },
					{ 
						"Properties",
						new Value(new ValueTable()
						{
							{ "Version", new Value("1.0.0") },
							{ "VCToolsRoot", new Value("C:/VCTools/Root/") },
						})
					},
				}));
				sdks.Add(new Value(new ValueTable()
				{
					{ "Name", new Value("Windows") },
					{
						"Properties",
						new Value(new ValueTable()
						{
							{ "Version", new Value("10.0.0") },
							{ "RootPath", new Value("C:/WindowsKit/Root/") },
						})
					},
				}));

				// Setup parameters table
				var parametersTable = new ValueTable();
				state.Add("Parameters", new Value(parametersTable));
				parametersTable.Add("SDKs", new Value(sdks));
				parametersTable.Add("System", new Value("win32"));
				parametersTable.Add("Architecture", new Value("x64"));

				var uut = new ResolveToolsTask(buildState);

				uut.Execute();

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Using VC Version: 1.0.0",
						"INFO: Using Windows Kit Version: 10.0.0",
					},
					testListener.GetMessages());

				// Verify build state
				var expectedBuildOperations = new List<BuildOperation>();

				Assert.Equal(
					expectedBuildOperations,
					buildState.GetBuildOperations());
			}
		}
	}
}
