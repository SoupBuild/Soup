// <copyright file="RecipeBuildGenerator.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PlatformProcessManager.h"
#include "STLFileSystem.h"

using namespace Soup::Syntax;
using namespace Soup::Syntax::InnerTree;

namespace Soup
{
	/// <summary>
	/// The recipe build generator that knows how to generate the source
	/// required to compile a specified Recipe.
	/// </summary>
	export class RecipeBuildGenerator
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuildGenerator"/> class.
		/// </summary>
		RecipeBuildGenerator(std::shared_ptr<ICompiler> systemCompiler) :
			_systemCompiler(std::move(systemCompiler))
		{
			if (_systemCompiler == nullptr)
				throw std::runtime_error("Argument null: systemCompiler");
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		Path EnsureExecutableBuilt(const Path& packageRoot, const Recipe& recipe)
		{
			Log::Info("EnsureExecutableBuilt '" + recipe.GetName() + "'");
			auto relativeGenerateBuildPath =
				Path(Constants::ProjectGenerateFolderName) +
				Path(Constants::ProjectGenerateBuildFolderName);
			auto generateBuildPath = 
				packageRoot +
				relativeGenerateBuildPath;

			// Ensure the build directories exists
			if (!IFileSystem::Current().Exists(generateBuildPath))
			{
				Log::Info("Create Directory: " + generateBuildPath.ToString());
				IFileSystem::Current().CreateDirectory(generateBuildPath);
			}

			// Generate the build files if outdated
			auto buildFile = Path(Constants::GenerateBuildFileName);
			auto relativeRootBuildFile = relativeGenerateBuildPath + buildFile;
			auto buildFileInputClosure = std::vector<Path>({
				Path(Constants::RecipeFileName),
			});

			Log::Info("Check if Generated Build source is outdated.");
			if (BuildStateChecker::IsOutdated(
				relativeRootBuildFile,
				buildFileInputClosure,
				packageRoot))
			{
				CreateBuildFile(packageRoot, generateBuildPath, buildFile);
			}
			else
			{
				Log::Info("Build file up to date: " + buildFile.ToString());
			}

			// Compile the build executable
			auto sourceFiles = std::vector<Path>({
				buildFile,
			});
			auto buildDependencies = std::vector<PackageReference>({
				// TODO: This needs to come from feed
				PackageReference(Path("D:/Repos/Soup/Source/Core/")),
				PackageReference(Path("D:/Repos/Soup/Source/Compiler/Clang/")),
			});
			auto buildExecutable = CompileBuildExecutable(
				generateBuildPath,
				sourceFiles,
				buildDependencies);

			return generateBuildPath + buildExecutable;
		}

		/// <summary>
		/// The core runtime within the generated build executable.
		/// TODO: We may want to move this into a separate class from the gen logic
		/// </summary>
		static int Run(const Path& packageRoot, std::shared_ptr<ICompiler> compiler)
		{
			auto projectId = 111;
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(projectId);

				// Setup the filter
				auto defaultTypes = 
					static_cast<uint32_t>(TraceEventFlag::HighPriority) |
					static_cast<uint32_t>(TraceEventFlag::Information) |
					static_cast<uint32_t>(TraceEventFlag::Warning) |
					static_cast<uint32_t>(TraceEventFlag::Error) |
					static_cast<uint32_t>(TraceEventFlag::Critical);
				auto filter = std::make_shared<EventTypeFilter>(
						static_cast<TraceEventFlag>(defaultTypes));

				// Setup the console listener
				Log::RegisterListener(
					std::make_shared<ConsoleTraceListener>(
						"Log",
						filter,
						false,
						false));

				Log::EnsureListener().SetShowEventId(true);

				// Setup the real servicess
				IFileSystem::Register(std::make_shared<STLFileSystem>());
				IProcessManager::Register(std::make_shared<PlatformProcessManager>());

				// Load the active recipe
				auto packageRecipePath = packageRoot + Path(Constants::RecipeFileName);
				Recipe recipe = {};
				if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, recipe))
				{
					Log::Error("Failed to load the recipe: " + packageRecipePath.ToString());
					throw std::runtime_error("RuntimeInitialize: Failed to load recipe.");
				}

				// Setup the recipe builder
				// TODO: Break out system compiler
				bool isSystemBuild = false;
				auto builder = RecipeBuilder(compiler, compiler);

				// Run the build
				auto arguments = RecipeBuildArguments();
				arguments.ForceRebuild = false;
				arguments.Configuration = "todo";
				builder.Execute(packageRoot, recipe, arguments, isSystemBuild);

