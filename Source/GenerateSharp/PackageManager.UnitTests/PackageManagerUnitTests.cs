// <copyright file="SwhereManagerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Moq;
using Opal;
using Opal.System;
using Soup.Build.Api.Client;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using Xunit;

namespace Soup.Build.PackageManager.UnitTests
{
	[Collection("Opal")]
	public class PackageManagerUnitTests
	{
		[Fact]
		public async Task RestorePackagesAsync_NoDependencies()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""MyPackage""
					Language: ""C++|0.1""
					Version: ""1.0.0"""))));

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var uut = new PackageManager(httpClient);

			var workingDirectory = new Path("C:/Root/MyPackage/");
			bool forceRestore = false;
			await uut.RestorePackagesAsync(workingDirectory, forceRestore);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"DIAG: Root:C++ MyPackage -> ./",
					"DIAG: Build0:C# Soup.Cpp -> 0.1.0",
					"DIAG: Deleting staging directory",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
				},
				mockFileSystem.GetRequests());

			// Verify http requests
			mockMessageHandler.VerifyNoOtherCalls();

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expected =
@"Version: 3
Closures: {
	Root: {
		Cpp: [
			{ Name: ""MyPackage"", Version: ""./"", Build: ""Build0"" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: ""Soup.Cpp"", Version: ""0.1.0"" }
		]
	}
}";

			Assert.Equal(expected, packageLockContent);
		}

		[Fact]
		public async Task RestorePackagesAsync_HasDependencies()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""MyPackage""
					Language: ""C++|0.1""
					Version: ""1.0.0""
					Dependencies: {
						Runtime: [
							{ Reference: ""Package1@1.2.3"" }
							{ Reference: ""Package2@3.2.1"" }
						]
					}"))));

			mockFileSystem.CreateMockFile(
				new Path("C:/Users/Me/.soup/packages/C++/Package1/1.2.3/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""Package1""
					Language: ""C++|0.1""
					Version: ""1.2.3"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/Users/Me/.soup/packages/C++/Package2/3.2.1/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""Package2""
					Language: ""C++|0.1""
					Version: ""3.2.1"""))));

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/Package1/version/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/Package2/version/3.2.1/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var uut = new PackageManager(httpClient);

			var workingDirectory = new Path("C:/Root/MyPackage/");
			bool forceRestore = false;
			await uut.RestorePackagesAsync(workingDirectory, forceRestore);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"HIGH: Install Package: C++ Package1@1.2.3",
					"HIGH: Downloading package",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/Package1/1.2.3/Recipe.sml",
					"HIGH: Install Package: C++ Package2@3.2.1",
					"HIGH: Downloading package",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/Package2/3.2.1/Recipe.sml",
					"DIAG: Root:C++ MyPackage -> ./",
					"DIAG: Root:C++ Package1 -> 1.2.3",
					"DIAG: Root:C++ Package2 -> 3.2.1",
					"DIAG: Build0:C# Soup.Cpp -> 0.1.0",
					"DIAG: Deleting staging directory",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/Package1/1.2.3/",
					"OpenWriteTruncate: C:/Users/Me/.soup/packages/.staging/Package1.zip",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/C++_Package1_1.2.3/",
					"DeleteFile: C:/Users/Me/.soup/packages/.staging/Package1.zip",
					"Exists: C:/Users/Me/.soup/packages/C++/Package1",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/Package1",
					"Rename: [C:/Users/Me/.soup/packages/.staging/C++_Package1_1.2.3/] -> [C:/Users/Me/.soup/packages/C++/Package1/1.2.3/]",
					"Exists: C:/Users/Me/.soup/packages/C++/Package1/1.2.3/Recipe.sml",
					"OpenRead: C:/Users/Me/.soup/packages/C++/Package1/1.2.3/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/Package2/3.2.1/",
					"OpenWriteTruncate: C:/Users/Me/.soup/packages/.staging/Package2.zip",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/C++_Package2_3.2.1/",
					"DeleteFile: C:/Users/Me/.soup/packages/.staging/Package2.zip",
					"Exists: C:/Users/Me/.soup/packages/C++/Package2",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/Package2",
					"Rename: [C:/Users/Me/.soup/packages/.staging/C++_Package2_3.2.1/] -> [C:/Users/Me/.soup/packages/C++/Package2/3.2.1/]",
					"Exists: C:/Users/Me/.soup/packages/C++/Package2/3.2.1/Recipe.sml",
					"OpenRead: C:/Users/Me/.soup/packages/C++/Package2/3.2.1/Recipe.sml",
					"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
				},
				mockFileSystem.GetRequests());

			// Verify zip requests
			mockZipManager.Verify(zip => zip.ExtractToDirectory(new Path("C:/Users/Me/.soup/packages/.staging/Package1.zip"), new Path("C:/Users/Me/.soup/packages/.staging/C++_Package1_1.2.3/")), Times.Once());
			mockZipManager.Verify(zip => zip.ExtractToDirectory(new Path("C:/Users/Me/.soup/packages/.staging/Package2.zip"), new Path("C:/Users/Me/.soup/packages/.staging/C++_Package2_3.2.1/")), Times.Once());
			mockZipManager.VerifyNoOtherCalls();

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/Package1/version/1.2.3/download"),
					"{Accept: [application/zip]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/Package2/version/3.2.1/download"),
					"{Accept: [application/zip]}",
					null),
				Times.Once());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expected =
