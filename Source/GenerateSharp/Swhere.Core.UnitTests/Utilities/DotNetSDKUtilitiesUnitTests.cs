// <copyright file="DotNetSDKUtilitiesUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Discover.UnitTests
{
	[Collection("Opal")]
	public class DotNetSDKUtilitiesUnitTests
	{
		[Fact]
		public void FindDotNet6Refs()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			mockFileSystem.RegisterDirectoryChildren(
				new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/"),
				new List<Path>()
				{
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/5.0.0"),
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.7"),
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.8"),
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.9"),
				});

			var result = DotNetSDKUtilities.FindDotNet6Refs();

			Assert.Equal("6.0.9", result.Version);
			Assert.Equal(new Path("C:/Program Files/dotnet/"), result.Path);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"HIGH: FindNewestDotNet6RuntimeVersion: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
					"INFO: CheckFile: 5.0.0",
					"INFO: CheckFile: 6.0.7",
					"INFO: CheckFile: 6.0.8",
					"INFO: CheckFile: 6.0.9",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetDirectoryChildren: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
				},
				mockFileSystem.GetRequests());
		}

		[Fact]
		public void FindDotNet6Refs_SkipReleaseCandidate()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			mockFileSystem.RegisterDirectoryChildren(
				new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/"),
				new List<Path>()
				{
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/5.0.0"),
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.7"),
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.8"),
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/6.0.9"),
					new Path("C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/7.0.0-rc.1.22426.10"),
				});

			var result = DotNetSDKUtilities.FindDotNet6Refs();

			Assert.Equal("6.0.9", result.Version);
			Assert.Equal(new Path("C:/Program Files/dotnet/"), result.Path);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"HIGH: FindNewestDotNet6RuntimeVersion: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
					"INFO: CheckFile: 5.0.0",
					"INFO: CheckFile: 6.0.7",
					"INFO: CheckFile: 6.0.8",
					"INFO: CheckFile: 6.0.9",
					"INFO: CheckFile: 7.0.0-rc.1.22426.10",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetDirectoryChildren: C:/Program Files/dotnet/packs/Microsoft.NETCore.App.Ref/",
				},
				mockFileSystem.GetRequests());
		}
	}
}
