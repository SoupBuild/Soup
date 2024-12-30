#include "nanobench.h"
#include <set>

import Monitor.Host;
import Opal;
import Soup.Core;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Core;

int main()
{
	{
		ankerl::nanobench::Bench().minEpochIterations(10000).run("PackageReference Parse Name Only", [&]
		{
			auto actual = PackageReference::Parse("Package1");
			ankerl::nanobench::doNotOptimizeAway(actual);
		});
	}

	{
		ankerl::nanobench::Bench().minEpochIterations(10000).run("PackageReference Parse Language, User, Name and Version", [&]
		{
			auto actual = PackageReference::Parse("[C#]User1|Package1@1.2.3");
			ankerl::nanobench::doNotOptimizeAway(actual);
		});
	}

	{
		auto binaryFileContent = std::vector<unsigned char>(
		{
			'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
			'T', 'B', 'L', '\0', 0x00, 0x00, 0x00, 0x00,
		});
		auto content = std::stringstream(std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()));

		ankerl::nanobench::Bench().minEpochIterations(10000).run("ValueTableReader Deserialize Empty", [&]
		{
			auto actual = ValueTableReader::Deserialize(content);
			ankerl::nanobench::doNotOptimizeAway(actual);
		});
	}

	{
		auto binaryFileContent = std::vector<unsigned char>(
		{
			'B', 'V', 'T', '\0', 0x02, 0x00, 0x00, 0x00,
			'T', 'B', 'L', '\0', 0x08, 0x00, 0x00, 0x00,
			0x0b, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'B', 'o', 'o', 'l', 'e', 'a', 'n', 0x6, 0x0, 0x0, 0x0, 0x1, 0x00, 0x00, 0x00,
			0x0d, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'D', 'e', 'e', 'p', 'T', 'a', 'b', 'l', 'e', 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e', '1', 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e', '2', 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e', '3', 0x1, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e', '4', 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
			0x0d, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'E', 'm', 'p', 't', 'y', 'L', 'i', 's', 't', 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
			0x0e, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'E', 'm', 'p', 't', 'y', 'T', 'a', 'b', 'l', 'e', 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
			0x09, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'F', 'l', 'o', 'a', 't', 0x5, 0x0, 0x0, 0x0, 0xae, 0x47, 0xe1, 0x7a, 0x14, 0xae, 0xf3, 0x3f,
			0x0b, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'I', 'n', 't', 'e', 'g', 'e', 'r', 0x4, 0x0, 0x0, 0x0, 0x85, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
			0x0f, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'I', 'n', 't', 'e', 'g', 'e', 'r', 'L', 'i', 's', 't', 0x2, 0x0, 0x0, 0x0, 0xa, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0x9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0xa, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
			0x0a, 0x00, 0x00, 0x00, 'T', 'e', 's', 't', 'S', 't', 'r', 'i', 'n', 'g', 0x3, 0x0, 0x0, 0x0, 0x5, 0x0, 0x0, 0x0, 'V', 'a', 'l', 'u', 'e',
		});
		auto content = std::stringstream(std::string(reinterpret_cast<char*>(binaryFileContent.data()), binaryFileContent.size()));

		ankerl::nanobench::Bench().minEpochIterations(10000).run("ValueTableReader Deserialize Complex", [&]
		{
			auto actual = Soup::Core::ValueTableReader::Deserialize(content);
			ankerl::nanobench::doNotOptimizeAway(actual);
		});
	}

	{
		auto fileSystemState = FileSystemState();
		auto binaryFileContent = std::vector<char>(
		{
			'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
			'F', 'I', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
			'R', 'T', 'S', '\0', 0x00, 0x00, 0x00, 0x00,
		});
		auto content = std::stringstream(std::string(binaryFileContent.data(), binaryFileContent.size()));

		ankerl::nanobench::Bench().minEpochIterations(10000).run("OperationResultsReader Deserialize Empty", [&]
		{
			auto actual = OperationResultsReader::Deserialize(content, fileSystemState);
			ankerl::nanobench::doNotOptimizeAway(actual);
		});
	}

	{
		auto fileSystemState = FileSystemState(
			20,
			{
				{ 11, Path("C:/File1") },
				{ 12, Path("C:/File2") },
				{ 13, Path("C:/File3") },
				{ 14, Path("C:/File4") },
				{ 15, Path("C:/File5") },
				{ 16, Path("C:/File6") },
				{ 17, Path("C:/File7") },
				{ 18, Path("C:/File8") },
			});
		auto binaryFileContent = std::vector<uint8_t>(
		{
			'B', 'O', 'R', '\0', 0x02, 0x00, 0x00, 0x00,
			'F', 'I', 'S', '\0', 0x08, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '1',
			0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '2',
			0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '3',
			0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '4',
			0x05, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '5',
			0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '6',
			0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '7',
			0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 'C', ':', '/', 'F', 'i', 'l', 'e', '8',
			'R', 'T', 'S', '\0', 0x02, 0x00, 0x00, 0x00,
			0x05, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x00,
			0x10, 0x16, 0x62, 0xbb, 0x0b, 0x41, 0x38, 0x00,
			0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
			0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
			0x06, 0x00, 0x00, 0x00,
			0x01, 0x00, 0x00, 0x00,
			0x80, 0x8d, 0xa9, 0xeb, 0x0b, 0x41, 0x38, 0x00,
			0x02, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
			0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
		});
		auto content = std::stringstream(std::string((char*)binaryFileContent.data(), binaryFileContent.size()));

		ankerl::nanobench::Bench().minEpochIterations(10000).run("OperationResultsReader Deserialize Complex", [&]
		{
			auto actual = OperationResultsReader::Deserialize(content, fileSystemState);
			ankerl::nanobench::doNotOptimizeAway(actual);
		});
	}

	{
		auto recipeFile = Path("./Recipe.sml");
		auto recipe = std::stringstream(
			R"(
				Name: 'MyPackage'
				Language: (C++@1)
			)");
		ankerl::nanobench::Bench().minEpochIterations(10000).run("RecipeSML Deserialize Simple", [&]
		{
			auto actual = Recipe(RecipeSML::Deserialize(recipeFile, recipe));
			ankerl::nanobench::doNotOptimizeAway(actual);
		});
	}

	{
		auto recipeFile = Path("./Recipe.sml");
		auto recipe = std::stringstream(
			R"(
				Name: 'MyPackage'
				Language: (C++@1)
				Version: 1.2.3
				Source: [
					'File1.cpp'
					'File2.cpp'
					'File3.cpp'
					'File4.cpp'
					'File5.cpp'
					'File6.cpp'
					'File7.cpp'
					'File8.cpp'
					'File9.cpp'
					'File10.cpp'
				]
				Dependencies: {
					Runtime: [
						'./Package1/'
					]
					Build: []
					Test: []
				}
			)");
		ankerl::nanobench::Bench().minEpochIterations(10000).run("RecipeSML Deserialize Complex", [&]
		{
			auto actual = Recipe(RecipeSML::Deserialize(recipeFile, recipe));
			ankerl::nanobench::doNotOptimizeAway(actual);
		});
	}

	{
		// Register the test listener
		auto testListener = std::make_shared<TestTraceListener>();
		auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

		// Register the test system
		auto system = std::make_shared<MockSystem>();
		auto scopedSystem = ScopedSystemRegister(system);

		// Register the test file system
		auto fileSystem = std::make_shared<MockFileSystem>();
		auto scopedFileSystem = ScopedFileSystemRegister(fileSystem);

		fileSystem->CreateMockFile(
			Path("C:/testlocation/Soup.Generate.exe"),
			std::make_shared<MockFile>());

		fileSystem->CreateMockDirectory(
			Path("C:/WorkingDirectory/MyPackage/"),
			std::make_shared<MockDirectory>(std::vector<Path>({
				Path("./Recipe.sml"),
			})));

		fileSystem->CreateMockDirectory(
			Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/"),
			std::make_shared<MockDirectory>(std::vector<Path>({})));

		fileSystem->CreateMockDirectory(
			Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/"),
			std::make_shared<MockDirectory>(std::vector<Path>({})));

		fileSystem->CreateMockDirectory(
			Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/temp/"),
			std::make_shared<MockDirectory>(std::vector<Path>({})));

		fileSystem->CreateMockDirectory(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/"),
			std::make_shared<MockDirectory>(std::vector<Path>({
				Path("./Recipe.sml"),
			})));

		fileSystem->CreateMockDirectory(
			Path("C:/BuiltIn/Packages/Soup/Wren/0.4.1/"),
			std::make_shared<MockDirectory>(std::vector<Path>({
				Path("./Recipe.sml"),
			})));

		fileSystem->CreateMockDirectory(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/"),
			std::make_shared<MockDirectory>(std::vector<Path>({})));

		fileSystem->CreateMockDirectory(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/"),
			std::make_shared<MockDirectory>(std::vector<Path>({})));

		fileSystem->CreateMockDirectory(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/temp/"),
			std::make_shared<MockDirectory>(std::vector<Path>({})));

		// Create the Recipe to build
		fileSystem->CreateMockFile(
			Path("C:/WorkingDirectory/MyPackage/Recipe.sml"),
			std::make_shared<MockFile>(std::stringstream(R"(
				Name: 'MyPackage'
				Language: (C++@0.8)
			)")));

		fileSystem->CreateMockFile(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/Recipe.sml"),
			std::make_shared<MockFile>(std::stringstream(R"(
				Name: 'Cpp'
				Language: (Wren@1)
			)")));

		fileSystem->CreateMockFile(
			Path("C:/BuiltIn/Packages/Soup/Wren/0.4.1/Recipe.sml"),
			std::make_shared<MockFile>(std::stringstream(R"(
				Name: 'Wren'
				Language: (Wren@1)
			)")));

		// Create the package lock
		fileSystem->CreateMockFile(
			Path("C:/WorkingDirectory/MyPackage/PackageLock.sml"),
			std::make_shared<MockFile>(std::stringstream(R"(
				Version: 5
				Closures: {
					Root: {
						'C++': {
							MyPackage: { Version: '../MyPackage/', Build: 'Build0', Tool: 'Tool0' }
						}
					}
					Build0: {
						Wren: {
							'Soup|Cpp': { Version: 0.8.2 }
						}
					}
					Tool0: {}
				}
			)")));

		fileSystem->CreateMockFile(
			Path("C:/Users/Me/.soup/locks/Wren/Soup/Cpp/0.8.2/PackageLock.sml"),
			std::make_shared<MockFile>(std::stringstream(R"(
				Version: 5
				Closures: {
					Root: {
						Wren: {
							'Soup|Cpp': { Version: './', Build: 'Build0', Tool: 'Tool0' }
						}
					}
					Build0: {
						Wren: {
							'Soup|Wren': { Version: 0.4.3 }
						}
					}
					Tool0: {}
				}
			)")));

		auto fileSystemState = FileSystemState();

		auto myPackageOperationGraph = OperationGraph(
			std::vector<OperationId>(),
			std::vector<OperationInfo>());
		auto myPackageFiles = std::set<FileId>();
		auto myPackageOperationGraphContent = std::stringstream();
		OperationGraphWriter::Serialize(myPackageOperationGraph, myPackageFiles, fileSystemState, myPackageOperationGraphContent);
		fileSystem->CreateMockFile(
			Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bog"),
			std::make_shared<MockFile>(std::move(myPackageOperationGraphContent)));

		auto soupCppOperationGraph = OperationGraph(
			std::vector<OperationId>(),
			std::vector<OperationInfo>());
		auto soupCppFiles = std::set<FileId>();
		auto soupCppOperationGraphContent = std::stringstream();
		OperationGraphWriter::Serialize(soupCppOperationGraph, soupCppFiles, fileSystemState, soupCppOperationGraphContent);
		fileSystem->CreateMockFile(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bog"),
			std::make_shared<MockFile>(std::move(soupCppOperationGraphContent)));

		auto soupCppGenerateInput = ValueTable({
			{
				"Dependencies",
				ValueTable(
				{
					{
						"Build",
						ValueTable(
						{
							{
								"Soup|Wren",
								ValueTable(
								{
									{ "SoupTargetDirectory", std::string("C:/BuiltIn/Packages/Soup/Wren/0.4.1/out/.soup/") },
								})
							},
						})
					},
				})
			},
			{
				"EvaluateMacros",
				ValueTable(
				{
					{ "/(PACKAGE_Soup|Cpp)/", std::string("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/") },
					{ "/(TARGET_Soup|Cpp)/", std::string("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/") },
				})
			},
			{
				"EvaluateReadAccess",
				ValueList(
				{
					std::string("/(PACKAGE_Soup|Cpp)/"),
					std::string("/(TARGET_Soup|Cpp)/"),
				})
			},
			{
				"EvaluateWriteAccess",
				ValueList(
				{
					std::string("/(TARGET_Soup|Cpp)/"),
				})
			},
			{
				"GenerateMacros",
				ValueTable(
				{
					{ "/(BUILD_TARGET_Soup|Wren)/", std::string("C:/BuiltIn/Packages/Soup/Wren/0.4.1/out/") },
				})
			},
			{
				"GenerateSubGraphMacros",
				ValueTable(
				{
					{ "/(TARGET_Soup|Wren)/", std::string("/(BUILD_TARGET_Soup|Wren)/") },
				})
			},
			{
				"GlobalState",
				ValueTable(
				{
					{
						"Context",
						ValueTable(
						{
							{ "HostPlatform", std::string("TestPlatform") },
							{ "PackageDirectory", std::string("/(PACKAGE_Soup|Cpp)/") },
							{ "TargetDirectory", std::string("/(TARGET_Soup|Cpp)/") },
						})
					},
					{
						"Dependencies",
						ValueTable(
						{
							{
								"Build",
								ValueTable(
								{
									{
										"Soup|Wren",
										ValueTable(
										{
											{
												"Context",
												ValueTable(
												{
													{ "Reference", std::string("[Wren]Soup|Wren@0.4.1") },
													{ "TargetDirectory", std::string("/(TARGET_Soup|Wren)/") },
												})
											},
										})
									},
								})
							},
						})
					},
					{
						"FileSystem",
						ValueList({
							std::string("Recipe.sml"),
						})
					},
					{
						"Parameters",
						ValueTable(
						{
							{ "System", std::string("Windows") },
						})
					},
				})
			},
			{
				"PackageRoot",
				std::string("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/")
			},
			{
				"UserDataPath",
				std::string("C:/Users/Me/.soup/")
			},
		});
		auto soupCppGenerateInputContent = std::stringstream();
		ValueTableWriter::Serialize(soupCppGenerateInput, soupCppGenerateInputContent);
		fileSystem->CreateMockFile(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/GenerateInput.bvt"),
			std::make_shared<MockFile>(std::move(soupCppGenerateInputContent)));

		auto myPackageGenerateInput = ValueTable({
			{
				"Dependencies",
				ValueTable(
				{
					{
						"Build",
						ValueTable(
						{
							{
								"Soup|Cpp",
								ValueTable(
								{
									{ "SoupTargetDirectory", std::string("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/") },
								})
							},
						})
					},
				})
			},
			{
				"EvaluateMacros",
				ValueTable(
				{
					{ "/(PACKAGE_MyPackage)/", std::string("C:/WorkingDirectory/MyPackage/") },
					{ "/(TARGET_MyPackage)/", std::string("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/") },
				})
			},
			{
				"EvaluateReadAccess",
				ValueList(
				{
					std::string("/(PACKAGE_MyPackage)/"),
					std::string("/(TARGET_MyPackage)/"),
				})
			},
			{
				"EvaluateWriteAccess",
				ValueList(
				{
					std::string("/(TARGET_MyPackage)/"),
				})
			},
			{
				"GenerateMacros",
				ValueTable(
				{
					{ "/(BUILD_TARGET_Soup|Cpp)/", std::string("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/") },
				})
			},
			{
				"GenerateSubGraphMacros",
				ValueTable(
				{
					{ "/(TARGET_Soup|Cpp)/", std::string("/(BUILD_TARGET_Soup|Cpp)/") },
				})
			},
			{
				"GlobalState",
				ValueTable(
				{
					{
						"Context",
						ValueTable(
						{
							{ "HostPlatform", std::string("TestPlatform") },
							{ "PackageDirectory", std::string("/(PACKAGE_MyPackage)/") },
							{ "TargetDirectory", std::string("/(TARGET_MyPackage)/") },
						})
					},
					{
						"Dependencies",
						ValueTable(
						{
							{
								"Build",
								ValueTable(
								{
									{
										"Soup|Cpp",
										ValueTable(
										{
											{
												"Context",
												ValueTable(
												{
													{ "Reference", std::string("[Wren]Soup|Cpp@0.8.2") },
													{ "TargetDirectory", std::string("/(TARGET_Soup|Cpp)/") },
												})
											},
										})
									},
								})
							},
						})
					},
					{
						"FileSystem",
						ValueList({
							std::string("Recipe.sml"),
						})
					},
					{ "Parameters", ValueTable() },
				})
			},
			{
				"PackageRoot",
				std::string("C:/WorkingDirectory/MyPackage/")
			},
			{
				"UserDataPath",
				std::string("C:/Users/Me/.soup/")
			},
		});
		auto myPackageGenerateInputContent = std::stringstream();
		ValueTableWriter::Serialize(myPackageGenerateInput, myPackageGenerateInputContent);
		fileSystem->CreateMockFile(
			Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/GenerateInput.bvt"),
			std::make_shared<MockFile>(std::move(myPackageGenerateInputContent)));

		auto myPackageGenerateResults = OperationResults({
			{
				1,
				OperationResult(
					true,
					std::chrono::clock_cast<std::chrono::file_clock>(
						std::chrono::time_point<std::chrono::system_clock>()),
					{},
					{})
			},
		});
		auto myPackageGenerateResultsContent = std::stringstream();
		auto myPackageGenerateResultsFiles = std::set<FileId>();
		OperationResultsWriter::Serialize(myPackageGenerateResults, myPackageGenerateResultsFiles, fileSystemState,  myPackageGenerateResultsContent);
		fileSystem->CreateMockFile(
			Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Generate.bor"),
			std::make_shared<MockFile>(std::move(myPackageGenerateResultsContent)));

		auto soupCppGenerateResults = OperationResults({
			{
				1,
				OperationResult(
					true,
					std::chrono::clock_cast<std::chrono::file_clock>(
						std::chrono::time_point<std::chrono::system_clock>()),
					{},
					{})
			},
		});
		auto soupCppGenerateResultsContent = std::stringstream();
		auto soupCppGenerateResultsFiles = std::set<FileId>();
		OperationResultsWriter::Serialize(soupCppGenerateResults, soupCppGenerateResultsFiles, fileSystemState, soupCppGenerateResultsContent);
		fileSystem->CreateMockFile(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Generate.bor"),
			std::make_shared<MockFile>(std::move(soupCppGenerateResultsContent)));

		auto soupCppEvaluateResults = OperationResults();
		auto soupCppEvaluateResultsContent = std::stringstream();
		auto soupCppEvaluateResultsFiles = std::set<FileId>();
		OperationResultsWriter::Serialize(soupCppEvaluateResults, soupCppEvaluateResultsFiles, fileSystemState, soupCppEvaluateResultsContent);
		fileSystem->CreateMockFile(
			Path("C:/Users/Me/.soup/packages/Wren/Soup/Cpp/0.8.2/out/tsWW3RZ_9Jb7Xbk2kTzx3n6uQUM/.soup/Evaluate.bor"),
			std::make_shared<MockFile>(std::move(soupCppEvaluateResultsContent)));

		auto myPackageEvaluateResults = OperationResults();
		auto myPackageEvaluateResultsContent = std::stringstream();
		auto myPackageEvaluateResultsFiles = std::set<FileId>();
		OperationResultsWriter::Serialize(myPackageEvaluateResults, myPackageEvaluateResultsFiles, fileSystemState, myPackageEvaluateResultsContent);
		fileSystem->CreateMockFile(
			Path("C:/WorkingDirectory/MyPackage/out/J_HqSstV55vlb-x6RWC_hLRFRDU/.soup/Evaluate.bor"),
			std::make_shared<MockFile>(std::move(myPackageEvaluateResultsContent)));

		// Register the test process manager
		auto processManager = std::make_shared<MockProcessManager>();
		auto scopedProcessManager = ScopedProcessManagerRegister(processManager);

		// Register the test process manager
		auto monitorProcessManager = std::make_shared<Monitor::MockMonitorProcessManager>();
		auto scopedMonitorProcessManager = Monitor::ScopedMonitorProcessManagerRegister(monitorProcessManager);

		ankerl::nanobench::Bench().minEpochIterations(3000).run("BuildEngine Execute NoDependencies UpToDate", [&]
		{
			auto builtInPackageDirectory = Path("C:/BuiltIn/Packages/");
			auto arguments = RecipeBuildArguments();
			arguments.HostPlatform = "TestPlatform";
			arguments.WorkingDirectory = Path("C:/WorkingDirectory/MyPackage/");

			// Load user config state
			auto userDataPath = BuildEngine::GetSoupUserDataPath();
			auto recipeCache = RecipeCache();

			auto packageProvider = BuildEngine::LoadBuildGraph(
				builtInPackageDirectory,
				arguments.WorkingDirectory,
				arguments.GlobalParameters,
				userDataPath,
				recipeCache);

			BuildEngine::Execute(
				packageProvider,
				std::move(arguments),
				userDataPath,
				recipeCache);
		});
	}
}