@"Version: 3
Closures: {
	Root: {
		Cpp: [
			{ Name: ""MyPackage"", Version: ""./"", Build: ""Build0"" }
			{ Name: ""Package1"", Version: ""1.2.3"", Build: ""Build0"" }
			{ Name: ""Package2"", Version: ""3.2.1"", Build: ""Build0"" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: ""Soup.Cpp"", Version: ""0.1.0"" }
		]
	}
}";

			Assert.Equal(expected, packageLockContent);
		}

		[Fact]
		public async Task RestorePackagesAsync_HasPublicBuildDependencies()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""MyPackage""
					Language: ""C++|0.1""
					Version: ""1.0.0""
					Dependencies: {
						Build: [
							{ Reference: ""Package1@1.2.3"" }
						]
					}"))));

			mockFileSystem.CreateMockFile(
				new Path("C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""Package1""
					Language: ""C#|0.1""
					Version: ""1.2.3"""))));

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%23/package/Package1/version/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var uut = new PackageManager(httpClient);

			var workingDirectory = new Path("C:/Root/MyPackage/");
			bool forceRestore = false;
			await uut.RestorePackagesAsync(workingDirectory, forceRestore);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"HIGH: Install Package: C# Package1@1.2.3",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/Users/Me/.soup/locks/C#/Package1/1.2.3/",
					"DIAG: Load Package Lock: C:/Users/Me/.soup/locks/C#/Package1/1.2.3/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml",
					"DIAG: Root:C# Package1 -> ./",
					"DIAG: Build0:C# Soup.CSharp -> 0.1.0",
					"DIAG: Root:C++ MyPackage -> ./",
					"DIAG: Build0:C# Soup.Cpp -> 0.1.0",
					"DIAG: Build0:C# Package1 -> 1.2.3",
					"DIAG: Deleting staging directory",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/Package1/1.2.3/",
					"OpenWriteTruncate: C:/Users/Me/.soup/packages/.staging/Package1.zip",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/C#_Package1_1.2.3/",
					"DeleteFile: C:/Users/Me/.soup/packages/.staging/Package1.zip",
					"Exists: C:/Users/Me/.soup/packages/C#/Package1",
					"CreateDirectory: C:/Users/Me/.soup/packages/C#/Package1",
					"Rename: [C:/Users/Me/.soup/packages/.staging/C#_Package1_1.2.3/] -> [C:/Users/Me/.soup/packages/C#/Package1/1.2.3/]",
					"Exists: C:/Users/Me/.soup/locks/C#/Package1/1.2.3/",
					"CreateDirectory: C:/Users/Me/.soup/locks/C#/Package1/1.2.3/",
					"Exists: C:/Users/Me/.soup/locks/C#/Package1/1.2.3/PackageLock.sml",
					"Exists: C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml",
					"OpenRead: C:/Users/Me/.soup/packages/C#/Package1/1.2.3/Recipe.sml",
					"OpenWriteTruncate: C:/Users/Me/.soup/locks/C#/Package1/1.2.3/PackageLock.sml",
					"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
				},
				mockFileSystem.GetRequests());

			// Verify zip requests
			mockZipManager.Verify(zip => zip.ExtractToDirectory(new Path("C:/Users/Me/.soup/packages/.staging/Package1.zip"), new Path("C:/Users/Me/.soup/packages/.staging/C#_Package1_1.2.3/")), Times.Once());
			mockZipManager.VerifyNoOtherCalls();

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%23/package/Package1/version/1.2.3/download"),
					"{Accept: [application/zip]}",
					null),
				Times.Once());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expected =
@"Version: 3
Closures: {
	Root: {
		Cpp: [
			{ Name: ""MyPackage"", Version: ""./"", Build: ""Build0"" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: ""Soup.Cpp"", Version: ""0.1.0"" }
			{ Name: ""Package1"", Version: ""1.2.3"" }
		]
	}
}";

			Assert.Equal(expected, packageLockContent);

			var packageLock2 = mockFileSystem.GetMockFile(new Path("C:/Users/Me/.soup/locks/C#/Package1/1.2.3/PackageLock.sml"));
			packageLock2.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader2 = new System.IO.StreamReader(packageLock2.Content);
			var packageLockContent2 = await reader2.ReadToEndAsync();
			var expectedPackageLock2 =
@"Version: 3
Closures: {
	Root: {
		CSharp: [
			{ Name: ""Package1"", Version: ""./"", Build: ""Build0"" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: ""Soup.CSharp"", Version: ""0.1.0"" }
		]
	}
}";

			Assert.Equal(expectedPackageLock2, packageLockContent2);
		}

		[Fact]
		public async Task RestorePackagesAsync_HasLocalBuildDependency()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""MyPackage""
					Language: ""C++|0.1""
					Version: ""1.0.0""
					Dependencies: {
						Build: [
							{ Reference: ""../Package1/"" }
						]
					}"))));

			mockFileSystem.CreateMockFile(
				new Path("C:/Root/Package1/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""Package1""
					Language: ""C#|0.1""
					Version: ""1.2.3"""))));

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var uut = new PackageManager(httpClient);

			var workingDirectory = new Path("C:/Root/MyPackage/");
			bool forceRestore = false;
			await uut.RestorePackagesAsync(workingDirectory, forceRestore);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Root/Package1/Recipe.sml",
					"DIAG: Load Package Lock: C:/Root/Package1/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/Package1/Recipe.sml",
					"DIAG: Root:C# Package1 -> ./",
					"DIAG: Build0:C# Soup.CSharp -> 0.1.0",
					"DIAG: Root:C++ MyPackage -> ./",
					"DIAG: Build0:C# Soup.Cpp -> 0.1.0",
					"DIAG: Build0:C# Package1 -> 1.2.3",
					"DIAG: Deleting staging directory",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"Exists: C:/Root/Package1/Recipe.sml",
					"OpenRead: C:/Root/Package1/Recipe.sml",
					"Exists: C:/Root/Package1/PackageLock.sml",
					"Exists: C:/Root/Package1/Recipe.sml",
					"OpenRead: C:/Root/Package1/Recipe.sml",
					"OpenWriteTruncate: C:/Root/Package1/PackageLock.sml",
					"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
				},
				mockFileSystem.GetRequests());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expectedPackageLock =
