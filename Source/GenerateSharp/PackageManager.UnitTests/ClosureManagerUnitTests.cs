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
				RuntimeClosure = new List<Api.Client.PackageFeedExactReferenceWithBuildModel>(),
				BuildClosures = new List<Api.Client.BuildClosureExactModel>(),
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
					"DIAG: BuildSet0:Wren Soup.Cpp -> 3.2.1",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
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
			var expectedGenerateRequest = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequestValue = JsonSerializer.Serialize(expectedGenerateRequest);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequestValue),
				Times.Once());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expected =
				"""
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "MyPackage", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
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
			var expectedGenerateRequest = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>()
				{
					new Api.Client.PackageFeedReferenceModel()
					{
						Language = "C\u002B\u002B",
						Name = "Package1",
						Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedReferenceModel()
					{
						Language = "C\u002B\u002B",
						Name = "Package2",
						Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
					},
				},
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequestValue = JsonSerializer.Serialize(expectedGenerateRequest);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequestValue),
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
				RuntimeClosure = new List<Api.Client.PackageFeedExactReferenceWithBuildModel>(),
				BuildClosures = new List<Api.Client.BuildClosureExactModel>(),
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
					"DIAG: Root:C++ Package1 -> 1.2.3",
					"DIAG: Root:C++ Package2 -> 3.2.1",
					"DIAG: BuildSet0:Wren Soup.Cpp -> 3.2.1",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"HIGH: Install Package: C++ Package1@1.2.3",
					"HIGH: Downloading package",
					"HIGH: Install Package: C++ Package2@3.2.1",
					"HIGH: Downloading package",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
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
					"Exists: C:/PackageStore/Cpp/Package1/1.2.3/",
					"OpenWriteTruncate: C:/Staging/Package1.zip",
					"CreateDirectory: C:/Staging/C++_Package1_1.2.3/",
					"DeleteFile: C:/Staging/Package1.zip",
					"Exists: C:/PackageStore/Cpp/Package1",
					"CreateDirectory: C:/PackageStore/Cpp/Package1",
					"Rename: [C:/Staging/C++_Package1_1.2.3/] -> [C:/PackageStore/Cpp/Package1/1.2.3/]",
					"Exists: C:/PackageStore/Cpp/Package2/3.2.1/",
					"OpenWriteTruncate: C:/Staging/Package2.zip",
					"CreateDirectory: C:/Staging/C++_Package2_3.2.1/",
					"DeleteFile: C:/Staging/Package2.zip",
					"Exists: C:/PackageStore/Cpp/Package2",
					"CreateDirectory: C:/PackageStore/Cpp/Package2",
					"Rename: [C:/Staging/C++_Package2_3.2.1/] -> [C:/PackageStore/Cpp/Package2/3.2.1/]",

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
			var expectedGenerateRequest = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>()
				{
					new Api.Client.PackageFeedReferenceModel()
					{
						Language = "C\u002B\u002B",
						Name = "Package1",
						Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedReferenceModel()
					{
						Language = "C\u002B\u002B",
						Name = "Package2",
						Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
					},
				},
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name ="BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1 },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequestValue = JsonSerializer.Serialize(expectedGenerateRequest);
			mockMessageHandler.Verify(messageHandler =>
					messageHandler.Send(
						HttpMethod.Get,
						new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
						"{Accept: [application/json]}",
						expectedGenerateRequestValue),
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
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "MyPackage", Version: "./", Build: "BuildSet0" }
							{ Name: "Package1", Version: "1.2.3", Build: "" }
							{ Name: "Package2", Version: "3.2.1", Build: "" }
						]
					}
					BuildSet0: {
						Wren: [
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
				new Path("C:/PackageStore/Wren/Package1/1.2.3/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Package1"
					Language: "Wren|6"
					Version: "1.2.3"
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/Wren/Soup.Cpp/5.0.0/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Soup.Cpp"
					Language: "Wren|4.5.6"
					Version: "5.0.0"
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/Wren/Soup.Wren/6.0.0/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Soup.Wren"
					Language: "Wren|4.5.6"
					Version: "6.0.0"
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
				RuntimeClosure = new List<Api.Client.PackageFeedExactReferenceWithBuildModel>(),
				BuildClosures = new List<Api.Client.BuildClosureExactModel>(),
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
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Package1/versions/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Soup.Cpp/versions/5.0.0/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
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
					"DIAG: BuildSet0:Wren Soup.Cpp -> 5.0.0",
					"DIAG: BuildSet0:Wren Package1 -> 1.2.3",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Cpp@5.0.0",
					"HIGH: Downloading package",
					"HIGH: Install Package: Wren Package1@1.2.3",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/Wren/Soup.Cpp/5.0.0/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:Wren Soup.Cpp -> ./",
					"DIAG: BuildSet0:Wren Soup.Wren -> 4.5.6",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language Wren",
					"INFO: Skip Package: Soup.Cpp -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Wren@4.5.6",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
					"DIAG: Create Directory: C:/LockStore/Wren/Package1/1.2.3/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/Wren/Package1/1.2.3/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:Wren Package1 -> ./",
					"DIAG: BuildSet0:Wren Soup.Wren -> 6.0.0",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language Wren",
					"INFO: Skip Package: Package1 -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Wren@6.0.0",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/LockStore/Wren/Soup.Wren/6.0.0/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/Wren/Soup.Wren/6.0.0/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:Wren Soup.Wren -> ./",
					"DIAG: BuildSet0:Wren Soup.Wren -> 4.5.6",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language Wren",
					"INFO: Skip Package: Soup.Wren -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Wren@4.5.6",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
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
					"Exists: C:/PackageStore/Wren/Soup.Cpp/5.0.0/",
					"OpenWriteTruncate: C:/Staging/Soup.Cpp.zip",
					"CreateDirectory: C:/Staging/Wren_Soup.Cpp_5.0.0/",
					"DeleteFile: C:/Staging/Soup.Cpp.zip",
					"Exists: C:/PackageStore/Wren/Soup.Cpp",
					"CreateDirectory: C:/PackageStore/Wren/Soup.Cpp",
					"Rename: [C:/Staging/Wren_Soup.Cpp_5.0.0/] -> [C:/PackageStore/Wren/Soup.Cpp/5.0.0/]",
					"Exists: C:/PackageStore/Wren/Package1/1.2.3/",
					"OpenWriteTruncate: C:/Staging/Package1.zip",
					"CreateDirectory: C:/Staging/Wren_Package1_1.2.3/",
					"DeleteFile: C:/Staging/Package1.zip",
					"Exists: C:/PackageStore/Wren/Package1",
					"CreateDirectory: C:/PackageStore/Wren/Package1",
					"Rename: [C:/Staging/Wren_Package1_1.2.3/] -> [C:/PackageStore/Wren/Package1/1.2.3/]",
					"Exists: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"CreateDirectory: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"Exists: C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml",
					"Exists: C:/PackageStore/Wren/Soup.Cpp/5.0.0/Recipe.sml",
					"OpenRead: C:/PackageStore/Wren/Soup.Cpp/5.0.0/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml",
					"Exists: C:/LockStore/Wren/Package1/1.2.3/",
					"CreateDirectory: C:/LockStore/Wren/Package1/1.2.3/",
					"Exists: C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml",
					"Exists: C:/PackageStore/Wren/Package1/1.2.3/Recipe.sml",
					"OpenRead: C:/PackageStore/Wren/Package1/1.2.3/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml",
					"Exists: C:/PackageStore/Wren/Soup.Wren/6.0.0/",
					"OpenWriteTruncate: C:/Staging/Soup.Wren.zip",
					"CreateDirectory: C:/Staging/Wren_Soup.Wren_6.0.0/",
					"DeleteFile: C:/Staging/Soup.Wren.zip",
					"Exists: C:/PackageStore/Wren/Soup.Wren",
					"CreateDirectory: C:/PackageStore/Wren/Soup.Wren",
					"Rename: [C:/Staging/Wren_Soup.Wren_6.0.0/] -> [C:/PackageStore/Wren/Soup.Wren/6.0.0/]",
					"Exists: C:/LockStore/Wren/Soup.Wren/6.0.0/",
					"CreateDirectory: C:/LockStore/Wren/Soup.Wren/6.0.0/",
					"Exists: C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml",
					"Exists: C:/PackageStore/Wren/Soup.Wren/6.0.0/Recipe.sml",
					"OpenRead: C:/PackageStore/Wren/Soup.Wren/6.0.0/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml",
				},
				mockFileSystem.GetRequests());

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
				new Path("C:/Staging/Soup.Wren.zip"),
				new Path("C:/Staging/Wren_Soup.Wren_6.0.0/")),
				Times.Once());

			// Verify http requests
			var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Package1",
								Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
							},
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 5, Minor = 0, Patch = 0, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest1Value),
				Times.Once());

			var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Package1",
								Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
							},
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 5, Minor = 0, Patch = 0, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest2Value),
				Times.Exactly(1));

			var expectedGenerateRequest3 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Wren",
								Version = new Api.Client.SemanticVersionModel() { Major = 4, Minor = 5, Patch = 6, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest3Value = JsonSerializer.Serialize(expectedGenerateRequest3);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest3Value),
				Times.Exactly(2));

			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Package1/versions/1.2.3/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Soup.Cpp/versions/5.0.0/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Soup.Wren/versions/6.0.0/download"),
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
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "MyPackage", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Cpp", Version: "5.0.0" }
							{ Name: "Package1", Version: "1.2.3" }
						]
					}
				}
				""";

			Assert.Equal(expected, packageLockContent);

			var package1Lock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml"));
			package1Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerPackage1Lock = new System.IO.StreamReader(package1Lock.Content);
			var package1LockContent = await readerPackage1Lock.ReadToEndAsync();
			var expectedPackage1Lock =
				"""
				Version: 4
				Closures: {
					Root: {
						Wren: [
							{ Name: "Package1", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Wren", Version: "6.0.0" }
						]
					}
				}
				""";

			Assert.Equal(expectedPackage1Lock, package1LockContent);

			var soupCppLock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml"));
			soupCppLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerSoupCppLock = new System.IO.StreamReader(soupCppLock.Content);
			var soupCppLockContent = await readerSoupCppLock.ReadToEndAsync();
			var expectedSoupCppLock =
				"""
				Version: 4
				Closures: {
					Root: {
						Wren: [
							{ Name: "Soup.Cpp", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Wren", Version: "4.5.6" }
						]
					}
				}
				""";

			Assert.Equal(expectedSoupCppLock, soupCppLockContent);

			var soupCSharpLock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml"));
			soupCSharpLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerSoupCSharpLock = new System.IO.StreamReader(soupCSharpLock.Content);
			var soupCSharpLockContent = await readerSoupCSharpLock.ReadToEndAsync();
			var expectedSoupCSharpLock =
				"""
				Version: 4
				Closures: {
					Root: {
						Wren: [
							{ Name: "Soup.Wren", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Wren", Version: "4.5.6" }
						]
					}
				}
				""";

			Assert.Equal(expectedSoupCSharpLock, soupCSharpLockContent);
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
							{ Reference: "Package1@1.2.3" }
						]
					}
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/Wren/Package1/1.2.3/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Package1"
					Language: "Wren|6"
					Version: "1.2.3"
					Dependencies: {
						Build: [
							{ Reference: "C++|Package2@2.3.4" }
						]
					}
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/Cpp/Package2/2.3.4/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Package2"
					Language: "C++|5"
					Version: "2.3.4"
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/Wren/Soup.Cpp/5.0.0/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Soup.Cpp"
					Language: "Wren|4.5.6"
					Version: "5.0.0"
					"""))));

			mockFileSystem.CreateMockFile(
				new Path("C:/PackageStore/Wren/Soup.Wren/6.0.0/Recipe.sml"),
				new MockFile(new System.IO.MemoryStream(Encoding.UTF8.GetBytes(
					"""
					Name: "Soup.Wren"
					Language: "Wren|4.5.6"
					Version: "6.0.0"
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
				RuntimeClosure = new List<Api.Client.PackageFeedExactReferenceWithBuildModel>(),
				BuildClosures = new List<Api.Client.BuildClosureExactModel>(),
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
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Package1/versions/1.2.3/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package2/versions/2.3.4/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Soup.Cpp/versions/5.0.0/download"),
					It.IsAny<string>(),
					null))
				.Returns(() => new HttpResponseMessage());
			mockMessageHandler
				.Setup(messageHandler => messageHandler.Send(
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
					"DIAG: BuildSet0:Wren Soup.Cpp -> 5.0.0",
					"DIAG: BuildSet0:Wren Package1 -> 1.2.3",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Cpp@5.0.0",
					"HIGH: Downloading package",
					"HIGH: Install Package: Wren Package1@1.2.3",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/Wren/Soup.Cpp/5.0.0/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:Wren Soup.Cpp -> ./",
					"DIAG: BuildSet0:Wren Soup.Wren -> 4.5.6",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language Wren",
					"INFO: Skip Package: Soup.Cpp -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Wren@4.5.6",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
					"DIAG: Create Directory: C:/LockStore/Wren/Package1/1.2.3/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/Wren/Package1/1.2.3/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:Wren Package1 -> ./",
					"DIAG: BuildSet0:C++ Package2 -> 2.3.4",
					"DIAG: BuildSet0:Wren Soup.Wren -> 6.0.0",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language Wren",
					"INFO: Skip Package: Package1 -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language C++",
					"HIGH: Install Package: C++ Package2@2.3.4",
					"HIGH: Downloading package",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Wren@6.0.0",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/LockStore/Cpp/Package2/2.3.4/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/Cpp/Package2/2.3.4/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/Cpp/Package2/2.3.4/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/Cpp/Package2/2.3.4/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C++ Package2 -> ./",
					"DIAG: BuildSet0:Wren Soup.Cpp -> 5.0.0",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: Package2 -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Cpp@5.0.0",
					"HIGH: Found local version",
					"DIAG: Create Directory: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"INFO: Root already processed",
					"DIAG: Create Directory: C:/LockStore/Wren/Soup.Wren/6.0.0/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/Wren/Soup.Wren/6.0.0/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:Wren Soup.Wren -> ./",
					"DIAG: BuildSet0:Wren Soup.Wren -> 4.5.6",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language Wren",
					"INFO: Skip Package: Soup.Wren -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Wren@4.5.6",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
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
					"Exists: C:/PackageStore/Wren/Soup.Cpp/5.0.0/",
					"OpenWriteTruncate: C:/Staging/Soup.Cpp.zip",
					"CreateDirectory: C:/Staging/Wren_Soup.Cpp_5.0.0/",
					"DeleteFile: C:/Staging/Soup.Cpp.zip",
					"Exists: C:/PackageStore/Wren/Soup.Cpp",
					"CreateDirectory: C:/PackageStore/Wren/Soup.Cpp",
					"Rename: [C:/Staging/Wren_Soup.Cpp_5.0.0/] -> [C:/PackageStore/Wren/Soup.Cpp/5.0.0/]",
					"Exists: C:/PackageStore/Wren/Package1/1.2.3/",
					"OpenWriteTruncate: C:/Staging/Package1.zip",
					"CreateDirectory: C:/Staging/Wren_Package1_1.2.3/",
					"DeleteFile: C:/Staging/Package1.zip",
					"Exists: C:/PackageStore/Wren/Package1",
					"CreateDirectory: C:/PackageStore/Wren/Package1",
					"Rename: [C:/Staging/Wren_Package1_1.2.3/] -> [C:/PackageStore/Wren/Package1/1.2.3/]",
					"Exists: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"CreateDirectory: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"Exists: C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml",
					"Exists: C:/PackageStore/Wren/Soup.Cpp/5.0.0/Recipe.sml",
					"OpenRead: C:/PackageStore/Wren/Soup.Cpp/5.0.0/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml",
					"Exists: C:/LockStore/Wren/Package1/1.2.3/",
					"CreateDirectory: C:/LockStore/Wren/Package1/1.2.3/",
					"Exists: C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml",
					"Exists: C:/PackageStore/Wren/Package1/1.2.3/Recipe.sml",
					"OpenRead: C:/PackageStore/Wren/Package1/1.2.3/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml",
					"Exists: C:/PackageStore/Cpp/Package2/2.3.4/",
					"OpenWriteTruncate: C:/Staging/Package2.zip",
					"CreateDirectory: C:/Staging/C++_Package2_2.3.4/",
					"DeleteFile: C:/Staging/Package2.zip",
					"Exists: C:/PackageStore/Cpp/Package2",
					"CreateDirectory: C:/PackageStore/Cpp/Package2",
					"Rename: [C:/Staging/C++_Package2_2.3.4/] -> [C:/PackageStore/Cpp/Package2/2.3.4/]",
					"Exists: C:/PackageStore/Wren/Soup.Wren/6.0.0/",
					"OpenWriteTruncate: C:/Staging/Soup.Wren.zip",
					"CreateDirectory: C:/Staging/Wren_Soup.Wren_6.0.0/",
					"DeleteFile: C:/Staging/Soup.Wren.zip",
					"Exists: C:/PackageStore/Wren/Soup.Wren",
					"CreateDirectory: C:/PackageStore/Wren/Soup.Wren",
					"Rename: [C:/Staging/Wren_Soup.Wren_6.0.0/] -> [C:/PackageStore/Wren/Soup.Wren/6.0.0/]",
					"Exists: C:/LockStore/Cpp/Package2/2.3.4/",
					"CreateDirectory: C:/LockStore/Cpp/Package2/2.3.4/",
					"Exists: C:/LockStore/Cpp/Package2/2.3.4/PackageLock.sml",
					"Exists: C:/PackageStore/Cpp/Package2/2.3.4/Recipe.sml",
					"OpenRead: C:/PackageStore/Cpp/Package2/2.3.4/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/Cpp/Package2/2.3.4/PackageLock.sml",
					"Exists: C:/PackageStore/Wren/Soup.Cpp/5.0.0/",
					"Exists: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"CreateDirectory: C:/LockStore/Wren/Soup.Cpp/5.0.0/",
					"Exists: C:/LockStore/Wren/Soup.Wren/6.0.0/",
					"CreateDirectory: C:/LockStore/Wren/Soup.Wren/6.0.0/",
					"Exists: C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml",
					"Exists: C:/PackageStore/Wren/Soup.Wren/6.0.0/Recipe.sml",
					"OpenRead: C:/PackageStore/Wren/Soup.Wren/6.0.0/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml",
				},
				mockFileSystem.GetRequests());

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
				new Path("C:/Staging/Soup.Wren.zip"),
				new Path("C:/Staging/Wren_Soup.Wren_6.0.0/")),
				Times.Once());

			// Verify http requests
			var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Package1",
								Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
							},
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 5, Minor = 0, Patch = 0, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest1Value),
				Times.Once());

			var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Package1",
								Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
							},
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 5, Minor = 0, Patch = 0, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest2Value),
				Times.Exactly(1));

			var expectedGenerateRequest3 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Wren",
								Version = new Api.Client.SemanticVersionModel() { Major = 4, Minor = 5, Patch = 6, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest3Value = JsonSerializer.Serialize(expectedGenerateRequest3);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest3Value),
				Times.Exactly(2));

			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Package1/versions/1.2.3/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/C%2B%2B/packages/Package2/versions/2.3.4/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Soup.Cpp/versions/5.0.0/download"),
					"{Accept: [application/json]}",
					null),
				Times.Once());
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/languages/Wren/packages/Soup.Wren/versions/6.0.0/download"),
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
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "MyPackage", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Cpp", Version: "5.0.0" }
							{ Name: "Package1", Version: "1.2.3" }
						]
					}
				}
				""";

			Assert.Equal(expected, packageLockContent);

			var package1Lock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/Package1/1.2.3/PackageLock.sml"));
			package1Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerPackage1Lock = new System.IO.StreamReader(package1Lock.Content);
			var package1LockContent = await readerPackage1Lock.ReadToEndAsync();
			var expectedPackage1Lock =
				"""
				Version: 4
				Closures: {
					Root: {
						Wren: [
							{ Name: "Package1", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						"C++": [
							{ Name: "Package2", Version: "2.3.4" }
						]
						Wren: [
							{ Name: "Soup.Wren", Version: "6.0.0" }
						]
					}
				}
				""";

			Assert.Equal(expectedPackage1Lock, package1LockContent);

			var package2Lock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Cpp/Package2/2.3.4/PackageLock.sml"));
			package2Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerPackage2Lock = new System.IO.StreamReader(package2Lock.Content);
			var package2LockContent = await readerPackage2Lock.ReadToEndAsync();
			var expectedPackage2Lock =
				"""
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "Package2", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Cpp", Version: "5.0.0" }
						]
					}
				}
				""";

			Assert.Equal(expectedPackage2Lock, package2LockContent);

			var soupCppLock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/Soup.Cpp/5.0.0/PackageLock.sml"));
			soupCppLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerSoupCppLock = new System.IO.StreamReader(soupCppLock.Content);
			var soupCppLockContent = await readerSoupCppLock.ReadToEndAsync();
			var expectedSoupCppLock =
				"""
				Version: 4
				Closures: {
					Root: {
						Wren: [
							{ Name: "Soup.Cpp", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Wren", Version: "4.5.6" }
						]
					}
				}
				""";

			Assert.Equal(expectedSoupCppLock, soupCppLockContent);

			var soupCSharpLock = mockFileSystem.GetMockFile(new Path("C:/LockStore/Wren/Soup.Wren/6.0.0/PackageLock.sml"));
			soupCSharpLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var readerSoupCSharpLock = new System.IO.StreamReader(soupCSharpLock.Content);
			var soupCSharpLockContent = await readerSoupCSharpLock.ReadToEndAsync();
			var expectedSoupCSharpLock =
				"""
				Version: 4
				Closures: {
					Root: {
						Wren: [
							{ Name: "Soup.Wren", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Wren", Version: "4.5.6" }
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
					Language: "Wren|4.5.6"
					Version: "1.2.3"
					"""))));

			// Mock out the http
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var generateClosureResult = new Api.Client.GenerateClosureResultModel()
			{
				Result = Api.Client.GenerateClosureResult.Success,
				RuntimeClosure = new List<Api.Client.PackageFeedExactReferenceWithBuildModel>(),
				BuildClosures = new List<Api.Client.BuildClosureExactModel>(),
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
					"DIAG: BuildSet0:Wren Soup.Cpp -> 3.2.1",
					"DIAG: BuildSet0:Wren Package1 -> ../Package1/",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version in build closure",
					"INFO: Skip Package: Package1 -> ../Package1/",
					"HIGH: Skip built in language version in build closure",
					"INFO: Ensure Package Lock Exists: C:/Root/Package1/PackageLock.sml",
					"DIAG: Load Package Lock: C:/Root/Package1/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/Package1/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:Wren Package1 -> ./",
					"DIAG: BuildSet0:Wren Soup.Wren -> 4.5.6",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language Wren",
					"INFO: Skip Package: Package1 -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Wren@4.5.6",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
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
			var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest1Value),
				Times.Once());

			var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest2Value),
				Times.Once());

			// Verify the contents of the package lock file
			var packageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			packageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(packageLock.Content);
			var packageLockContent = await reader.ReadToEndAsync();
			var expectedPackageLock =
				"""
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "MyPackage", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
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
				Version: 4
				Closures: {
					Root: {
						Wren: [
							{ Name: "Package1", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Wren", Version: "4.5.6" }
						]
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
			var mockMessageHandler = new Mock<ShimHttpMessageHandler>() { CallBase = true, };
			using var httpClient = new HttpClient(mockMessageHandler.Object);

			var generateClosureResult = new Api.Client.GenerateClosureResultModel()
			{
				Result = Api.Client.GenerateClosureResult.Success,
				RuntimeClosure = new List<Api.Client.PackageFeedExactReferenceWithBuildModel>(),
				BuildClosures = new List<Api.Client.BuildClosureExactModel>(),
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
					"DIAG: BuildSet0:Wren Soup.Cpp -> 3.2.1",
					"DIAG: BuildSet0:Wren Package1 -> ../Package1/",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C++",
					"INFO: Skip Package: MyPackage -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version in build closure",
					"INFO: Skip Package: Package1 -> ../Package1/",
					"HIGH: Skip built in language version in build closure",
					"INFO: Ensure Package Lock Exists: C:/Root/Package1/PackageLock.sml",
					"DIAG: Load Package Lock: C:/Root/Package1/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/Root/Package1/Recipe.sml",
					"DIAG: Load Recipe: C:/Root/Package2/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:Wren Package1 -> ./",
					"DIAG: BuildSet0:Wren Soup.Wren -> 4.5.6",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language Wren",
					"INFO: Skip Package: Package1 -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.Wren@4.5.6",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
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
					"Exists: C:/Root/Package2/Recipe.sml",
					"OpenRead: C:/Root/Package2/Recipe.sml",
					"OpenWriteTruncate: C:/Root/Package1/PackageLock.sml",
				},
				mockFileSystem.GetRequests());

			// Verify http requests
			var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest1Value),
				Times.Once());

			var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.Cpp",
								Version = new Api.Client.SemanticVersionModel() { Major = 3, Minor = 2, Patch = 1, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest2Value),
				Times.Once());

			// Verify the contents of the package lock file
			var myPackageLock = mockFileSystem.GetMockFile(new Path("C:/Root/MyPackage/PackageLock.sml"));
			myPackageLock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var reader = new System.IO.StreamReader(myPackageLock.Content);
			var myPackageLockContent = await reader.ReadToEndAsync();
			var expectedMyPackageLock =
				"""
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "MyPackage", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Cpp", Version: "3.2.1" }
							{ Name: "Package1", Version: "../Package1/" }
						]
					}
				}
				""";

			Assert.Equal(expectedMyPackageLock, myPackageLockContent);

			var package1Lock = mockFileSystem.GetMockFile(new Path("C:/Root/Package1/PackageLock.sml"));
			package1Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			using var package1LockReader = new System.IO.StreamReader(package1Lock.Content);
			var packageLock1Content = await package1LockReader.ReadToEndAsync();
			var expectedPackage1Lock =
				"""
				Version: 4
				Closures: {
					Root: {
						Wren: [
							{ Name: "Package1", Version: "./", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						Wren: [
							{ Name: "Soup.Wren", Version: "4.5.6" }
						]
					}
				}
				""";

			Assert.Equal(expectedPackage1Lock, packageLock1Content);

			////var package2Lock = mockFileSystem.GetMockFile(new Path("C:/Root/Package2/PackageLock.sml"));
			////package2Lock.Content.Seek(0, System.IO.SeekOrigin.Begin);
			////using var package2LockReader = new System.IO.StreamReader(package2Lock.Content);
			////var packageLock2Content = await package1LockReader.ReadToEndAsync();
			////var expectedPackage2Lock =
			////	"""
			////	Version: 4
			////	Closures: {
			////		Root: {
			////			Wren: [
			////				{ Name: "Package1", Version: "./", Build: "BuildSet0" }
			////			]
			////		}
			////		BuildSet0: {
			////			Wren: [
			////				{ Name: "Soup.Wren", Version: "4.5.6" }
			////			]
			////		}
			////	}
			////	""";

			// TODO: Assert.Equal(expectedPackage2Lock, packageLock2Content);
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
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "MyPackage", Version: "./", Build: "BuildSet0" }
							{ Name: "Package1", Version: "1.2.4", Build: "BuildSet0" }
							{ Name: "Package2", Version: "3.2.1", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						"C#": [
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
				new Path("C:/PackageStore/CSharp/Soup.Cpp/3.2.2/Recipe.sml"),
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
				RuntimeClosure = new List<Api.Client.PackageFeedExactReferenceWithBuildModel>(),
				BuildClosures = new List<Api.Client.BuildClosureExactModel>(),
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
					"HIGH: Install Package: C++ Package1@1.2.4",
					"HIGH: Downloading package",
					"HIGH: Install Package: C++ Package2@3.2.1",
					"HIGH: Downloading package",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.Cpp@3.2.2",
					"HIGH: Downloading package",
					"DIAG: Create Directory: C:/LockStore/CSharp/Soup.Cpp/3.2.2/",
					"INFO: Ensure Package Lock Exists: C:/LockStore/CSharp/Soup.Cpp/3.2.2/PackageLock.sml",
					"DIAG: Load Package Lock: C:/LockStore/CSharp/Soup.Cpp/3.2.2/PackageLock.sml",
					"INFO: Package Lock file does not exist.",
					"INFO: Discovering full closure",
					"DIAG: Load Recipe: C:/PackageStore/CSharp/Soup.Cpp/3.2.2/Recipe.sml",
					"INFO: Generate final service closure",
					"DIAG: Root:C# Soup.Cpp -> ./",
					"DIAG: BuildSet0:Wren Soup.CSharp -> 1.2.3",
					"INFO: Restore Packages for Closure Root",
					"INFO: Restore Packages for Language C#",
					"INFO: Skip Package: Soup.Cpp -> ./",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language Wren",
					"HIGH: Install Package: Wren Soup.CSharp@1.2.3",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"OpenRead: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/PackageStore/Cpp/Package1/1.2.4/",
					"OpenWriteTruncate: C:/Staging/Package1.zip",
					"CreateDirectory: C:/Staging/C++_Package1_1.2.4/",
					"DeleteFile: C:/Staging/Package1.zip",
					"Exists: C:/PackageStore/Cpp/Package1",
					"CreateDirectory: C:/PackageStore/Cpp/Package1",
					"Rename: [C:/Staging/C++_Package1_1.2.4/] -> [C:/PackageStore/Cpp/Package1/1.2.4/]",
					"Exists: C:/PackageStore/Cpp/Package2/3.2.1/",
					"OpenWriteTruncate: C:/Staging/Package2.zip",
					"CreateDirectory: C:/Staging/C++_Package2_3.2.1/",
					"DeleteFile: C:/Staging/Package2.zip",
					"Exists: C:/PackageStore/Cpp/Package2",
					"CreateDirectory: C:/PackageStore/Cpp/Package2",
					"Rename: [C:/Staging/C++_Package2_3.2.1/] -> [C:/PackageStore/Cpp/Package2/3.2.1/]",
					"Exists: C:/PackageStore/CSharp/Soup.Cpp/3.2.2/",
					"OpenWriteTruncate: C:/Staging/Soup.Cpp.zip",
					"CreateDirectory: C:/Staging/C#_Soup.Cpp_3.2.2/",
					"DeleteFile: C:/Staging/Soup.Cpp.zip",
					"Exists: C:/PackageStore/CSharp/Soup.Cpp",
					"CreateDirectory: C:/PackageStore/CSharp/Soup.Cpp",
					"Rename: [C:/Staging/C#_Soup.Cpp_3.2.2/] -> [C:/PackageStore/CSharp/Soup.Cpp/3.2.2/]",
					"Exists: C:/LockStore/CSharp/Soup.Cpp/3.2.2/",
					"CreateDirectory: C:/LockStore/CSharp/Soup.Cpp/3.2.2/",
					"Exists: C:/LockStore/CSharp/Soup.Cpp/3.2.2/PackageLock.sml",
					"Exists: C:/PackageStore/CSharp/Soup.Cpp/3.2.2/Recipe.sml",
					"OpenRead: C:/PackageStore/CSharp/Soup.Cpp/3.2.2/Recipe.sml",
					"OpenWriteTruncate: C:/LockStore/CSharp/Soup.Cpp/3.2.2/PackageLock.sml",
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
			var expectedGenerateRequest1 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.CSharp",
								Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest1Value = JsonSerializer.Serialize(expectedGenerateRequest1);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest1Value),
				Times.Once());

			var expectedGenerateRequest2 = new Api.Client.GenerateClosureRequestModel()
			{
				RuntimePackages = new List<Api.Client.PackageFeedReferenceModel>(),
				BuildClosures = new List<Api.Client.BuildClosureModel>()
				{
					new Api.Client.BuildClosureModel()
					{
						Name = "BuildSet0",
						Closure = new List<Api.Client.PackageFeedReferenceModel>()
						{
							new Api.Client.PackageFeedReferenceModel()
							{
								Language = "Wren",
								Name = "Soup.CSharp",
								Version = new Api.Client.SemanticVersionModel() { Major = 1, Minor = 2, Patch = 3, },
							},
						},
					},
				},
				RequestedVersions = new List<Api.Client.PackageFeedExactReferenceModel>()
				{
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.CSharp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 1, Minor = 2, Patch = 3, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Cpp",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 3, Minor = 2, Patch = 1, },
					},
					new Api.Client.PackageFeedExactReferenceModel()
					{
						Language = "Wren",
						Name = "Soup.Wren",
						Version = new Api.Client.SemanticVersionExactModel() { Major = 4, Minor = 5, Patch = 6, },
					},
				},
			};
			var expectedGenerateRequest2Value = JsonSerializer.Serialize(expectedGenerateRequest2);
			mockMessageHandler.Verify(messageHandler =>
				messageHandler.Send(
					HttpMethod.Get,
					new Uri("https://test.api.soupbuild.com/v1/closure/generate"),
					"{Accept: [application/json]}",
					expectedGenerateRequest2Value),
				Times.Once());

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
				new Path("C:/PackageStore/Cpp/Package1/1.2.4/"),
				new List<DirectoryEntry>());
			mockFileSystem.RegisterChildren(
				new Path("C:/PackageStore/Cpp/Package2/3.2.1/"),
				new List<DirectoryEntry>());

			// Create the original file
			var original =
				"""
				Version: 4
				Closures: {
					Root: {
						"C++": [
							{ Name: "MyPackage", Version: "./", Build: "BuildSet0" }
							{ Name: "Package1", Version: "1.2.4", Build: "BuildSet0" }
							{ Name: "Package2", Version: "3.2.1", Build: "BuildSet0" }
						]
					}
					BuildSet0: {
						"C#": [
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
					"HIGH: Install Package: C++ Package1@1.2.4",
					"HIGH: Found local version",
					"HIGH: Install Package: C++ Package2@3.2.1",
					"HIGH: Found local version",
					"INFO: Restore Packages for Closure BuildSet0",
					"INFO: Restore Packages for Language C#",
					"HIGH: Install Package: C# Soup.Cpp@3.2.1",
					"HIGH: Skip built in language version in build closure",
					"HIGH: Skip built in language version in build closure",
				},
				testListener.GetMessages());

			// Verify expected file system requests
			Assert.Equal(
				new List<string>()
				{
					"Exists: C:/Root/MyPackage/PackageLock.sml",
					"OpenRead: C:/Root/MyPackage/PackageLock.sml",
					"Exists: C:/PackageStore/Cpp/Package1/1.2.4/",
					"Exists: C:/PackageStore/Cpp/Package2/3.2.1/",
				},
				mockFileSystem.GetRequests());
		}
	}
}
