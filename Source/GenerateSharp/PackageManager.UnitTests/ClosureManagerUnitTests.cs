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

namespace Soup.Build.PackageManager.UnitTests;

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
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		var generateClosureResult = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 1,
					Build = "Build0",
					Tool = "Tool0",
				}
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Cpp",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
							}
						},
					}
				}
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				}
			},
		};
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
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
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
				"DIAG: Build0:Wren mwasplund|Soup.Cpp -> 3.2.1",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: MyPackage -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Cpp@3.2.1",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure",

			},
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			new List<string>()
			{
				"Exists: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
				"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
			},
			mockFileSystem.Requests);

		// Verify http requests
		var expectedGenerateRequest = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 1,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, }
				},
				Dependencies = new Dictionary<string, ICollection<int>>(),
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequestValue = JsonSerializer.Serialize(expectedGenerateRequest);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequestValue),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();

		// Verify the contents of the package lock file
		var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
		_ = packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var reader = new System.IO.StreamReader(packageLock.Content);
		var packageLockContent = await reader.ReadToEndAsync();
		var expected =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							MyPackage: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Cpp": { Version: "3.2.1" }
						}
					}
					Tool0: {

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
							{ Reference: "User1|Package1@1.2.3" }
							{ Reference: "User1|Package2@3.2.1" }
						]
					}
					"""))));

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		var generateClosureResult = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Failure,
			Message = "Something went horribly wrong!",
		};
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				It.IsAny<string>(),
				It.IsAny<string>()))
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult)),
			});
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package1/versions/1.2.3/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package2/versions/3.2.1/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());

		var uut = new ClosureManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			new SemanticVersion(1, 2, 3),
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			new List<string>()
			{
				"Exists: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
			},
			mockFileSystem.Requests);

		// Verify http requests
		var expectedGenerateRequest = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 3,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, }
				},
				Dependencies = new Dictionary<string, ICollection<int>>()
					{
						{
							"Runtime",
							new List<int>()
							{
								1,
								2,
							}
						},
					},
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>()
			{
				new Api.Client.PackagePublicReferenceModel()
				{
					Id = 1,
					Language = "C++",
					Name = "Package1",
					Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicReferenceModel()
				{
					Id = 2,
					Language = "C++",
					Name = "Package2",
					Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
				},
			},
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequestValue = JsonSerializer.Serialize(expectedGenerateRequest);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequestValue),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();
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
							{ Reference: "User1|Package1@1.2.3" }
							{ Reference: "User1|Package2@3.2.1" }
						]
					}
					"""))));

		// Setup the mock zip manager
		var mockZipManager = new Mock<IZipManager>();
		using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		var generateClosureResult = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 3,
					Build = "Build0",
					Tool = "Tool0",
				},
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					Public = new Api.Client.PackagePublicExactReferenceModel()
					{
						Name = "Package1",
						Owner = "User1",
						Language = "C++",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					Build = "Build0",
					Tool = "Tool0",
				},
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					Public = new Api.Client.PackagePublicExactReferenceModel()
					{
						Name = "Package2",
						Owner = "User1",
						Language = "C++",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					Build = "Build0",
					Tool = "Tool0",
				},
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Cpp",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
							}
						},
					}
				},
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				},
			},
		};
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				It.IsAny<string>(),
				It.IsAny<string>()))
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult)),
			});
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package1/versions/1.2.3/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package2/versions/3.2.1/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());

		var uut = new ClosureManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			new SemanticVersion(1, 2, 3),
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
				"DIAG: Root:C++ User1|Package1 -> 1.2.3",
				"DIAG: Root:C++ User1|Package2 -> 3.2.1",
				"DIAG: Build0:Wren mwasplund|Soup.Cpp -> 3.2.1",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: MyPackage -> ./",
				"HIGH: Install Package: C++ User1 Package1@1.2.3",
				"HIGH: Downloading package",
				"HIGH: Install Package: C++ User1 Package2@3.2.1",
				"HIGH: Downloading package",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Cpp@3.2.1",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure"
			},
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			new List<string>()
			{
				"Exists: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
				"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/PackageStore/Cpp/User1/Package1/1.2.3/",
				"OpenWriteTruncate: C:/Staging/Package1.zip",
				"CreateDirectory: C:/Staging/C++_Package1_1.2.3/",
				"DeleteFile: C:/Staging/Package1.zip",
				"Exists: C:/PackageStore/Cpp/User1/Package1",
				"CreateDirectory: C:/PackageStore/Cpp/User1/Package1",
				"Rename: [C:/Staging/C++_Package1_1.2.3/] -> [C:/PackageStore/Cpp/User1/Package1/1.2.3/]",
				"Exists: C:/PackageStore/Cpp/User1/Package2/3.2.1/",
				"OpenWriteTruncate: C:/Staging/Package2.zip",
				"CreateDirectory: C:/Staging/C++_Package2_3.2.1/",
				"DeleteFile: C:/Staging/Package2.zip",
				"Exists: C:/PackageStore/Cpp/User1/Package2",
				"CreateDirectory: C:/PackageStore/Cpp/User1/Package2",
				"Rename: [C:/Staging/C++_Package2_3.2.1/] -> [C:/PackageStore/Cpp/User1/Package2/3.2.1/]",
			},
			mockFileSystem.Requests);

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
		var expectedGenerateRequest = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 3,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>()
				{
					{
						"Runtime",
						new List<int>()
						{
							1,
							2,
						}
					},
				}
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>()
			{
				new Api.Client.PackagePublicReferenceModel()
				{
					Id = 1,
					Language = "C++",
					Name = "Package1",
					Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicReferenceModel()
				{
					Id = 2,
					Language = "C++",
					Name = "Package2",
					Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
				},
			},
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequestValue = JsonSerializer.Serialize(expectedGenerateRequest);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequestValue),
			Times.Once());
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package1/versions/1.2.3/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package2/versions/3.2.1/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();

		// Verify the contents of the package lock file
		var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
		_ = packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var reader = new System.IO.StreamReader(packageLock.Content);
		var packageLockContent = await reader.ReadToEndAsync();
		var expected =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							MyPackage: { Version: "./", Build: "Build0", Tool: "Tool0" }
							"User1|Package1": { Version: "1.2.3", Build: "Build0", Tool: "Tool0" }
							"User1|Package2": { Version: "3.2.1", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Cpp": { Version: "3.2.1" }
						}
					}
					Tool0: {

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
							{ Reference: "User1|Package1@1.2.3" }
						]
					}
					"""))));

		mockFileSystem.CreateMockFile(
			new Path("C:/PackageStore/Wren/User1/Package1/1.2.3/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
					Name: "Package1"
					Language: "Wren|4"
					Version: "1.2.3"
					"""))));

		mockFileSystem.CreateMockFile(
			new Path("C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
					Name: "Soup.Cpp"
					Language: "Wren|4"
					Version: "5.0.0"
					"""))));

		// Setup the mock zip manager
		var mockZipManager = new Mock<IZipManager>();
		using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		var generateClosureResult1 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 2,
					Build = "Build0",
					Tool = "Tool0",
				},
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Cpp",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 5, Minor = 0, Patch = 0, },
							}
						},
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Package1",
								Owner = "User1",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
							}
						},
					}
				},
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				},
			},
		};
		var generateClosureResult2 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 1,
					Build = "Build0",
					Tool = "Tool0",
				},
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Wren",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
							}
						},
					}
				},
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				},
			},
		};
		_ = mockMessageHandler
			.SetupSequence(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				It.IsAny<string>(),
				It.IsAny<string>()))
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult1)),
			})
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult2)),
			})
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult2)),
			});
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/User1/Package1/versions/1.2.3/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/mwasplund/Soup.Cpp/versions/5.0.0/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/users/mwasplund/packages/Wren/Soup.Wren/versions/6.0.0/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());

		var uut = new ClosureManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			new SemanticVersion(1, 2, 3),
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
				"DIAG: Build0:Wren mwasplund|Soup.Cpp -> 5.0.0",
				"DIAG: Build0:Wren User1|Package1 -> 1.2.3",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: MyPackage -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Cpp@5.0.0",
				"HIGH: Downloading package",
				"HIGH: Install Package: Wren User1 Package1@1.2.3",
				"HIGH: Downloading package",
				"INFO: Restore Packages for Closure Tool0",
				"DIAG: Create Directory: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/",
				"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml",
				"DIAG: Load Package Lock: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:Wren Soup.Cpp -> ./",
				"DIAG: Build0:Wren mwasplund|Soup.Wren -> 4.5.6",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language Wren",
				"INFO: Skip Package: Soup.Cpp -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Wren@4.5.6",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure",
				"DIAG: Create Directory: C:/LockStore/Wren/User1/Package1/1.2.3/",
				"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml",
				"DIAG: Load Package Lock: C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/PackageStore/Wren/User1/Package1/1.2.3/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:Wren Package1 -> ./",
				"DIAG: Build0:Wren mwasplund|Soup.Wren -> 4.5.6",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language Wren",
				"INFO: Skip Package: Package1 -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Wren@4.5.6",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure"
			},
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			new List<string>()
			{
				"Exists: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
				"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/",
				"OpenWriteTruncate: C:/Staging/Soup.Cpp.zip",
				"CreateDirectory: C:/Staging/Wren_Soup.Cpp_5.0.0/",
				"DeleteFile: C:/Staging/Soup.Cpp.zip",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp",
				"CreateDirectory: C:/PackageStore/Wren/mwasplund/Soup.Cpp",
				"Rename: [C:/Staging/Wren_Soup.Cpp_5.0.0/] -> [C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/]",
				"Exists: C:/PackageStore/Wren/User1/Package1/1.2.3/",
				"OpenWriteTruncate: C:/Staging/Package1.zip",
				"CreateDirectory: C:/Staging/Wren_Package1_1.2.3/",
				"DeleteFile: C:/Staging/Package1.zip",
				"Exists: C:/PackageStore/Wren/User1/Package1",
				"CreateDirectory: C:/PackageStore/Wren/User1/Package1",
				"Rename: [C:/Staging/Wren_Package1_1.2.3/] -> [C:/PackageStore/Wren/User1/Package1/1.2.3/]",
				"Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/",
				"CreateDirectory: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/",
				"Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/Recipe.sml",
				"OpenRead: C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/Recipe.sml",
				"OpenWriteTruncate: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml",
				"Exists: C:/LockStore/Wren/User1/Package1/1.2.3/",
				"CreateDirectory: C:/LockStore/Wren/User1/Package1/1.2.3/",
				"Exists: C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml",
				"Exists: C:/PackageStore/Wren/User1/Package1/1.2.3/Recipe.sml",
				"OpenRead: C:/PackageStore/Wren/User1/Package1/1.2.3/Recipe.sml",
				"OpenWriteTruncate: C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml",
			},
			mockFileSystem.Requests);

		// Verify zip requests
		mockZipManager.Verify(zip => zip.ExtractToDirectory(
			new Path("C:/Staging/Soup.Cpp.zip"),
			new Path("C:/Staging/Wren_Soup.Cpp_5.0.0/")),
			Times.Once());
		mockZipManager.Verify(zip => zip.ExtractToDirectory(
			new Path("C:/Staging/Package1.zip"),
			new Path("C:/Staging/Wren_Package1_1.2.3/")),
			Times.Once());
		mockZipManager.VerifyNoOtherCalls();

		// Verify http requests
		var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 2,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 5, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>()
					{
						{
							"Build",
							new List<int>()
							{
								1,
							}
						},
					}
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>()
			{
				new Api.Client.PackagePublicReferenceModel()
				{
					Id = 1,
					Language = "Wren",
					Name = "Package1",
					Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
				},
			},
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest1Value),
			Times.Once());

		var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 1,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "Wren",
					Version = new Api.Client.SemanticVersionModel() { Major = 4, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>(),
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest2Value),
			Times.Exactly(2));

		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/User1/Package1/versions/1.2.3/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/mwasplund/Soup.Cpp/versions/5.0.0/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();

		// Verify the contents of the package lock file
		var myPackageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
		_ = myPackageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var myPackageLockReader = new System.IO.StreamReader(myPackageLock.Content);
		var myPackageLockContent = await myPackageLockReader.ReadToEndAsync();
		var expectedMyPackageLock =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							MyPackage: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Cpp": { Version: "5.0.0" }
							"User1|Package1": { Version: "1.2.3" }
						}
					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedMyPackageLock, myPackageLockContent);

		var package1Lock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml"));
		_ = package1Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var readerPackage1Lock = new System.IO.StreamReader(package1Lock.Content);
		var package1LockContent = await readerPackage1Lock.ReadToEndAsync();
		var expectedPackage1Lock =
			"""
				Version: 5
				Closures: {
					Root: {
						Wren: {
							Package1: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Wren": { Version: "4.5.6" }
						}
					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedPackage1Lock, package1LockContent);

		var soupCppLock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml"));
		_ = soupCppLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var readerSoupCppLock = new System.IO.StreamReader(soupCppLock.Content);
		var soupCppLockContent = await readerSoupCppLock.ReadToEndAsync();
		var expectedSoupCppLock =
			"""
				Version: 5
				Closures: {
					Root: {
						Wren: {
							"Soup.Cpp": { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Wren": { Version: "4.5.6" }
						}
					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedSoupCppLock, soupCppLockContent);
	}

	[Fact]
	public async Task RestorePackagesAsync_HasPublicBuildDependencies_WithToolDependency()
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
							{ Reference: "User1|Package1@1.2.3" }
						]
					}
					"""))));

		mockFileSystem.CreateMockFile(
			new Path("C:/PackageStore/Wren/User1/Package1/1.2.3/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
					Name: "Package1"
					Language: "Wren|6"
					Version: "1.2.3"
					Dependencies: {
						Tool: [
							{ Reference: "C++|User1|Package2@2.3.4" }
						]
					}
					"""))));

		mockFileSystem.CreateMockFile(
			new Path("C:/PackageStore/Cpp/User1/Package2/2.3.4/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
					Name: "Package2"
					Language: "C++|5"
					Version: "2.3.4"
					"""))));

		mockFileSystem.CreateMockFile(
			new Path("C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
					Name: "Soup.Cpp"
					Language: "Wren|4.5.6"
					Version: "5.0.0"
					"""))));

		// Setup the mock zip manager
		var mockZipManager = new Mock<IZipManager>();
		using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		var generateClosureResult1 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 2,
					Build = "Build0",
					Tool = "Tool0",
				},
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Cpp",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 5, Minor = 0, Patch = 0, },
							}
						},
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Package1",
								Owner = "User1",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
							}
						},
					}
				},
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				},
			},
		};
		var generateClosureResult2 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 1,
					Build = "Build0",
					Tool = "Tool0",
				},
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Wren",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
							}
						},
					}
				},
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				},
			},
		};
		var generateClosureResult3 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 2,
					Build = "Build0",
					Tool = "Tool0",
				},
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Wren",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
							}
						},
					}
				},
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Package2",
								Owner = "User1",
								Language = "C++",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 2, Minor = 3, Patch = 4, },
							}
						},
					}
				},
			},
		};
		var generateClosureResult4 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 1,
					Build = "Build0",
					Tool = "Tool0",
				},
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Cpp",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
							}
						},
					}
				},
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				},
			},
		};
		_ = mockMessageHandler
			.SetupSequence(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				It.IsAny<string>(),
				It.IsAny<string>()))
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult1)),
			})
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult2)),
			})
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult3)),
			})
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult4)),
			});
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/User1/Package1/versions/1.2.3/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package2/versions/2.3.4/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/mwasplund/Soup.Cpp/versions/5.0.0/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Soup.Wren/versions/6.0.0/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());

		var uut = new ClosureManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			new SemanticVersion(1, 2, 3),
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
				"DIAG: Build0:Wren mwasplund|Soup.Cpp -> 5.0.0",
				"DIAG: Build0:Wren User1|Package1 -> 1.2.3",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: MyPackage -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Cpp@5.0.0",
				"HIGH: Downloading package",
				"HIGH: Install Package: Wren User1 Package1@1.2.3",
				"HIGH: Downloading package",
				"INFO: Restore Packages for Closure Tool0",
				"DIAG: Create Directory: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/",
				"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml",
				"DIAG: Load Package Lock: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:Wren Soup.Cpp -> ./",
				"DIAG: Build0:Wren mwasplund|Soup.Wren -> 4.5.6",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language Wren",
				"INFO: Skip Package: Soup.Cpp -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Wren@4.5.6",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure",
				"DIAG: Create Directory: C:/LockStore/Wren/User1/Package1/1.2.3/",
				"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml",
				"DIAG: Load Package Lock: C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/PackageStore/Wren/User1/Package1/1.2.3/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:Wren Package1 -> ./",
				"DIAG: Build0:Wren mwasplund|Soup.Wren -> 4.5.6",
				"DIAG: Tool0:C++ User1|Package2 -> 2.3.4",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language Wren",
				"INFO: Skip Package: Package1 -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Wren@4.5.6",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"INFO: Restore Packages for Language C++",
				"HIGH: Install Package: C++ User1 Package2@2.3.4",
				"HIGH: Downloading package",
				"HIGH: Skip built in language version in build closure",
				"DIAG: Create Directory: C:/LockStore/Cpp/User1/Package2/2.3.4/",
				"INFO: Ensure Package Lock Exists: C:/LockStore/Cpp/User1/Package2/2.3.4/PackageLock.sml",
				"DIAG: Load Package Lock: C:/LockStore/Cpp/User1/Package2/2.3.4/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/PackageStore/Cpp/User1/Package2/2.3.4/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:C++ Package2 -> ./",
				"DIAG: Build0:Wren mwasplund|Soup.Cpp -> 3.2.1",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: Package2 -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Cpp@3.2.1",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure",
			},
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			new List<string>()
			{
				"Exists: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
				"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/",
				"OpenWriteTruncate: C:/Staging/Soup.Cpp.zip",
				"CreateDirectory: C:/Staging/Wren_Soup.Cpp_5.0.0/",
				"DeleteFile: C:/Staging/Soup.Cpp.zip",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp",
				"CreateDirectory: C:/PackageStore/Wren/mwasplund/Soup.Cpp",
				"Rename: [C:/Staging/Wren_Soup.Cpp_5.0.0/] -> [C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/]",
				"Exists: C:/PackageStore/Wren/User1/Package1/1.2.3/",
				"OpenWriteTruncate: C:/Staging/Package1.zip",
				"CreateDirectory: C:/Staging/Wren_Package1_1.2.3/",
				"DeleteFile: C:/Staging/Package1.zip",
				"Exists: C:/PackageStore/Wren/User1/Package1",
				"CreateDirectory: C:/PackageStore/Wren/User1/Package1",
				"Rename: [C:/Staging/Wren_Package1_1.2.3/] -> [C:/PackageStore/Wren/User1/Package1/1.2.3/]",
				"Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/",
				"CreateDirectory: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/",
				"Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/Recipe.sml",
				"OpenRead: C:/PackageStore/Wren/mwasplund/Soup.Cpp/5.0.0/Recipe.sml",
				"OpenWriteTruncate: C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml",
				"Exists: C:/LockStore/Wren/User1/Package1/1.2.3/",
				"CreateDirectory: C:/LockStore/Wren/User1/Package1/1.2.3/",
				"Exists: C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml",
				"Exists: C:/PackageStore/Wren/User1/Package1/1.2.3/Recipe.sml",
				"OpenRead: C:/PackageStore/Wren/User1/Package1/1.2.3/Recipe.sml",
				"OpenWriteTruncate: C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml",
				"Exists: C:/PackageStore/Cpp/User1/Package2/2.3.4/",
				"OpenWriteTruncate: C:/Staging/Package2.zip",
				"CreateDirectory: C:/Staging/C++_Package2_2.3.4/",
				"DeleteFile: C:/Staging/Package2.zip",
				"Exists: C:/PackageStore/Cpp/User1/Package2",
				"CreateDirectory: C:/PackageStore/Cpp/User1/Package2",
				"Rename: [C:/Staging/C++_Package2_2.3.4/] -> [C:/PackageStore/Cpp/User1/Package2/2.3.4/]",
				"Exists: C:/LockStore/Cpp/User1/Package2/2.3.4/",
				"CreateDirectory: C:/LockStore/Cpp/User1/Package2/2.3.4/",
				"Exists: C:/LockStore/Cpp/User1/Package2/2.3.4/PackageLock.sml",
				"Exists: C:/PackageStore/Cpp/User1/Package2/2.3.4/Recipe.sml",
				"OpenRead: C:/PackageStore/Cpp/User1/Package2/2.3.4/Recipe.sml",
				"OpenWriteTruncate: C:/LockStore/Cpp/User1/Package2/2.3.4/PackageLock.sml",
			},
			mockFileSystem.Requests);

		// Verify zip requests
		mockZipManager.Verify(zip => zip.ExtractToDirectory(
			new Path("C:/Staging/Soup.Cpp.zip"),
			new Path("C:/Staging/Wren_Soup.Cpp_5.0.0/")),
			Times.Once());
		mockZipManager.Verify(zip => zip.ExtractToDirectory(
			new Path("C:/Staging/Package1.zip"),
			new Path("C:/Staging/Wren_Package1_1.2.3/")),
			Times.Once());
		mockZipManager.Verify(zip => zip.ExtractToDirectory(
			new Path("C:/Staging/Package2.zip"),
			new Path("C:/Staging/C++_Package2_2.3.4/")),
			Times.Once());
		mockZipManager.VerifyNoOtherCalls();

		// Verify http requests
		var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 2,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 5, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>()
					{
						{
							"Build",
							new List<int>()
							{
								1,
							}
						},
					}
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>()
			{
				new Api.Client.PackagePublicReferenceModel()
				{
					Id = 1,
					Language = "Wren",
					Name = "Package1",
					Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
				},
			},
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest1Value),
			Times.Once());

		var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 1,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "Wren",
					Version = new Api.Client.SemanticVersionModel() { Major = 4, Minor = 5, Patch = 6, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>(),
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest2Value),
			Times.Exactly(1));

		var expectedGenerateRequest3 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 2,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "Wren",
					Version = new Api.Client.SemanticVersionModel() { Major = 6, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>()
				{
					{
						"Tool",
						new List<int>()
						{
							1,
						}
					},
				}
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>()
			{
				new Api.Client.PackagePublicReferenceModel()
				{
					Id = 1,
					Language = "C++",
					Name = "Package2",
					Version = new Api.Client.SemanticVersionModel() { Major = 2, Minor = 3, Patch = 4, },
				},
			},
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest3Value = JsonSerializer.Serialize(expectedGenerateRequest3);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest3Value),
			Times.Once());

		var expectedGenerateRequest4 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 1,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 5, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>(),
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest4Value = JsonSerializer.Serialize(expectedGenerateRequest4);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest4Value),
			Times.Once());

		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/User1/Package1/versions/1.2.3/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package2/versions/2.3.4/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/mwasplund/Soup.Cpp/versions/5.0.0/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();

		// Verify the contents of the package lock file
		var myPackageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
		_ = myPackageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var readerMyPackageLock = new System.IO.StreamReader(myPackageLock.Content);
		var myPackageLockContent = await readerMyPackageLock.ReadToEndAsync();
		var expectedMyPackageLock =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							MyPackage: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Cpp": { Version: "5.0.0" }
							"User1|Package1": { Version: "1.2.3" }
						}
					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedMyPackageLock, myPackageLockContent);

		var package1Lock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/User1/Package1/1.2.3/PackageLock.sml"));
		_ = package1Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var readerPackage1Lock = new System.IO.StreamReader(package1Lock.Content);
		var package1LockContent = await readerPackage1Lock.ReadToEndAsync();
		var expectedPackage1Lock =
			"""
				Version: 5
				Closures: {
					Root: {
						Wren: {
							Package1: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Wren": { Version: "4.5.6" }
						}
					}
					Tool0: {
						"C++": {
							"User1|Package2": { Version: "2.3.4" }
						}
					}
				}
				""";

		Assert.Equal(expectedPackage1Lock, package1LockContent);

		var package2Lock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Cpp/User1/Package2/2.3.4/PackageLock.sml"));
		_ = package2Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var readerPackage2Lock = new System.IO.StreamReader(package2Lock.Content);
		var package2LockContent = await readerPackage2Lock.ReadToEndAsync();
		var expectedPackage2Lock =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							Package2: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Cpp": { Version: "3.2.1" }
						}
					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedPackage2Lock, package2LockContent);

		var soupCppLock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/mwasplund/Soup.Cpp/5.0.0/PackageLock.sml"));
		_ = soupCppLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var readerSoupCppLock = new System.IO.StreamReader(soupCppLock.Content);
		var soupCppLockContent = await readerSoupCppLock.ReadToEndAsync();
		var expectedSoupCppLock =
			"""
				Version: 5
				Closures: {
					Root: {
						Wren: {
							"Soup.Cpp": { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Wren": { Version: "4.5.6" }
						}
					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedSoupCppLock, soupCppLockContent);
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
					Language: "Wren|4.5.6"
					Version: "1.2.3"
					"""))));

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		var generateClosureResult1 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 2,
					Build = "Build0",
					Tool = "Tool0",
				}
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Cpp",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
							}
						},
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							LocalId = 1,
						},
					}
				}
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				}
			},
		};
		var generateClosureResult2 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 1,
					Build = "Build0",
					Tool = "Tool0",
				}
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Wren",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
							}
						},
					}
				}
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				}
			},
		};
		_ = mockMessageHandler
			.SetupSequence(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				It.IsAny<string>(),
				It.IsAny<string>()))
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult1)),
			})
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult2)),
			});

		var uut = new ClosureManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			new SemanticVersion(1, 2, 3),
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
				"DIAG: Build0:Wren mwasplund|Soup.Cpp -> 3.2.1",
				"DIAG: Build0:Wren Package1 -> ../Package1/",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: MyPackage -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Cpp@3.2.1",
				"HIGH: Skip built in language version in build closure",
				"INFO: Skip Package: Package1 -> ../Package1/",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure",
				"INFO: Ensure Package Lock Exists: C:/Root/Package1/PackageLock.sml",
				"DIAG: Load Package Lock: C:/Root/Package1/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/Root/Package1/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:Wren Package1 -> ./",
				"DIAG: Build0:Wren mwasplund|Soup.Wren -> 4.5.6",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language Wren",
				"INFO: Skip Package: Package1 -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Wren@4.5.6",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure"
			},
			testListener.Messages);

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
			mockFileSystem.Requests);

		// Verify http requests
		var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 2,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>()
					{
						{
							"Build",
							new List<int>()
							{
								1,
							}
						},
					}
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>()
			{
				new Api.Client.PackageLocalReferenceModel()
				{
					Id = 1,
					Language = new Api.Client.LanguageReferenceModel()
					{
						Name = "Wren",
						Version = new Api.Client.SemanticVersionModel() { Major = 4, Minor = 5, Patch = 6, },
					},
					Dependencies = new Dictionary<string, ICollection<int>>(),
				},
			},
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest1Value),
			Times.Once());

		var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 1,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "Wren",
					Version = new Api.Client.SemanticVersionModel() { Major = 4, Minor = 5, Patch = 6, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>(),
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest2Value),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();

		// Verify the contents of the package lock file
		var packageLock1 = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
		_ = packageLock1.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var reader1 = new System.IO.StreamReader(packageLock1.Content);
		var packageLockContent1 = await reader1.ReadToEndAsync();
		var expectedPackageLock1 =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							MyPackage: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Cpp": { Version: "3.2.1" }
							Package1: { Version: "../Package1/" }
						}
					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedPackageLock1, packageLockContent1);

		var packageLock2 = mockFileSystem.GetMockFile(new Path("C:/Root/Package1/PackageLock.sml"));
		_ = packageLock2.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var reader2 = new System.IO.StreamReader(packageLock2.Content);
		var packageLockContent2 = await reader2.ReadToEndAsync();
		var expectedPackageLock2 =
			"""
				Version: 5
				Closures: {
					Root: {
						Wren: {
							Package1: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							"mwasplund|Soup.Wren": { Version: "4.5.6" }
						}
					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedPackageLock2, packageLockContent2);
	}

	[Fact]
	public async Task RestorePackagesAsync_HasLocalBuildDependency_WithToolDependency()
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
					Language: "Wren|4.5.6"
					Version: "1.2.3"
					Dependencies: {
						Tool: [
							{ Reference: "../Package2/" }
						]
					}
					"""))));

		mockFileSystem.CreateMockFile(
			new Path("C:/Root/Package2/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
					Name: "Package2"
					Language: "C++|3.2.1"
					Version: "2.3.4"
					"""))));

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		var generateClosureResult1 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 3,
					Build = "Build0",
					Tool = "Tool0",
				}
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							LocalId = 2,
						},
					}
				}
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							LocalId = 1,
						},
					}
				}
			},
		};

		var generateClosureResult2 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 2,
					Build = "Build0",
					Tool = "Tool0",
				}
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				}
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				}
			},
		};

		var generateClosureResult3 = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 1,
					Build = "Build0",
					Tool = "Tool0",
				}
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				}
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				}
			},
		};

		_ = mockMessageHandler
			.SetupSequence(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				It.IsAny<string>(),
				It.IsAny<string>()))
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult1)),
			})
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult2)),
			})
			.Returns(() => new HttpResponseMessage()
			{
				Content = new StringContent(JsonSerializer.Serialize(generateClosureResult3)),
			});

		var uut = new ClosureManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			new SemanticVersion(1, 2, 3),
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
				"DIAG: Load Recipe: C:/Root/Package2/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:C++ MyPackage -> ./",
				"DIAG: Build0:Wren Package1 -> ../Package1/",
				"DIAG: Tool0:C++ Package2 -> ../Package2/",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: MyPackage -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"INFO: Skip Package: Package1 -> ../Package1/",
				"INFO: Restore Packages for Closure Tool0",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: Package2 -> ../Package2/",
				"INFO: Ensure Package Lock Exists: C:/Root/Package1/PackageLock.sml",
				"DIAG: Load Package Lock: C:/Root/Package1/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/Root/Package1/Recipe.sml",
				"DIAG: Load Recipe: C:/Root/Package2/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:Wren Package1 -> ./",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language Wren",
				"INFO: Skip Package: Package1 -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Closure Tool0",
				"INFO: Ensure Package Lock Exists: C:/Root/Package2/PackageLock.sml",
				"DIAG: Load Package Lock: C:/Root/Package2/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/Root/Package2/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:C++ Package2 -> ./",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language C++",
				"INFO: Skip Package: Package2 -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Closure Tool0",
			},
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			new List<string>()
			{
				"Exists: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
				"Exists: C:/Root/Package1/Recipe.sml",
				"OpenRead: C:/Root/Package1/Recipe.sml",
				"Exists: C:/Root/Package2/Recipe.sml",
				"OpenRead: C:/Root/Package2/Recipe.sml",
				"OpenWriteTruncate: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/Root/Package1/PackageLock.sml",
				"Exists: C:/Root/Package1/Recipe.sml",
				"OpenRead: C:/Root/Package1/Recipe.sml",
				"Exists: C:/Root/Package2/Recipe.sml",
				"OpenRead: C:/Root/Package2/Recipe.sml",
				"OpenWriteTruncate: C:/Root/Package1/PackageLock.sml",
				"Exists: C:/Root/Package2/PackageLock.sml",
				"Exists: C:/Root/Package2/Recipe.sml",
				"OpenRead: C:/Root/Package2/Recipe.sml",
				"OpenWriteTruncate: C:/Root/Package2/PackageLock.sml",
			},
			mockFileSystem.Requests);

		// Verify http requests
		var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 3,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>()
				{
					{
						"Build",
						new List<int>()
						{
							2,
						}
					},
				},
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>()
			{
				new Api.Client.PackageLocalReferenceModel()
				{
					Id = 1,
					Language = new Api.Client.LanguageReferenceModel()
					{
						Name = "C++",
						Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					Dependencies = new Dictionary<string, ICollection<int>>(),
				},
				new Api.Client.PackageLocalReferenceModel()
				{
					Id = 2,
					Language = new Api.Client.LanguageReferenceModel()
					{
						Name = "Wren",
						Version = new Api.Client.SemanticVersionModel() { Major = 4, Minor = 5, Patch = 6, },
					},
					Dependencies = new Dictionary<string, ICollection<int>>()
					{
						{
							"Tool",
							new List<int>()
							{
								1,
							}
						},
					},
				},
			},
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest1Value),
			Times.Once());

		var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 2,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "Wren",
					Version = new Api.Client.SemanticVersionModel() { Major = 4, Minor = 5, Patch = 6, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>()
				{
					{
						"Tool",
						new List<int>()
						{
							1,
						}
					}
				},
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>()
			{
				new Api.Client.PackageLocalReferenceModel()
				{
					Id = 1,
					Language = new Api.Client.LanguageReferenceModel()
					{
						Name = "C++",
						Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					Dependencies = new Dictionary<string, ICollection<int>>(),
				},
			},
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest2Value),
			Times.Once());

		var expectedGenerateRequest3 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 1,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "C++",
					Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>(),
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest3Value = JsonSerializer.Serialize(expectedGenerateRequest3);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest3Value),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();

		// Verify the contents of the package lock file
		var myPackageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
		_ = myPackageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var myPackageLockReader = new System.IO.StreamReader(myPackageLock.Content);
		var myPackageLockContent = await myPackageLockReader.ReadToEndAsync();
		var expectedMyPackageLock =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							MyPackage: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						Wren: {
							Package1: { Version: "../Package1/" }
						}
					}
					Tool0: {
						"C++": {
							Package2: { Version: "../Package2/" }
						}
					}
				}
				""";

		Assert.Equal(expectedMyPackageLock, myPackageLockContent);

		var package1Lock = mockFileSystem.GetMockFile(new Path("C:/Root/Package1/PackageLock.sml"));
		_ = package1Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var package1LockReader = new System.IO.StreamReader(package1Lock.Content);
		var packageLock1Content = await package1LockReader.ReadToEndAsync();
		var expectedPackage1Lock =
			"""
				Version: 5
				Closures: {
					Root: {
						Wren: {
							Package1: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {

					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedPackage1Lock, packageLock1Content);

		var package2Lock = mockFileSystem.GetMockFile(new Path("C:/Root/Package2/PackageLock.sml"));
		_ = package2Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var package2LockReader = new System.IO.StreamReader(package2Lock.Content);
		var packageLock2Content = await package2LockReader.ReadToEndAsync();
		var expectedPackage2Lock =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							Package2: { Version: "./", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {

					}
					Tool0: {

					}
				}
				""";

		Assert.Equal(expectedPackage2Lock, packageLock2Content);
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
				Version: 5
				Closures: {
					Root: {
						"C++": {
							MyPackage: { Version: "./", Build: "Build0", Tool: "Tool0" }
							"User1|Package1": { Version: "1.2.4", Build: "Build0", Tool: "Tool0" }
							"User1|Package2": { Version: "3.2.1", Build: "Build0", Tool: "Tool0" }
						}
					}
					Build0: {
						"Wren": {
							"mwasplund|Soup.Cpp": { Version: "3.2.2" }
						}
					}
					Tool0: {
					}
				}
				""";
		using var originalContent = new System.IO.MemoryStream();
		await originalContent.WriteAsync(Encoding.UTF8.GetBytes(original));
		_ = originalContent.Seek(0, System.IO.SeekOrigin.Begin);
		mockFileSystem.CreateMockFile(
			new Path("C:/Root/MyPackage/PackageLock.sml"),
			new MockFile(originalContent));

		mockFileSystem.CreateMockFile(
			new Path("C:/PackageStore/Wren/mwasplund/Soup.Cpp/3.2.2/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
					Name: "Soup.Cpp"
					Language: "Wren|1.2.3"
					Version: "3.2.2"
					"""))));

		// Setup the mock zip manager
		var mockZipManager = new Mock<IZipManager>();
		using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package1/versions/1.2.4/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package2/versions/3.2.1/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/mwasplund/Soup.Cpp/versions/3.2.2/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());

		var generateClosureResult = new Api.Client.GenerateClosureResultModel()
		{
			Result = Api.Client.GenerateClosureResult.Success,
			RuntimeClosure = new List<Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel>()
			{
				new Api.Client.PackageLocalOrPublicExactReferenceWithSubGraphsModel()
				{
					LocalId = 1,
					Build = "Build0",
					Tool = "Tool0",
				},
			},
			BuildClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Build0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
					{
						new Api.Client.PackageLocalOrPublicExactReferenceModel()
						{
							Public = new Api.Client.PackagePublicExactReferenceModel()
							{
								Name = "Soup.Cpp",
								Owner = "mwasplund",
								Language = "Wren",
								Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
							}
						},
					}
				},
			},
			ToolClosures = new Dictionary<string, ICollection<Api.Client.PackageLocalOrPublicExactReferenceModel>>()
			{
				{
					"Tool0",
					new List<Api.Client.PackageLocalOrPublicExactReferenceModel>()
				},
			},
		};
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
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
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
				"HIGH: Install Package: C++ User1 Package1@1.2.4",
				"HIGH: Downloading package",
				"HIGH: Install Package: C++ User1 Package2@3.2.1",
				"HIGH: Downloading package",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Cpp@3.2.2",
				"HIGH: Downloading package",
				"INFO: Restore Packages for Closure Tool0",
				"DIAG: Create Directory: C:/LockStore/Wren/mwasplund/Soup.Cpp/3.2.2/",
				"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/3.2.2/PackageLock.sml",
				"DIAG: Load Package Lock: C:/LockStore/Wren/mwasplund/Soup.Cpp/3.2.2/PackageLock.sml",
				"INFO: Package Lock file does not exist.",
				"INFO: Discovering full closure",
				"DIAG: Load Recipe: C:/PackageStore/Wren/mwasplund/Soup.Cpp/3.2.2/Recipe.sml",
				"INFO: Generate final service closure",
				"DIAG: Root:Wren Soup.Cpp -> ./",
				"DIAG: Build0:Wren mwasplund|Soup.Cpp -> 3.2.1",
				"INFO: Restore Packages for Closure Root",
				"INFO: Restore Packages for Language Wren",
				"INFO: Skip Package: Soup.Cpp -> ./",
				"INFO: Restore Packages for Closure Build0",
				"INFO: Restore Packages for Language Wren",
				"HIGH: Install Package: Wren mwasplund Soup.Cpp@3.2.1",
				"HIGH: Skip built in language version in build closure",
				"INFO: Restore Packages for Closure Tool0",
				"HIGH: Skip built in language version in build closure"
			},
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			new List<string>()
			{
				"Exists: C:/Root/MyPackage/PackageLock.sml",
				"OpenRead: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/PackageStore/Cpp/User1/Package1/1.2.4/",
				"OpenWriteTruncate: C:/Staging/Package1.zip",
				"CreateDirectory: C:/Staging/C++_Package1_1.2.4/",
				"DeleteFile: C:/Staging/Package1.zip",
				"Exists: C:/PackageStore/Cpp/User1/Package1",
				"CreateDirectory: C:/PackageStore/Cpp/User1/Package1",
				"Rename: [C:/Staging/C++_Package1_1.2.4/] -> [C:/PackageStore/Cpp/User1/Package1/1.2.4/]",
				"Exists: C:/PackageStore/Cpp/User1/Package2/3.2.1/",
				"OpenWriteTruncate: C:/Staging/Package2.zip",
				"CreateDirectory: C:/Staging/C++_Package2_3.2.1/",
				"DeleteFile: C:/Staging/Package2.zip",
				"Exists: C:/PackageStore/Cpp/User1/Package2",
				"CreateDirectory: C:/PackageStore/Cpp/User1/Package2",
				"Rename: [C:/Staging/C++_Package2_3.2.1/] -> [C:/PackageStore/Cpp/User1/Package2/3.2.1/]",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp/3.2.2/",
				"OpenWriteTruncate: C:/Staging/Soup.Cpp.zip",
				"CreateDirectory: C:/Staging/Wren_Soup.Cpp_3.2.2/",
				"DeleteFile: C:/Staging/Soup.Cpp.zip",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp",
				"CreateDirectory: C:/PackageStore/Wren/mwasplund/Soup.Cpp",
				"Rename: [C:/Staging/Wren_Soup.Cpp_3.2.2/] -> [C:/PackageStore/Wren/mwasplund/Soup.Cpp/3.2.2/]",
				"Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/3.2.2/",
				"CreateDirectory: C:/LockStore/Wren/mwasplund/Soup.Cpp/3.2.2/",
				"Exists: C:/LockStore/Wren/mwasplund/Soup.Cpp/3.2.2/PackageLock.sml",
				"Exists: C:/PackageStore/Wren/mwasplund/Soup.Cpp/3.2.2/Recipe.sml",
				"OpenRead: C:/PackageStore/Wren/mwasplund/Soup.Cpp/3.2.2/Recipe.sml",
				"OpenWriteTruncate: C:/LockStore/Wren/mwasplund/Soup.Cpp/3.2.2/PackageLock.sml",
			},
			mockFileSystem.Requests);

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
			new Path("C:/Staging/Soup.Cpp.zip"), new Path("C:/Staging/Wren_Soup.Cpp_3.2.2/")),
			Times.Once());
		mockZipManager.VerifyNoOtherCalls();

		// Verify http requests
		var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
		{
			RootPackage = new Api.Client.PackageLocalReferenceModel()
			{
				Id = 1,
				Language = new Api.Client.LanguageReferenceModel()
				{
					Name = "Wren",
					Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				Dependencies = new Dictionary<string, ICollection<int>>(),
			},
			LocalPackages = new List<Api.Client.PackageLocalReferenceModel>(),
			PublicPackages = new List<Api.Client.PackagePublicReferenceModel>(),
			PreferredVersions = new List<Api.Client.PackagePublicExactReferenceModel>()
			{
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.CSharp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Cpp",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
				},
				new Api.Client.PackagePublicExactReferenceModel()
				{
					Language = "Wren",
					Owner = "mwasplund",
					Name = "Soup.Wren",
					Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
				},
			},
		};
		var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
				"{Accept: [application/json]}",
				expectedGenerateRequest1Value),
			Times.Once());

		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package1/versions/1.2.4/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/Package2/versions/3.2.1/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/Wren/mwasplund/Soup.Cpp/versions/3.2.2/download"),
				"{Accept: [application/json]}",
				null),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();
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
			new Path("C:/PackageStore/Cpp/User1/Package1/1.2.4/"),
			new List<DirectoryEntry>());
		mockFileSystem.RegisterChildren(
			new Path("C:/PackageStore/Cpp/User1/Package2/3.2.1/"),
			new List<DirectoryEntry>());

		// Create the original file
		var original =
			"""
				Version: 5
				Closures: {
					Root: {
						"C++": {
							MyPackage: { Version: "./", Build: "BuildSet0" }
							"User1|Package1": { Version: "1.2.4", Build: "BuildSet0" }
							"User1|Package2": { Version: "3.2.1", Build: "BuildSet0" }
						}
					}
					BuildSet0: {
						"C#": {
							"mwasplund|Soup.Cpp": { Version: "3.2.1" }
						}
					}
				}
				""";
		using var originalContent = new System.IO.MemoryStream();
		await originalContent.WriteAsync(Encoding.UTF8.GetBytes(original));
		_ = originalContent.Seek(0, System.IO.SeekOrigin.Begin);
		mockFileSystem.CreateMockFile(
			new Path("C:/Root/MyPackage/PackageLock.sml"),
			new MockFile(originalContent));

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		var uut = new ClosureManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			new SemanticVersion(1, 2, 3),
			new SemanticVersion(3, 2, 1),
			new SemanticVersion(4, 5, 6));

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
				"HIGH: Install Package: C++ User1 Package1@1.2.4",
				"HIGH: Found local version",
				"HIGH: Install Package: C++ User1 Package2@3.2.1",
				"HIGH: Found local version",
				"INFO: Restore Packages for Closure BuildSet0",
				"INFO: Restore Packages for Language C#",
				"HIGH: Install Package: C# mwasplund Soup.Cpp@3.2.1",
				"HIGH: Skip built in language version in build closure",
				"HIGH: Skip built in language version in build closure"
			},
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			new List<string>()
			{
				"Exists: C:/Root/MyPackage/PackageLock.sml",
				"OpenRead: C:/Root/MyPackage/PackageLock.sml",
				"Exists: C:/PackageStore/Cpp/User1/Package1/1.2.4/",
				"Exists: C:/PackageStore/Cpp/User1/Package2/3.2.1/",
			},
			mockFileSystem.Requests);

		// Verify http requests
		mockMessageHandler.VerifyNoOtherCalls();
	}
}