@"Version: 3
Closures: {
	Root: {
		Cpp: [
			{ Name: ""MyPackage"", Version: ""./"", Build: ""Build0"" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: ""Soup.Cpp"", Version: ""0.1.0"" }
			{ Name: ""Package1"", Version: ""1.2.3"" }
		]
	}
}";

			Assert.Equal(expectedPackageLock, packageLockContent);

			var packageLock2 = mockFileSystem.GetMockFile(new Path("C:/Root/Package1/PackageLock.sml"));
			packageLock2.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader2 = new System.IO.StreamReader(packageLock2.Content);
			var packageLockContent2 = await reader2.ReadToEndAsync();
			var expectedPackageLock2 =
@"Version: 3
Closures: {
	Root: {
		CSharp: [
			{ Name: ""Package1"", Version: ""./"", Build: ""Build0"" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: ""Soup.CSharp"", Version: ""0.1.0"" }
		]
	}
}";

			Assert.Equal(expectedPackageLock2, packageLockContent2);
		}

		[Fact]
		public async Task RestorePackagesAsync_HasDependencies_HasPackageLock_OneOverride()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			// Create the original file
			var original =
@"Version: 3
Closures: {
	Root: {
		Cpp: [
			{ Name: ""MyPackage"", Version: ""./"", Build: ""Build0"" }
			{ Name: ""Package1"", Version: ""1.2.4"", Build: ""Build0"" }
			{ Name: ""Package2"", Version: ""3.2.1"", Build: ""Build0"" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: ""Soup.Cpp"", Version: ""0.1.0"" }
		]
	}
}";
			using var originalContent = new System.IO.MemoryStream();
			await originalContent.WriteAsync(Encoding.UTF8.GetBytes(original));
			originalContent.Seek(0, System.IO.SeekOrigin.Begin);
			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/PackageLock.sml"),
				new MockFile(originalContent));

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/Package1/version/1.2.4/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/Package2/version/3.2.1/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var uut = new PackageManager(httpClient);

			var workingDirectory = new Path("C:/Root/MyPackage/");
			bool forceRestore = false;
			await uut.RestorePackagesAsync(workingDirectory, forceRestore);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Restore from package lock",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"HIGH: Install Package: C++ Package1@1.2.4",
					"HIGH: Downloading package",
					"HIGH: Install Package: C++ Package2@3.2.1",
					"HIGH: Downloading package",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.Cpp@0.1.0",
					"HIGH: Skip build logic for now",
					"DIAG: Deleting staging directory",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"OpenRead: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/Package1/1.2.4/",
					"OpenWriteTruncate: C:/Users/Me/.soup/packages/.staging/Package1.zip",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/C++_Package1_1.2.4/",
					"DeleteFile: C:/Users/Me/.soup/packages/.staging/Package1.zip",
					"Exists: C:/Users/Me/.soup/packages/C++/Package1",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/Package1",
					"Rename: [C:/Users/Me/.soup/packages/.staging/C++_Package1_1.2.4/] -> [C:/Users/Me/.soup/packages/C++/Package1/1.2.4/]",
					"Exists: C:/Users/Me/.soup/packages/C++/Package2/3.2.1/",
					"OpenWriteTruncate: C:/Users/Me/.soup/packages/.staging/Package2.zip",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/C++_Package2_3.2.1/",
					"DeleteFile: C:/Users/Me/.soup/packages/.staging/Package2.zip",
					"Exists: C:/Users/Me/.soup/packages/C++/Package2",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/Package2",
					"Rename: [C:/Users/Me/.soup/packages/.staging/C++_Package2_3.2.1/] -> [C:/Users/Me/.soup/packages/C++/Package2/3.2.1/]",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
				},
				mockFileSystem.GetRequests());

			// Verify zip requests
			mockZipManager.Verify(zip => zip.ExtractToDirectory(new Path("C:/Users/Me/.soup/packages/.staging/Package1.zip"), new Path("C:/Users/Me/.soup/packages/.staging/C++_Package1_1.2.4/")), Times.Once());
			mockZipManager.Verify(zip => zip.ExtractToDirectory(new Path("C:/Users/Me/.soup/packages/.staging/Package2.zip"), new Path("C:/Users/Me/.soup/packages/.staging/C++_Package2_3.2.1/")), Times.Once());
			mockZipManager.VerifyNoOtherCalls();

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/Package1/version/1.2.4/download"),
					"{Accept: [application/zip]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/Package2/version/3.2.1/download"),
					"{Accept: [application/zip]}",
					null),
				Times.Once());
		}

		[Fact]
		public async Task RestorePackagesAsync_HasDependencies_HasPackageLock_AllLocal()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			mockFileSystem.RegisterChildren(
				new Path("C:/Users/Me/.soup/packages/C++/Package1/1.2.4/"),
				new List<DirectoryEntry>());
			mockFileSystem.RegisterChildren(
				new Path("C:/Users/Me/.soup/packages/C++/Package2/3.2.1/"),
				new List<DirectoryEntry>());

			// Create the original file
			var original =
