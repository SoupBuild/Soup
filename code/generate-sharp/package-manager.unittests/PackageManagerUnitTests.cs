// <copyright file="PackageManagerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Moq;
using Opal;
using Opal.System;
using System;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using Xunit;
using Path = Opal.Path;

namespace Soup.Build.PackageManager.UnitTests;

[Collection("Opal")]
public class PackageManagerUnitTests
{
	[Fact]
	public async Task RestorePackagesAsync()
	{
		// Register the test listener
		var testListener = new TestTraceListener();
		using var scopedTraceListener = new ScopedTraceListenerRegister(testListener);

		// Setup the mock file system
		var mockFileSystem = new MockFileSystem();
		using var scopedFileSystem = new ScopedSingleton<IFileSystem>(mockFileSystem);

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		// Mock out the closure manager
		var mockClosureManager = new Mock<IClosureManager>(MockBehavior.Strict);
		_ = mockClosureManager
			.Setup(manager => manager.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/Users/Me/.soup/packages/"),
				new Path("C:/Users/Me/.soup/locks/"),
				new Path("C:/Users/Me/.soup/packages/.staging/")))
			.Returns(() => Task.CompletedTask);

		var uut = new PackageManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			mockClosureManager.Object);

		var workingDirectory = new Path("C:/Root/MyPackage/");
		await uut.RestorePackagesAsync(workingDirectory);

		// Verify expected logs
		Assert.Equal(
			[
				"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
				"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
				"DIAG: Deleting staging directory",
			],
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			[
				"GetUserProfileDirectory",
				"Exists: C:/Users/Me/.soup/packages/.staging/",
				"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
				"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
			],
			mockFileSystem.Requests);

		// Verify http requests
		mockMessageHandler.VerifyNoOtherCalls();

		// Verify closure manager requests
		mockClosureManager.Verify(manager =>
			manager.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/Users/Me/.soup/packages/"),
				new Path("C:/Users/Me/.soup/locks/"),
				new Path("C:/Users/Me/.soup/packages/.staging/")),
			Times.Once());
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
			"""
			Name: 'MyPackage'
			Language: (C++@3.2)
			Version: 1.0.0
			"""));
		_ = originalContent.Seek(0, System.IO.SeekOrigin.Begin);
		mockFileSystem.CreateMockFile(
			new Path("C:/Root/MyPackage/Recipe.sml"),
			new MockFile(originalContent));

		mockFileSystem.CreateMockFile(
			new Path("C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
				Name: 'Package1'
				Language: (C++@3.2)
				Version: 1.2.3
				"""))));

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		// Mock out the closure manager
		var mockClosureManager = new Mock<IClosureManager>(MockBehavior.Strict);
		_ = mockClosureManager
			.Setup(manager => manager.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/Users/Me/.soup/packages/"),
				new Path("C:/Users/Me/.soup/locks/"),
				new Path("C:/Users/Me/.soup/packages/.staging/")))
			.Returns(() => Task.CompletedTask);

		var uut = new PackageManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			mockClosureManager.Object);

		var workingDirectory = new Path("C:/Root/MyPackage/");
		var packageReference = "User1|OtherPackage@1.2.3";
		await uut.InstallPackageReferenceAsync(workingDirectory, packageReference);

		// Verify expected logs
		Assert.Equal(
			[
				"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
				"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
				"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
				"INFO: Adding reference to recipe",
				"INFO: Deleting staging directory",
			],
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			[
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
				"GetUserProfileDirectory",
				"OpenWriteTruncate: C:/Root/MyPackage/Recipe.sml",
				"Exists: C:/Users/Me/.soup/packages/.staging/",
				"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
				"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
			],
			mockFileSystem.Requests);

		// Verify http requests
		mockMessageHandler.VerifyNoOtherCalls();

		// Verify closure manager requests
		mockClosureManager.Verify(manager =>
			manager.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/Users/Me/.soup/packages/"),
				new Path("C:/Users/Me/.soup/locks/"),
				new Path("C:/Users/Me/.soup/packages/.staging/")),
			Times.Once());

		// Verify the contents of the recipe file
		var recipeFile = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/Recipe.sml"));
		_ = recipeFile.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var recipeReader = new System.IO.StreamReader(recipeFile.Content);
		var recipeContent = await recipeReader.ReadToEndAsync();
		var expectedRecipe =
			"""
			Name: 'MyPackage'
			Language: (C++@3.2)
			Version: 1.0.0
			Dependencies: {
				Runtime: [
					'User1|OtherPackage@1.2.3'
				]
			}
			""";

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
			"""
			Name: 'MyPackage'
			Language: (C++@3.2)
			Version: 1.0.0
			"""));
		_ = originalContent.Seek(0, System.IO.SeekOrigin.Begin);
		mockFileSystem.CreateMockFile(
			new Path("C:/Root/MyPackage/Recipe.sml"),
			new MockFile(originalContent));

		mockFileSystem.CreateMockFile(
			new Path("C:/Users/Me/.soup/packages/C++/OtherPackage/1.2.3/Recipe.sml"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
				"""
				Name: 'Package1'
				Language: (C++@3.2)
				Version: 1.2.3
				"""))));

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		// Mock out the closure manager
		var mockClosureManager = new Mock<IClosureManager>(MockBehavior.Strict);
		_ = mockClosureManager
			.Setup(manager => manager.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/Users/Me/.soup/packages/"),
				new Path("C:/Users/Me/.soup/locks/"),
				new Path("C:/Users/Me/.soup/packages/.staging/")))
			.Returns(() => Task.CompletedTask);

		var uut = new PackageManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			mockClosureManager.Object);

		var getPackageResponse = JsonSerializer.Serialize(new Api.Client.PackageModel()
		{
			Name = "OtherPackage",
			Owner = "User1",
			Latest = new Api.Client.SemanticVersion() { Major = 1, Minor = 2, Patch = 3, },
		});
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/OtherPackage"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage() { Content = new StringContent(getPackageResponse) });
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/OtherPackage/versions/1.2.3/download"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage());

		mockMessageHandler.VerifyNoOtherCalls();

		var workingDirectory = new Path("C:/Root/MyPackage/");
		var packageReference = "User1|OtherPackage";
		await uut.InstallPackageReferenceAsync(workingDirectory, packageReference);

		// Verify expected logs
		Assert.Equal(
			[
				"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
				"DIAG: Using Package Store: C:/Users/Me/.soup/packages/",
				"DIAG: Using Lock Store: C:/Users/Me/.soup/locks/",
				"HIGH: Latest Version: 1.2.3",
				"INFO: Adding reference to recipe",
				"INFO: Deleting staging directory",
			],
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			[
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
				"GetUserProfileDirectory",
				"OpenWriteTruncate: C:/Root/MyPackage/Recipe.sml",
				"Exists: C:/Users/Me/.soup/packages/.staging/",
				"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
				"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
			],
			mockFileSystem.Requests);

		// Verify http requests
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/User1/OtherPackage"),
				"{Accept: [application/json]}",
				null),
			Times.Once());

		// Verify closure manager requests
		mockClosureManager.Verify(manager =>
			manager.GenerateAndRestoreRecursiveLocksAsync(
				new Path("C:/Root/MyPackage/"),
				new Path("C:/Users/Me/.soup/packages/"),
				new Path("C:/Users/Me/.soup/locks/"),
				new Path("C:/Users/Me/.soup/packages/.staging/")),
			Times.Once());

		// Verify the contents of the recipe file
		var recipeFile = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/Recipe.sml"));
		_ = recipeFile.Content.Seek(0, System.IO.SeekOrigin.Begin);
		using var recipeReader = new System.IO.StreamReader(recipeFile.Content);
		var recipeContent = await recipeReader.ReadToEndAsync();
		var expectedRecipe =
			"""
			Name: 'MyPackage'
			Language: (C++@3.2)
			Version: 1.0.0
			Dependencies: {
				Runtime: [
					'User1|OtherPackage@1.2.3'
				]
			}
			""";

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
			"""
			Name: 'MyPackage'
			Language: (C++@0.1)
			Version: 1.0.0
			"""));
		_ = originalContent.Seek(0, System.IO.SeekOrigin.Begin);
		mockFileSystem.CreateMockFile(
			new Path("C:/Root/MyPackage/Recipe.sml"),
			new MockFile(originalContent));

		// Pretend that there is a zip file created
		mockFileSystem.CreateMockFile(
			new Path("C:/Users/Me/.soup/packages/.staging/MyPackage.zip"),
			new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes("ZIP_FILE_CONTENT"))));

		mockFileSystem.RegisterChildren(
			new Path("C:/Root/MyPackage/"),
			[
				new DirectoryEntry()
				{
					Path = new Path("C:/Root/MyPackage/Recipe.sml"),
					IsDirectory = false,
				},
			]);

		// Setup the mock authentication manager
		var mockAuthenticationManager = new Mock<IAuthenticationManager>();
		using var scopedAuthenticationManager = new ScopedSingleton<IAuthenticationManager>(mockAuthenticationManager.Object);

		// Setup the mock zip manager
		var mockZipManager = new Mock<IZipManager>();
		using var scopedZipManager = new ScopedSingleton<IZipManager>(mockZipManager.Object);

		// Create mock archive
		var mockZipArchive = new Mock<IZipArchive>();
		_ = mockZipManager.Setup(zip => zip.OpenCreate(It.IsAny<Path>())).Returns(mockZipArchive.Object);

		// Mock out the http
		var mockMessageHandler = new Mock<IHttpMessageHandler>();
		using var shimHandler = new ShimHttpMessageHandler(mockMessageHandler.Object);
		using var httpClient = new HttpClient(shimHandler);

		// Mock out the closure manager
		var mockClosureManager = new Mock<IClosureManager>(MockBehavior.Strict);

		var uut = new PackageManager(
			new Uri("https://test.api.soupbuild.com/"),
			httpClient,
			mockClosureManager.Object);

		var getPackageResponse = JsonSerializer.Serialize(new Api.Client.PackageModel()
		{
			Name = "MyPackage",
			Owner = "User1",
			Latest = new Api.Client.SemanticVersion() { Major = 1, Minor = 2, Patch = 3, },
		});
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/_/MyPackage"),
				It.IsAny<string>(),
				null))
			.Returns(() => new HttpResponseMessage() { Content = new StringContent(getPackageResponse) });
		_ = mockMessageHandler
			.Setup(messageHandler => messageHandler.SendAsync(
				HttpMethod.Put,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/_/MyPackage/versions/1.0.0"),
				It.IsAny<string>(),
				It.IsAny<string>()))
			.Returns(() => new HttpResponseMessage(HttpStatusCode.Created));

		var workingDirectory = new Path("C:/Root/MyPackage/");
		await uut.PublishPackageAsync(workingDirectory);

		// Verify expected logs
		Assert.Equal(
			[
				"INFO: Publish Project: C:/Root/MyPackage/",
				"DIAG: Load Recipe: C:/Root/MyPackage/Recipe.sml",
				"INFO: Using Package Store: C:/Users/Me/.soup/packages/",
				"INFO: Request Authentication Token",
				"INFO: Publish package",
				"INFO: Package published",
				"INFO: Cleanup staging directory",
			],
			testListener.Messages);

		// Verify expected file system requests
		Assert.Equal(
			[
				"Exists: C:/Root/MyPackage/Recipe.sml",
				"OpenRead: C:/Root/MyPackage/Recipe.sml",
				"GetUserProfileDirectory",
				"Exists: C:/Users/Me/.soup/packages/.staging/",
				"CreateDirectory: C:/Users/Me/.soup/packages/.staging/",
				"GetChildren: C:/Root/MyPackage/",
				"OpenRead: C:/Users/Me/.soup/packages/.staging/MyPackage.zip",
				"DeleteDirectoryRecursive: C:/Users/Me/.soup/packages/.staging/",
			],
			mockFileSystem.Requests);

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
			messageHandler.SendAsync(
				HttpMethod.Get,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/_/MyPackage"),
				"{Accept: [application/json]}",
				null),
			Times.Once());
		mockMessageHandler.Verify(messageHandler =>
			messageHandler.SendAsync(
				HttpMethod.Put,
				new Uri("https://test.api.soupbuild.com/v1/packages/C%2B%2B/_/MyPackage/versions/1.0.0"),
				"",
				"ZIP_FILE_CONTENT"),
			Times.Once());

		mockMessageHandler.VerifyNoOtherCalls();
	}
}