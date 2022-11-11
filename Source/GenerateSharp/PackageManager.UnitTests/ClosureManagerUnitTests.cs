// <copyright file="ClosureManagerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Moq;
using Opal;
using Opal.System;
using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using Xunit;

namespace Soup.Build.PackageManager.UnitTests
{
	[Collection("Opal")]
	public class ClosureManagerUnitTests
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
					"""
					Name: "MyPackage"
					Language: "C++|3.2.1"
					Version: "1.0.0"
					"""))));

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var generateClosureResult = new Api.Client.GenerateClosureResultModel()
			{
				Result = Api.Client.GenerateClosureResult.Success,
				RootClosure = new Dictionary<string, ICollection<Api.Client.PackageFeedExactReferenceWithBuildModel>>(),
				BuildClosures = new Dictionary<string, IDictionary<string, ICollection<Api.Client.PackageFeedExactReferenceModel>>>(),
			};
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					It.IsAny<string>(),
					It.IsAny<string>()))
				.Returns(() => new HttpResponseMessage()
				{
					Content = new StringContent(JsonSerializer.Serialize(generateClosureResult)),
				});

			var uut = new ClosureManager(
				new Uri("https://test.api.soupbuild.com/"),
				httpClient,
				new SemanticVersion(1, 2, 3),
				new SemanticVersion(3, 2, 1));

			await uut.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/PackageStore/"),
				new Path("C:/LockStore/"),
				new Path("C:/Staging/"));

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: Ensure Package Lock Exists: C:/Root/MyPackage/PackageLock.sml",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C++ MyPackage -> ./",
					"DIAG: Build0:C# Soup.Cpp -> 3.2.1",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version",
					"HIGH: Skip built in language version",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
				},
				mockFileSystem.GetRequests());

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					"""{"rootClosure":{"C\u002B\u002B":[]},"buildClosures":{},"requestedVersions":{}}"""),
				Times.Once());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expected =
				"""
				Version: 3
				Closures: {
					Root: {
						Cpp: [
							{ Name: "MyPackage", Version: "./", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.Cpp", Version: "3.2.1" }
						]
					}
				}
				""";

			Assert.Equal(expected, packageLockContent);
		}

		[Fact]
		public async Task RestorePackagesAsync_HasDependencies_GenerateClosureRequestFailure()
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
					"""
					Name: "MyPackage"
					Language: "C++|3.2.1"
					Version: "1.0.0"
					Dependencies: {
						Runtime: [
							{ Reference: "Package1@1.2.3" }
							{ Reference: "Package2@3.2.1" }
						]
					}
					"""))));

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var generateClosureResult = new Api.Client.GenerateClosureResultModel()
			{
				Result = Api.Client.GenerateClosureResult.Failure,
				Message = "Something went horribly wrong!",
			};
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					It.IsAny<string>(),
					It.IsAny<string>()))
				.Returns(() => new HttpResponseMessage()
				{
					Content = new StringContent(JsonSerializer.Serialize(generateClosureResult)),
				});
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package1/versions/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package2/versions/3.2.1/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var uut = new ClosureManager(
				new Uri("https://test.api.soupbuild.com/"),
				httpClient,
				new SemanticVersion(1, 2, 3),
				new SemanticVersion(3, 2, 1));

			_ = await Assert.ThrowsAsync<HandledException>(async () =>
			{
				await uut.GenerateAndRestoreRecursiveLocksAsync(
					new Path("C:/Root/MyPackage/"),
					new Path("C:/PackageStore/"),
					new Path("C:/LockStore/"),
					new Path("C:/Staging/"));
			});

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: Ensure Package Lock Exists: C:/Root/MyPackage/PackageLock.sml",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"INFO: Generate final service closure",
					"ERRO: Unable to create closure: Something went horribly wrong!",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
				},
				mockFileSystem.GetRequests());

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					"""{"rootClosure":{"C\u002B\u002B":[{"build":"","name":"Package1","version":{"major":1,"minor":2,"patch":3}},{"build":"","name":"Package2","version":{"major":3,"minor":2,"patch":1}}]},"buildClosures":{},"requestedVersions":{}}"""),
				Times.Once());
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
					"""
					Name: "MyPackage"
					Language: "C++|3.2.1"
					Version: "1.0.0"
					Dependencies: {
						Runtime: [
							{ Reference: "Package1@1.2.3" }
							{ Reference: "Package2@3.2.1" }
						]
					}
					"""))));

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var generateClosureResult = new Api.Client.GenerateClosureResultModel()
			{
				Result = Api.Client.GenerateClosureResult.Success,
				RootClosure = new Dictionary<string, ICollection<Api.Client.PackageFeedExactReferenceWithBuildModel>>(),
				BuildClosures = new Dictionary<string, IDictionary<string, ICollection<Api.Client.PackageFeedExactReferenceModel>>>(),
			};
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					It.IsAny<string>(),
					It.IsAny<string>()))
				.Returns(() => new HttpResponseMessage()
				{
					Content = new StringContent(JsonSerializer.Serialize(generateClosureResult)),
				});
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package1/versions/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package2/versions/3.2.1/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var uut = new ClosureManager(
				new Uri("https://test.api.soupbuild.com/"),
				httpClient,
				new SemanticVersion(1, 2, 3),
				new SemanticVersion(3, 2, 1));

			await uut.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/PackageStore/"),
				new Path("C:/LockStore/"),
				new Path("C:/Staging/"));

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: Ensure Package Lock Exists: C:/Root/MyPackage/PackageLock.sml",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C++ MyPackage -> ./",
					"DIAG: Root:C++ Package1 -> 1.2.3",
					"DIAG: Root:C++ Package2 -> 3.2.1",
					"DIAG: Build0:C# Soup.Cpp -> 3.2.1",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"HIGH: Install Package: C++ Package1@1.2.3",
					"HIGH: Downloading package",
					"HIGH: Install Package: C++ Package2@3.2.1",
					"HIGH: Downloading package",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version",
					"HIGH: Skip built in language version",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/PackageStore/C++/Package1/1.2.3/",
					"OpenWriteTruncate: C:/Staging/Package1.zip",
					"CreateDirectory: C:/Staging/C++_Package1_1.2.3/",
					"DeleteFile: C:/Staging/Package1.zip",
					"Exists: C:/PackageStore/C++/Package1",
					"CreateDirectory: C:/PackageStore/C++/Package1",
					"Rename: [C:/Staging/C++_Package1_1.2.3/] -> [C:/PackageStore/C++/Package1/1.2.3/]",
					"Exists: C:/PackageStore/C++/Package2/3.2.1/",
					"OpenWriteTruncate: C:/Staging/Package2.zip",
					"CreateDirectory: C:/Staging/C++_Package2_3.2.1/",
					"DeleteFile: C:/Staging/Package2.zip",
					"Exists: C:/PackageStore/C++/Package2",
					"CreateDirectory: C:/PackageStore/C++/Package2",
					"Rename: [C:/Staging/C++_Package2_3.2.1/] -> [C:/PackageStore/C++/Package2/3.2.1/]",

				},
				mockFileSystem.GetRequests());

			// Verify zip requests
			mockZipManager.Verify(zip => zip.ExtractToDirectory(
				new Path("C:/Staging/Package1.zip"),
				new Path("C:/Staging/C++_Package1_1.2.3/")),
				Times.Once());
			mockZipManager.Verify(zip => zip.ExtractToDirectory(
				new Path("C:/Staging/Package2.zip"),
				new Path("C:/Staging/C++_Package2_3.2.1/")),
				Times.Once());
			mockZipManager.VerifyNoOtherCalls();

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					"""{"rootClosure":{"C\u002B\u002B":[{"build":"","name":"Package1","version":{"major":1,"minor":2,"patch":3}},{"build":"","name":"Package2","version":{"major":3,"minor":2,"patch":1}}]},"buildClosures":{},"requestedVersions":{}}"""),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package1/versions/1.2.3/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package2/versions/3.2.1/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expected =
				"""
				Version: 3
				Closures: {
					Root: {
						Cpp: [
							{ Name: "MyPackage", Version: "./", Build: "Build0" }
							{ Name: "Package1", Version: "1.2.3", Build: "" }
							{ Name: "Package2", Version: "3.2.1", Build: "" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.Cpp", Version: "3.2.1" }
						]
					}
				}
				""";

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
					"""
					Name: "MyPackage"
					Language: "C++|5"
					Version: "1.0.0"
					Dependencies: {
						Build: [
							{ Reference: "Package1@1.2.3" }
						]
					}
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/C#/Package1/1.2.3/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Package1"
					Language: "C#|4"
					Version: "1.2.3"
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/C#/Soup.Cpp/5.0.0/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Soup.Cpp"
					Language: "C#|1.2.3"
					Version: "5.0.0"
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/C#/Soup.CSharp/4.0.0/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Soup.CSharp"
					Language: "C#|1.2.3"
					Version: "4.0.0"
					"""))));

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var generateClosureResult = new Api.Client.GenerateClosureResultModel()
			{
				Result = Api.Client.GenerateClosureResult.Success,
				RootClosure = new Dictionary<string, ICollection<Api.Client.PackageFeedExactReferenceWithBuildModel>>(),
				BuildClosures = new Dictionary<string, IDictionary<string, ICollection<Api.Client.PackageFeedExactReferenceModel>>>(),
			};
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					It.IsAny<string>(),
					It.IsAny<string>()))
				.Returns(() => new HttpResponseMessage()
				{
					Content = new StringContent(JsonSerializer.Serialize(generateClosureResult)),
				});
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%23/packages/Package1/versions/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%23/packages/Soup.Cpp/versions/5.0.0/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%23/packages/Soup.CSharp/versions/4.0.0/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var uut = new ClosureManager(
				new Uri("https://test.api.soupbuild.com/"),
				httpClient,
				new SemanticVersion(1, 2, 3),
				new SemanticVersion(3, 2, 1));

			await uut.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/PackageStore/"),
				new Path("C:/LockStore/"),
				new Path("C:/Staging/"));

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: Ensure Package Lock Exists: C:/Root/MyPackage/PackageLock.sml",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C++ MyPackage -> ./",
					"DIAG: Build0:C# Soup.Cpp -> 5.0.0",
					"DIAG: Build0:C# Package1 -> 1.2.3",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.Cpp@5.0.0",
					"HIGH: Downloading package",
					"HIGH: Install Package: C# Package1@1.2.3",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/LockStore/C#/Soup.Cpp/5.0.0/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/C#/Soup.Cpp/5.0.0/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/C#/Soup.Cpp/5.0.0/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/C#/Soup.Cpp/5.0.0/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C# Soup.Cpp -> ./",
					"DIAG: Build0:C# Soup.CSharp -> 1.2.3",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C#",
					"INFO: Skip Package: Soup.Cpp -> ./",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.CSharp@1.2.3",
					"HIGH: Skip built in language version",
					"HIGH: Skip built in language version",
					"DIAG: Create Directory: C:/LockStore/C#/Package1/1.2.3/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/C#/Package1/1.2.3/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/C#/Package1/1.2.3/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/C#/Package1/1.2.3/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C# Package1 -> ./",
					"DIAG: Build0:C# Soup.CSharp -> 4.0.0",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C#",
					"INFO: Skip Package: Package1 -> ./",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.CSharp@4.0.0",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/LockStore/C#/Soup.CSharp/4.0.0/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/C#/Soup.CSharp/4.0.0/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/C#/Soup.CSharp/4.0.0/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/C#/Soup.CSharp/4.0.0/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C# Soup.CSharp -> ./",
					"DIAG: Build0:C# Soup.CSharp -> 1.2.3",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C#",
					"INFO: Skip Package: Soup.CSharp -> ./",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.CSharp@1.2.3",
					"HIGH: Skip built in language version",
					"HIGH: Skip built in language version",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/PackageStore/C#/Soup.Cpp/5.0.0/",
					"OpenWriteTruncate: C:/Staging/Soup.Cpp.zip",
					"CreateDirectory: C:/Staging/C#_Soup.Cpp_5.0.0/",
					"DeleteFile: C:/Staging/Soup.Cpp.zip",
					"Exists: C:/PackageStore/C#/Soup.Cpp",
					"CreateDirectory: C:/PackageStore/C#/Soup.Cpp",
					"Rename: [C:/Staging/C#_Soup.Cpp_5.0.0/] -> [C:/PackageStore/C#/Soup.Cpp/5.0.0/]",
					"Exists: C:/PackageStore/C#/Package1/1.2.3/",
					"OpenWriteTruncate: C:/Staging/Package1.zip",
					"CreateDirectory: C:/Staging/C#_Package1_1.2.3/",
					"DeleteFile: C:/Staging/Package1.zip",
					"Exists: C:/PackageStore/C#/Package1",
					"CreateDirectory: C:/PackageStore/C#/Package1",
					"Rename: [C:/Staging/C#_Package1_1.2.3/] -> [C:/PackageStore/C#/Package1/1.2.3/]",
					"Exists: C:/LockStore/C#/Soup.Cpp/5.0.0/",
					"CreateDirectory: C:/LockStore/C#/Soup.Cpp/5.0.0/",
					"Exists: C:/LockStore/C#/Soup.Cpp/5.0.0/PackageLock.sml",
					"Exists: C:/PackageStore/C#/Soup.Cpp/5.0.0/Recipe.sml",
					"OpenRead: C:/PackageStore/C#/Soup.Cpp/5.0.0/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/C#/Soup.Cpp/5.0.0/PackageLock.sml",
					"Exists: C:/LockStore/C#/Package1/1.2.3/",
					"CreateDirectory: C:/LockStore/C#/Package1/1.2.3/",
					"Exists: C:/LockStore/C#/Package1/1.2.3/PackageLock.sml",
					"Exists: C:/PackageStore/C#/Package1/1.2.3/Recipe.sml",
					"OpenRead: C:/PackageStore/C#/Package1/1.2.3/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/C#/Package1/1.2.3/PackageLock.sml",
					"Exists: C:/PackageStore/C#/Soup.CSharp/4.0.0/",
					"OpenWriteTruncate: C:/Staging/Soup.CSharp.zip",
					"CreateDirectory: C:/Staging/C#_Soup.CSharp_4.0.0/",
					"DeleteFile: C:/Staging/Soup.CSharp.zip",
					"Exists: C:/PackageStore/C#/Soup.CSharp",
					"CreateDirectory: C:/PackageStore/C#/Soup.CSharp",
					"Rename: [C:/Staging/C#_Soup.CSharp_4.0.0/] -> [C:/PackageStore/C#/Soup.CSharp/4.0.0/]",
					"Exists: C:/LockStore/C#/Soup.CSharp/4.0.0/",
					"CreateDirectory: C:/LockStore/C#/Soup.CSharp/4.0.0/",
					"Exists: C:/LockStore/C#/Soup.CSharp/4.0.0/PackageLock.sml",
					"Exists: C:/PackageStore/C#/Soup.CSharp/4.0.0/Recipe.sml",
					"OpenRead: C:/PackageStore/C#/Soup.CSharp/4.0.0/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/C#/Soup.CSharp/4.0.0/PackageLock.sml",
				},
				mockFileSystem.GetRequests());

			// Verify zip requests
			mockZipManager.Verify(zip => zip.ExtractToDirectory(
				new Path("C:/Staging/Soup.Cpp.zip"),
				new Path("C:/Staging/C#_Soup.Cpp_5.0.0/")),
				Times.Once());
			mockZipManager.Verify(zip => zip.ExtractToDirectory(
				new Path("C:/Staging/Package1.zip"),
				new Path("C:/Staging/C#_Package1_1.2.3/")),
				Times.Once());
			mockZipManager.Verify(zip => zip.ExtractToDirectory(
				new Path("C:/Staging/Soup.CSharp.zip"),
				new Path("C:/Staging/C#_Soup.CSharp_4.0.0/")),
				Times.Once());

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					"""{"rootClosure":{"C\u002B\u002B":[]},"buildClosures":{},"requestedVersions":{}}"""),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%23/packages/Package1/versions/1.2.3/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%23/packages/Soup.Cpp/versions/5.0.0/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%23/packages/Soup.CSharp/versions/4.0.0/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expected =
				"""
				Version: 3
				Closures: {
					Root: {
						Cpp: [
							{ Name: "MyPackage", Version: "./", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.Cpp", Version: "5.0.0" }
							{ Name: "Package1", Version: "1.2.3" }
						]
					}
				}
				""";

			Assert.Equal(expected, packageLockContent);

			var package1Lock = mockFileSystem.GetMockFile(new Path("C:/LockStore/C#/Package1/1.2.3/PackageLock.sml"));
			package1Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerPackage1Lock = new System.IO.StreamReader(package1Lock.Content);
			var package1LockContent = await readerPackage1Lock.ReadToEndAsync();
			var expectedPackage1Lock =
				"""
				Version: 3
				Closures: {
					Root: {
						CSharp: [
							{ Name: "Package1", Version: "./", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.CSharp", Version: "4.0.0" }
						]
					}
				}
				""";

			Assert.Equal(expectedPackage1Lock, package1LockContent);

			var soupCppLock = mockFileSystem.GetMockFile(new Path("C:/LockStore/C#/Soup.Cpp/5.0.0/PackageLock.sml"));
			soupCppLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerSoupCppLock = new System.IO.StreamReader(soupCppLock.Content);
			var soupCppLockContent = await readerSoupCppLock.ReadToEndAsync();
			var expectedSoupCppLock =
				"""
				Version: 3
				Closures: {
					Root: {
						CSharp: [
							{ Name: "Soup.Cpp", Version: "./", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.CSharp", Version: "1.2.3" }
						]
					}
				}
				""";

			Assert.Equal(expectedSoupCppLock, soupCppLockContent);

			var soupCSharpLock = mockFileSystem.GetMockFile(new Path("C:/LockStore/C#/Soup.CSharp/4.0.0/PackageLock.sml"));
			soupCSharpLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerSoupCSharpLock = new System.IO.StreamReader(soupCSharpLock.Content);
			var soupCSharpLockContent = await readerSoupCSharpLock.ReadToEndAsync();
			var expectedSoupCSharpLock =
				"""
				Version: 3
				Closures: {
					Root: {
						CSharp: [
							{ Name: "Soup.CSharp", Version: "./", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.CSharp", Version: "1.2.3" }
						]
					}
				}
				""";

			Assert.Equal(expectedSoupCSharpLock, soupCSharpLockContent);
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
					"""
					Name: "MyPackage"
					Language: "C++|3.2.1"
					Version: "1.0.0"
					Dependencies: {
						Build: [
							{ Reference: "../Package1/" }
						]
					}
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/Root/Package1/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Package1"
					Language: "C#|1.2.3"
					Version: "1.2.3"
					"""))));

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var generateClosureResult = new Api.Client.GenerateClosureResultModel()
			{
				Result = Api.Client.GenerateClosureResult.Success,
				RootClosure = new Dictionary<string, ICollection<Api.Client.PackageFeedExactReferenceWithBuildModel>>(),
				BuildClosures = new Dictionary<string, IDictionary<string, ICollection<Api.Client.PackageFeedExactReferenceModel>>>(),
			};
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					It.IsAny<string>(),
					It.IsAny<string>()))
				.Returns(() => new HttpResponseMessage()
				{
					Content = new StringContent(JsonSerializer.Serialize(generateClosureResult)),
				});

			var uut = new ClosureManager(
				new Uri("https://test.api.soupbuild.com/"),
				httpClient,
				new SemanticVersion(1, 2, 3),
				new SemanticVersion(3, 2, 1));

			await uut.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/PackageStore/"),
				new Path("C:/LockStore/"),
				new Path("C:/Staging/"));

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: Ensure Package Lock Exists: C:/Root/MyPackage/PackageLock.sml",
					"DIAG: Load Package Lock: C:/Root/MyPackage/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
					"DIAG: Load Recipe: C:/Root/Package1/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C++ MyPackage -> ./",
					"DIAG: Build0:C# Soup.Cpp -> 3.2.1",
					"DIAG: Build0:C# Package1 -> ../Package1/",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version",
					"INFO: Skip Package: Package1 -> ../Package1/",
					"HIGH: Skip built in language version",
					"INFO: Ensure Package Lock Exists: C:/Root/Package1/PackageLock.sml",
					"DIAG: Load Package Lock: C:/Root/Package1/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/Package1/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C# Package1 -> ./",
					"DIAG: Build0:C# Soup.CSharp -> 1.2.3",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C#",
					"INFO: Skip Package: Package1 -> ./",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.CSharp@1.2.3",
					"HIGH: Skip built in language version",
					"HIGH: Skip built in language version",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/MyPackage/Recipe.sml",
					"OpenRead: C:/Root/MyPackage/Recipe.sml",
					"Exists: C:/Root/Package1/Recipe.sml",
					"OpenRead: C:/Root/Package1/Recipe.sml",
					"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/Root/Package1/PackageLock.sml",
					"Exists: C:/Root/Package1/Recipe.sml",
					"OpenRead: C:/Root/Package1/Recipe.sml",
					"OpenWriteTruncate: C:/Root/Package1/PackageLock.sml",
				},
				mockFileSystem.GetRequests());

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					"""{"rootClosure":{"C\u002B\u002B":[]},"buildClosures":{},"requestedVersions":{}}"""),
				Times.Once());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expectedPackageLock =
				"""
				Version: 3
				Closures: {
					Root: {
						Cpp: [
							{ Name: "MyPackage", Version: "./", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.Cpp", Version: "3.2.1" }
							{ Name: "Package1", Version: "../Package1/" }
						]
					}
				}
				""";

			Assert.Equal(expectedPackageLock, packageLockContent);

			var packageLock2 = mockFileSystem.GetMockFile(new Path("C:/Root/Package1/PackageLock.sml"));
			packageLock2.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader2 = new System.IO.StreamReader(packageLock2.Content);
			var packageLockContent2 = await reader2.ReadToEndAsync();
			var expectedPackageLock2 =
				"""
				Version: 3
				Closures: {
					Root: {
						CSharp: [
							{ Name: "Package1", Version: "./", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.CSharp", Version: "1.2.3" }
						]
					}
				}
				""";

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
				"""
				Version: 3
				Closures: {
					Root: {
						Cpp: [
							{ Name: "MyPackage", Version: "./", Build: "Build0" }
							{ Name: "Package1", Version: "1.2.4", Build: "Build0" }
							{ Name: "Package2", Version: "3.2.1", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.Cpp", Version: "3.2.2" }
						]
					}
				}
				""";
			using var originalContent = new System.IO.MemoryStream();
			await originalContent.WriteAsync(Encoding.UTF8.GetBytes(original));
			originalContent.Seek(0, System.IO.SeekOrigin.Begin);
			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/PackageLock.sml"),
				new MockFile(originalContent));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/C#/Soup.Cpp/3.2.2/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Soup.Cpp"
					Language: "C#|1.2.3"
					Version: "3.2.2"
					"""))));

			// Setup the mock zip manager
			var mockZipManager = new Mock<IZipManager>();
			using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package1/versions/1.2.4/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package2/versions/3.2.1/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%23/packages/Soup.Cpp/versions/3.2.2/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());

			var generateClosureResult = new Api.Client.GenerateClosureResultModel()
			{
				Result = Api.Client.GenerateClosureResult.Success,
				RootClosure = new Dictionary<string, ICollection<Api.Client.PackageFeedExactReferenceWithBuildModel>>(),
				BuildClosures = new Dictionary<string, IDictionary<string, ICollection<Api.Client.PackageFeedExactReferenceModel>>>(),
			};
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					It.IsAny<string>(),
					It.IsAny<string>()))
				.Returns(() => new HttpResponseMessage()
				{
					Content = new StringContent(JsonSerializer.Serialize(generateClosureResult)),
				});

			var uut = new ClosureManager(
				new Uri("https://test.api.soupbuild.com/"),
				httpClient,
				new SemanticVersion(1, 2, 3),
				new SemanticVersion(3, 2, 1));

			await uut.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/PackageStore/"),
				new Path("C:/LockStore/"),
				new Path("C:/Staging/"));

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: Ensure Package Lock Exists: C:/Root/MyPackage/PackageLock.sml",
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
					"HIGH: Install Package: C# Soup.Cpp@3.2.2",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/LockStore/C#/Soup.Cpp/3.2.2/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/C#/Soup.Cpp/3.2.2/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/C#/Soup.Cpp/3.2.2/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/C#/Soup.Cpp/3.2.2/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C# Soup.Cpp -> ./",
					"DIAG: Build0:C# Soup.CSharp -> 1.2.3",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C#",
					"INFO: Skip Package: Soup.Cpp -> ./",
					"INFO: Restore Packages for Closure Build0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.CSharp@1.2.3",
					"HIGH: Skip built in language version",
					"HIGH: Skip built in language version",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"OpenRead: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/PackageStore/C++/Package1/1.2.4/",
					"OpenWriteTruncate: C:/Staging/Package1.zip",
					"CreateDirectory: C:/Staging/C++_Package1_1.2.4/",
					"DeleteFile: C:/Staging/Package1.zip",
					"Exists: C:/PackageStore/C++/Package1",
					"CreateDirectory: C:/PackageStore/C++/Package1",
					"Rename: [C:/Staging/C++_Package1_1.2.4/] -> [C:/PackageStore/C++/Package1/1.2.4/]",
					"Exists: C:/PackageStore/C++/Package2/3.2.1/",
					"OpenWriteTruncate: C:/Staging/Package2.zip",
					"CreateDirectory: C:/Staging/C++_Package2_3.2.1/",
					"DeleteFile: C:/Staging/Package2.zip",
					"Exists: C:/PackageStore/C++/Package2",
					"CreateDirectory: C:/PackageStore/C++/Package2",
					"Rename: [C:/Staging/C++_Package2_3.2.1/] -> [C:/PackageStore/C++/Package2/3.2.1/]",
					"Exists: C:/PackageStore/C#/Soup.Cpp/3.2.2/",
					"OpenWriteTruncate: C:/Staging/Soup.Cpp.zip",
					"CreateDirectory: C:/Staging/C#_Soup.Cpp_3.2.2/",
					"DeleteFile: C:/Staging/Soup.Cpp.zip",
					"Exists: C:/PackageStore/C#/Soup.Cpp",
					"CreateDirectory: C:/PackageStore/C#/Soup.Cpp",
					"Rename: [C:/Staging/C#_Soup.Cpp_3.2.2/] -> [C:/PackageStore/C#/Soup.Cpp/3.2.2/]",
					"Exists: C:/LockStore/C#/Soup.Cpp/3.2.2/",
					"CreateDirectory: C:/LockStore/C#/Soup.Cpp/3.2.2/",
					"Exists: C:/LockStore/C#/Soup.Cpp/3.2.2/PackageLock.sml",
					"Exists: C:/PackageStore/C#/Soup.Cpp/3.2.2/Recipe.sml",
					"OpenRead: C:/PackageStore/C#/Soup.Cpp/3.2.2/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/C#/Soup.Cpp/3.2.2/PackageLock.sml",
				},
				mockFileSystem.GetRequests());

			// Verify zip requests
			mockZipManager.Verify(zip => zip.ExtractToDirectory(
				new Path("C:/Staging/Package1.zip"),
				new Path("C:/Staging/C++_Package1_1.2.4/")),
				Times.Once());
			mockZipManager.Verify(zip => zip.ExtractToDirectory(
				new Path("C:/Staging/Package2.zip"),
				new Path("C:/Staging/C++_Package2_3.2.1/")),
				Times.Once());
			mockZipManager.Verify(zip => zip.ExtractToDirectory(
				new Path("C:/Staging/Soup.Cpp.zip"), new Path("C:/Staging/C#_Soup.Cpp_3.2.2/")),
				Times.Once());

			// Verify http requests
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package1/versions/1.2.4/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package2/versions/3.2.1/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%23/packages/Soup.Cpp/versions/3.2.2/download"),
					"{Accept: [application/json]}",
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
				new Path("C:/PackageStore/C++/Package1/1.2.4/"),
				new List<DirectoryEntry>());
			mockFileSystem.RegisterChildren(
				new Path("C:/PackageStore/C++/Package2/3.2.1/"),
				new List<DirectoryEntry>());

			// Create the original file
			var original =
				"""
				Version: 3
				Closures: {
					Root: {
						Cpp: [
							{ Name: "MyPackage", Version: "./", Build: "Build0" }
							{ Name: "Package1", Version: "1.2.4", Build: "Build0" }
							{ Name: "Package2", Version: "3.2.1", Build: "Build0" }
						]
					}
					Build0: {
						CSharp: [
							{ Name: "Soup.Cpp", Version: "3.2.1" }
						]
					}
				}
				""";
			using var originalContent = new System.IO.MemoryStream();
			await originalContent.WriteAsync(Encoding.UTF8.GetBytes(original));
			originalContent.Seek(0, System.IO.SeekOrigin.Begin);
			mockFileSystem.CreateMockFile(
				new Path("C:/Root/MyPackage/PackageLock.sml"),
				new MockFile(originalContent));

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var uut = new ClosureManager(
				new Uri("https://test.api.soupbuild.com/"),
				httpClient,
				new SemanticVersion(1, 2, 3),
				new SemanticVersion(3, 2, 1));

			await uut.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/PackageStore/"),
				new Path("C:/LockStore/"),
				new Path("C:/Staging/"));

			// Verify expected logs
			Assert.Equal(
				new List<string>()
				{
					"INFO: Ensure Package Lock Exists: C:/Root/MyPackage/PackageLock.sml",
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
					"HIGH: Install Package: C# Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version",
					"HIGH: Skip built in language version",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"OpenRead: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/PackageStore/C++/Package1/1.2.4/",
					"Exists: C:/PackageStore/C++/Package2/3.2.1/",
				},
				mockFileSystem.GetRequests());
		}
	}
}