@"Version: 3
Closures: {
	Root: {
		Cpp: [
			{ Name: ""MyPackage"", Version: ""./"", Build: ""Build0"" }
			{ Name: ""Package1"", Version: ""1.2.4"", Build: ""Build0"" }
			{ Name: ""Package2"", Version: ""3.2.1"", Build: ""Build0"" }
		]
	}
	Build0: {
		CSharp: [
			{ Name: ""Soup.Cpp"", Version: ""0.1.0"" }
		]
	}
}";
			using var originalContent = new System.IO.MemoryStream();
			await originalContent.WriteAsync(Encoding.UTF8.GetBytes(original));
			originalContent.Seek(0, System.IO.SeekOrigin.Begin);
			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/PackageLock.sml"),
				new MockFile(originalContent));

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var uut = new PackageManager(httpClient);

			var workingDirectory = new Path("C:/Root/MyPackage/");
			bool forceRestore = false;
			await uut.RestorePackagesAsync(workingDirectory, forceRestore);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Restore from package lock",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"HIGH: Install Package: C++ Package1@1.2.4",
					"HIGH: Found local version",
					"HIGH: Install Package: C++ Package2@3.2.1",
					"HIGH: Found local version",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.Cpp@0.1.0",
					"HIGH: Skip build logic for now",
					"DIAG: Deleting staging directory",

				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"OpenRead: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Users/Me/.soup/packages/C++/Package1/1.2.4/",
					"Exists: C:/Users/Me/.soup/packages/C++/Package2/3.2.1/",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
				},
				mockFileSystem.GetRequests());
		}

		[Fact]
		public async Task InstallPackageReferenceAsync_WithVersion()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			using var originalContent = new System.IO.MemoryStream();
			await originalContent.WriteAsync(Encoding.UTF8.GetBytes(
				@"Name: ""MyPackage""
				Language: ""C++|0.1""
				Version: ""1.0.0"""));
			originalContent.Seek(0, System.IO.SeekOrigin.Begin);
			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/Recipe.sml"),
				new MockFile(originalContent));

			mockFileSystem.CreateMockFile(
				new Path("C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""Package1""
					Language: ""C++|0.1""
					Version: ""1.2.3"""))));

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var uut = new PackageManager(httpClient);

			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/OtherPackage/version/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var workingDirectory = new Path("C:/Root/MyPackage/");
			var packageReference = "OtherPackage@1.2.3";
			await uut.InstallPackageReferenceAsync(workingDirectory, packageReference);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
					"HIGH: Install Package: C++ OtherPackage@1.2.3",
					"HIGH: Downloading package",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml",
					"INFO: Deleting staging directory",
					"INFO: Adding reference to recipe",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"Exists: C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/",
					"OpenWriteTruncate: C:/Users/Me/.soup/packages/.staging/OtherPackage.zip",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/C++_OtherPackage_1.2.3/",
					"DeleteFile: C:/Users/Me/.soup/packages/.staging/OtherPackage.zip",
					"Exists: C:/Users/Me/.soup/packages/C++/OtherPackage",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/OtherPackage",
					"Rename: [C:/Users/Me/.soup/packages/.staging/C++_OtherPackage_1.2.3/] -> [C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/]",
					"Exists: C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml",
					"OpenRead: C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
					"OpenWriteTruncate: C:/Root/MyPackage/Recipe.sml",
				},
				mockFileSystem.GetRequests());

			// Verify zip requests
			mockZipManager.Verify(zip => zip.ExtractToDirectory(new Path("C:/Users/Me/.soup/packages/.staging/OtherPackage.zip"), new Path("C:/Users/Me/.soup/packages/.staging/C++_OtherPackage_1.2.3/")), Times.Once());
			mockZipManager.VerifyNoOtherCalls();

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/OtherPackage/version/1.2.3/download"),
					"{Accept: [application/zip]}",
					null),
				Times.Once());

			// Verify the contents of the recipe file
			var recipeFile = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/Recipe.sml"));
			recipeFile.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var recipeReader = new System.IO.StreamReader(recipeFile.Content);
			var recipeContent = await recipeReader.ReadToEndAsync();
			var expectedRecipe =