				Log::Info("Build Completed.");
				return 0;
			}
			catch (std::exception& ex)
			{
				Log::Error(std::string("Build Failed: ") + ex.what());
				return -1;
			}
			catch (...)
			{
				Log::Error("Build Failed: Unknown error.");
				return -2;
			}
		}

	private:
		static void CreateBuildFile(
			const Path& packageRoot,
			const Path& targetDirectory,
			const Path& targetFile)
		{
			Log::Info("Generate Build File: " + targetFile.ToString());

			// Build up the core build translation unit and write it to disk
			auto buildTranslationUnit = BuildTranslationUnit(packageRoot);
			auto buildFile = IFileSystem::Current().OpenWrite(targetDirectory + targetFile);
			buildTranslationUnit->Write(*buildFile);
		}

		static std::shared_ptr<const SyntaxTree> BuildTranslationUnit(const Path& packageRoot)
		{
			// Build up the core translation unit
			std::vector<std::shared_ptr<const Declaration>> declarations = {};

			// Add the main method
			declarations.push_back(BuildImportModule("std.core"));
			declarations.push_back(BuildImportModule("Soup.Core"));
			declarations.push_back(BuildImportModule("Soup.Compiler.Clang"));
			declarations.push_back(BuildUsingNamespace("Soup"));
			declarations.push_back(BuildMainMethod(packageRoot));

			auto translationUnit = SyntaxFactory::CreateTranslationUnit(
				SyntaxFactory::CreateSyntaxList<Declaration>(std::move(declarations)),
				SyntaxFactory::CreateKeywordToken(SyntaxTokenType::EndOfFile));

			return std::make_shared<const SyntaxTree>(
				std::move(translationUnit));
		}

		static std::shared_ptr<const Declaration> BuildImportModule(std::string_view name)
		{
			return SyntaxFactory::CreateModuleImportDeclaration(
				SyntaxFactory::CreateKeywordToken(
					SyntaxTokenType::Import,
					{},
					{
						SyntaxFactory::CreateTrivia(" "),
					}),
				SyntaxFactory::CreateSyntaxSeparatorList<SyntaxToken>(
					{
						SyntaxFactory::CreateUniqueToken(SyntaxTokenType::Identifier, std::string(name)),
					},
					{}),
				SyntaxFactory::CreateKeywordToken(
					SyntaxTokenType::Semicolon,
					{},
					{
						SyntaxFactory::CreateTrivia("\n"),
					}));
		}

		static std::shared_ptr<const Declaration> BuildUsingNamespace(std::string_view name)
		{
			return SyntaxFactory::CreateUsingDirective(
				SyntaxFactory::CreateKeywordToken(
					SyntaxTokenType::Using,
					{},
					{
						SyntaxFactory::CreateTrivia(" "),
					}),
				SyntaxFactory::CreateKeywordToken(
					SyntaxTokenType::Namespace,
					{},
					{
						SyntaxFactory::CreateTrivia(" "),
					}),
				SyntaxFactory::CreateSyntaxSeparatorList<SyntaxToken>(
					{
						SyntaxFactory::CreateUniqueToken(SyntaxTokenType::Identifier, std::string(name)),
					},
					{}),
				SyntaxFactory::CreateKeywordToken(
					SyntaxTokenType::Semicolon,
					{},
					{
						SyntaxFactory::CreateTrivia("\n"),
					}));
		}

		static std::shared_ptr<const Declaration> BuildMainMethod(const Path& packageRoot)
		{
			auto packageRootLiteral = "\"" + packageRoot.ToString() + "\"";

			// auto packageRoot = Path("{packageRoot}");
			// auto compiler = std::make_shared<Compiler::Clang::Compiler>();
			// auto result = RecipeBuildGenerator::Run(packageRoot, compiler);
			// return result;
			auto statements = std::vector<std::shared_ptr<const Statement>>({
				BuildAutoInitializeDeclarationStatement(
					"packageRoot",
					{
						SyntaxFactory::CreateTrivia("\t"),
					},
					SyntaxFactory::CreateInvocationExpression(
						SyntaxFactory::CreateIdentifierExpression(
							SyntaxFactory::CreateSimpleIdentifier(
								SyntaxFactory::CreateUniqueToken(SyntaxTokenType::Identifier, "Path"))),
						SyntaxFactory::CreateKeywordToken(SyntaxTokenType::OpenParenthesis),
						SyntaxFactory::CreateSyntaxSeparatorList<SyntaxNode>(
						{
							SyntaxFactory::CreateLiteralExpression(
								LiteralType::String,
								SyntaxFactory::CreateUniqueToken(SyntaxTokenType::StringLiteral, packageRootLiteral)),
						},
						{}),
						SyntaxFactory::CreateKeywordToken(SyntaxTokenType::CloseParenthesis))),
				BuildAutoInitializeDeclarationStatement(
					"compiler",
					{
						SyntaxFactory::CreateTrivia("\t"),
					},
					SyntaxFactory::CreateInvocationExpression(
						SyntaxFactory::CreateIdentifierExpression(
							SyntaxFactory::CreateSimpleIdentifier(
								SyntaxFactory::CreateUniqueToken(SyntaxTokenType::Identifier, "std::make_shared<Compiler::Clang::Compiler>"))), // TODO: Super cheating
						SyntaxFactory::CreateKeywordToken(SyntaxTokenType::OpenParenthesis),
						SyntaxFactory::CreateSyntaxSeparatorList<SyntaxNode>({}, {}),
						SyntaxFactory::CreateKeywordToken(SyntaxTokenType::CloseParenthesis))),
				BuildAutoInitializeDeclarationStatement(
					"result",
					{
						SyntaxFactory::CreateTrivia("\t"),
					},
					SyntaxFactory::CreateInvocationExpression(
						SyntaxFactory::CreateIdentifierExpression(
							SyntaxFactory::CreateSimpleIdentifier(
								SyntaxFactory::CreateUniqueToken(SyntaxTokenType::Identifier, "RecipeBuildGenerator::Run"))), // TODO: Cheating
						SyntaxFactory::CreateKeywordToken(SyntaxTokenType::OpenParenthesis),
						SyntaxFactory::CreateSyntaxSeparatorList<SyntaxNode>(
							{
								SyntaxFactory::CreateIdentifierExpression(
									SyntaxFactory::CreateSimpleIdentifier(
										SyntaxFactory::CreateUniqueToken(SyntaxTokenType::Identifier, "packageRoot"))),
								SyntaxFactory::CreateIdentifierExpression(
									SyntaxFactory::CreateSimpleIdentifier(
										SyntaxFactory::CreateUniqueToken(SyntaxTokenType::Identifier, "compiler"))),
							},
							{
								SyntaxFactory::CreateKeywordToken(SyntaxTokenType::Comma),
							}),
						SyntaxFactory::CreateKeywordToken(SyntaxTokenType::CloseParenthesis))),
				SyntaxFactory::CreateReturnStatement(
					SyntaxFactory::CreateKeywordToken(
						SyntaxTokenType::Return,
						{
							SyntaxFactory::CreateTrivia("\t"),
						},
						{}),
					SyntaxFactory::CreateIdentifierExpression(
						SyntaxFactory::CreateSimpleIdentifier(
							SyntaxFactory::CreateUniqueToken(
								SyntaxTokenType::Identifier,
								"result",
								{
									SyntaxFactory::CreateTrivia(" ")
								},
								{}))),
					SyntaxFactory::CreateKeywordToken(
						SyntaxTokenType::Semicolon,
						{},
						{
							SyntaxFactory::CreateTrivia("\n")
						})),
			});

			//
			// int main()
			// {
			// 
			// }
			auto runnerFunction = SyntaxFactory::CreateFunctionDefinition(
				SyntaxFactory::CreateDeclarationSpecifierSequence(
					SyntaxFactory::CreatePrimitiveDataTypeSpecifier(
						PrimitiveDataType::Int,
						SyntaxFactory::CreateKeywordToken(
							SyntaxTokenType::Int,
							{
								SyntaxFactory::CreateTrivia("\n"),
							},
							{}))),
				SyntaxFactory::CreateIdentifierExpression(
					SyntaxFactory::CreateSimpleIdentifier(
						SyntaxFactory::CreateUniqueToken(
							SyntaxTokenType::Identifier,
							"main",
							{
								SyntaxFactory::CreateTrivia(" "),
							},
							{}))),
				SyntaxFactory::CreateParameterList(
					SyntaxFactory::CreateKeywordToken(SyntaxTokenType::OpenParenthesis),
					SyntaxFactory::CreateSyntaxSeparatorList<Parameter>({}, {}),
					SyntaxFactory::CreateKeywordToken(SyntaxTokenType::CloseParenthesis)),
				SyntaxFactory::CreateRegularFunctionBody(
					SyntaxFactory::CreateCompoundStatement(
						SyntaxFactory::CreateKeywordToken(
							SyntaxTokenType::OpenBrace,
							{
								SyntaxFactory::CreateTrivia("\n"),
							},
							{
								SyntaxFactory::CreateTrivia("\n"),
							}),
						SyntaxFactory::CreateSyntaxList<Statement>(
							std::move(statements)),
						SyntaxFactory::CreateKeywordToken(SyntaxTokenType::CloseBrace))));

			return runnerFunction;
		}

		static std::shared_ptr<const Statement> BuildAutoInitializeDeclarationStatement(
			std::string variableName,
			std::vector<SyntaxTrivia> leadingTrivia,
			std::shared_ptr<const Expression> assignExpression)
		{
			return SyntaxFactory::CreateDeclarationStatement(
				SyntaxFactory::CreateSimpleDeclaration(
					SyntaxFactory::CreateDeclarationSpecifierSequence(
						SyntaxFactory::CreatePrimitiveDataTypeSpecifier(
							PrimitiveDataType::Auto,
							SyntaxFactory::CreateKeywordToken(
								SyntaxTokenType::Auto,
								std::move(leadingTrivia),
								{}))),
					SyntaxFactory::CreateInitializerDeclaratorList(
						SyntaxFactory::CreateSyntaxSeparatorList<InitializerDeclarator>(
							{
								SyntaxFactory::CreateInitializerDeclarator(
									SyntaxFactory::CreateSimpleIdentifier(
										SyntaxFactory::CreateUniqueToken(
											SyntaxTokenType::Identifier,
											std::move(variableName),
											{
												SyntaxFactory::CreateTrivia(" "),
											},
											{})),
									SyntaxFactory::CreateValueEqualInitializer(
										SyntaxFactory::CreateKeywordToken(
											SyntaxTokenType::Equal,
											{
												SyntaxFactory::CreateTrivia(" "),
											},
											{
												SyntaxFactory::CreateTrivia(" "),
											}),
										assignExpression)),
							},
							{})),
					SyntaxFactory::CreateKeywordToken(
						SyntaxTokenType::Semicolon,
						{},
						{
							SyntaxFactory::CreateTrivia("\n"),
						})));
		}

		/// <summary>
		/// Compile the external build executable
		/// </summary>
		Path CompileBuildExecutable(
			const Path& targetDirectory,
			const std::vector<Path>& sourceFiles,
			const std::vector<PackageReference>& buildDependencies)
		{
			Log::Info("Compiling Build Executable");

			// Determine the include paths
			std::unordered_set<std::string> includePaths;

			// Add all dependency packages modules references
			auto includeModules = std::vector<Path>();
			for (auto dependecy : buildDependencies)
			{
				auto packagePath = RecipeExtensions::GetPackageReferencePath(targetDirectory, dependecy);
				auto modulePath = RecipeExtensions::GetRecipeOutputPath(
					packagePath,
					RecipeExtensions::GetBinaryDirectory(*_systemCompiler, "todo"),
					std::string(_systemCompiler->GetModuleFileExtension()));
				includeModules.push_back(std::move(modulePath));
			}

			// Add the dependency static library closure to link if targeting an executable
			std::vector<Path> linkLibraries;
			RecipeExtensions::GenerateDependecyStaticLibraryClosure(
				*_systemCompiler,
				"todo",
				targetDirectory,
				buildDependencies,
				linkLibraries);

			// Build up arguments to build this individual recipe
			auto arguments = BuildArguments();
			arguments.TargetName = "Soup.RecipeBuild";
			arguments.WorkingDirectory = targetDirectory;
			arguments.ObjectDirectory = RecipeExtensions::GetObjectDirectory(*_systemCompiler, "todo");
			arguments.BinaryDirectory = RecipeExtensions::GetBinaryDirectory(*_systemCompiler, "todo");
			arguments.ModuleInterfaceSourceFile = Path();
			arguments.SourceFiles = sourceFiles;
			arguments.IncludeModules = std::move(includeModules);
			arguments.LinkLibraries = std::move(linkLibraries);
			arguments.IsIncremental = true;
			arguments.PreprocessorDefinitions = std::vector<std::string>({});
			arguments.TargetType = BuildTargetType::Executable;

			// Perform the build
			auto buildEngine = BuildEngine(_systemCompiler);
			auto wasBuilt = buildEngine.Execute(arguments);

			return arguments.BinaryDirectory + Path(arguments.TargetName + ".exe");
		}

	private:
		std::shared_ptr<ICompiler> _systemCompiler;
	};
}