@"Name: ""MyPackage""
				Language: ""C++|0.1""
				Version: ""1.0.0""
Dependencies: {
	Runtime: [
		""OtherPackage@1.2.3""
	]
}";

			Assert.Equal(expectedRecipe, recipeContent);
		}

		[Fact]
		public async Task InstallPackageReferenceAsync_NoVersion()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			using var originalContent = new System.IO.MemoryStream();
			await originalContent.WriteAsync(Encoding.UTF8.GetBytes(
				@"Name: ""MyPackage""
				Language: ""C++|0.1""
				Version: ""1.0.0"""));
			originalContent.Seek(0, System.IO.SeekOrigin.Begin);
			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/Recipe.sml"),
				new MockFile(originalContent));

			mockFileSystem.CreateMockFile(
				new Path("C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					@"Name: ""Package1""
					Language: ""C++|0.1""
					Version: ""1.2.3"""))));

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var uut = new PackageManager(httpClient);

			var getPackageResponse = JsonSerializer.Serialize(new PackageModel()
				{
					Name = "OtherPackage",
					Latest = new Api.Client.SemanticVersion() { Major = 1, Minor = 2, Patch = 3, },
				});
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/OtherPackage"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage() { Content = new StringContent(getPackageResponse) });
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/OtherPackage/version/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var workingDirectory = new Path("C:/Root/MyPackage/");
			var packageReference = "OtherPackage";
			await uut.InstallPackageReferenceAsync(workingDirectory, packageReference);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
					"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
					"HIGH: Latest Version: 1.2.3",
					"HIGH: Install Package: C++ OtherPackage@1.2.3",
					"HIGH: Downloading package",
					"DIAG: Load Recipe: C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml",
					"INFO: Deleting staging directory",
					"INFO: Adding reference to recipe",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"Exists: C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/",
					"OpenWriteTruncate: C:/Users/Me/.soup/packages/.staging/OtherPackage.zip",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/C++_OtherPackage_1.2.3/",
					"DeleteFile: C:/Users/Me/.soup/packages/.staging/OtherPackage.zip",
					"Exists: C:/Users/Me/.soup/packages/C++/OtherPackage",
					"CreateDirectory: C:/Users/Me/.soup/packages/C++/OtherPackage",
					"Rename: [C:/Users/Me/.soup/packages/.staging/C++_OtherPackage_1.2.3/] -> [C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/]",
					"Exists: C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml",
					"OpenRead: C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
					"OpenWriteTruncate: C:/Root/MyPackage/Recipe.sml",
				},
				mockFileSystem.GetRequests());

			// Verify zip requests
			mockZipManager.Verify(zip => zip.ExtractToDirectory(new Path("C:/Users/Me/.soup/packages/.staging/OtherPackage.zip"), new Path("C:/Users/Me/.soup/packages/.staging/C++_OtherPackage_1.2.3/")), Times.Once());
			mockZipManager.VerifyNoOtherCalls();

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/OtherPackage"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/OtherPackage/version/1.2.3/download"),
					"{Accept: [application/zip]}",
					null),
				Times.Once());

			// Verify the contents of the recipe file
			var recipeFile = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/Recipe.sml"));
			recipeFile.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var recipeReader = new System.IO.StreamReader(recipeFile.Content);
			var recipeContent = await recipeReader.ReadToEndAsync();
			var expectedRecipe =
@"Name: ""MyPackage""
				Language: ""C++|0.1""
				Version: ""1.0.0""
Dependencies: {
	Runtime: [
		""OtherPackage@1.2.3""
	]
}";

			Assert.Equal(expectedRecipe, recipeContent);
		}

		[Fact]
		public async Task PublishPackageAsync()
		{
			// Register the test listener
			var testListener = new TestTraceListener();
			using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

			// Setup the mock file system
			var mockFileSystem = new MockFileSystem();
			using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

			using var originalContent = new System.IO.MemoryStream();
			await originalContent.WriteAsync(Encoding.UTF8.GetBytes(
				@"Name: ""MyPackage""
				Language: ""C++|0.1""
				Version: ""1.0.0"""));
			originalContent.Seek(0, System.IO.SeekOrigin.Begin);
			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/Recipe.sml"),
				new MockFile(originalContent));

			// Pretend that there is a zip file created
			mockFileSystem.CreateMockFile(
				new Path("C:/Users/Me/.soup/packages/.staging/MyPackage.zip"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes("ZIP_FILE_CONTENT"))));

			mockFileSystem.RegisterChildren(
				new Path("C:/Root/MyPackage/"),
				new List<DirectoryEntry>()
				{
					new DirectoryEntry()
					{
						Path = new Path("C:/Root/MyPackage/Recipe.sml"),
						IsDirectory = false,
					},
				});

			// Setup the mock authentication manager
			var mockAuthenticationManager = new Mock<IAuthenticationManager>();
			using var scopedAuthenticationManager = new ScopedSingleton<IAuthenticationManager>(mockAuthenticationManager.Object);

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Create mock archive
			var mockZipArchive = new Mock<IZipArchive>();
			mockZipManager.Setup(zip => zip.OpenCreate(It.IsAny<Path>())).Returns(mockZipArchive.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var uut = new PackageManager(httpClient);

			var getPackageResponse = JsonSerializer.Serialize(new PackageModel()
			{
				Name = "MyPackage",
				Latest = new Api.Client.SemanticVersion() { Major = 1, Minor = 2, Patch = 3, },
			});
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/MyPackage"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage() { Content = new StringContent(getPackageResponse) });
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Put,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/MyPackage/version/1.0.0"),
					It.IsAny<string>(),
					It.IsAny<string>()))
				.Returns(() => new HttpResponseMessage(HttpStatusCode.Created));

			var workingDirectory = new Path("C:/Root/MyPackage/");
			await uut.PublishPackageAsync(workingDirectory);

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: Publish Project: C:/Root/MyPackage/",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
					"INFO: Request Authentication Token",
					"INFO: Publish package",
					"INFO: Package published",
					"INFO: Cleanup staging directory",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"GetUserProfileDirectory",
					"Exists: C:/Users/Me/.soup/packages/.staging/",
					"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
					"GetChildren: C:/Root/MyPackage/",
					"OpenRead: C:/Users/Me/.soup/packages/.staging/MyPackage.zip",
					"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
				},
				mockFileSystem.GetRequests());

			// Verify authentication requests
			mockAuthenticationManager.Verify(auth => auth.EnsureSignInAsync(), Times.Once());
			mockAuthenticationManager.VerifyNoOtherCalls();

			// Verify zip requests
			mockZipManager.Verify(zip => zip.OpenCreate(new Path("C:/Users/Me/.soup/packages/.staging/MyPackage.zip")), Times.Once());
			mockZipArchive.Verify(zip => zip.CreateEntryFromFile(new Path("C:/Root/MyPackage/Recipe.sml"), "Recipe.sml"), Times.Once());
			mockZipArchive.Verify(zip => zip.Dispose(), Times.Once());
			mockZipManager.VerifyNoOtherCalls();

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/MyPackage"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Put,
					new Uri("https://api.soupbuild.com/v1/language/C%2B%2B/package/MyPackage/version/1.0.0"),
					"",
					"ZIP_FILE_CONTENT"),
				Times.Once());
		}
	}
}