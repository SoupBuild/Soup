// <copyright file="RecipeBuildGenerator.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

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
        RecipeBuildGenerator(std::shared_ptr<ICompiler> compiler) :
            _compiler(std::move(compiler))
        {
            if (_compiler == nullptr)
                throw std::runtime_error("Argument null: compiler");
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        void Execute(const Path& workingDirectory, const Recipe& recipe)
        {
            auto generateBuildPath = 
                workingDirectory +
                Path(Constants::ProjectGenerateFolderName) +
                Path(Constants::ProjectGenerateBuildFolderName);

            // Ensure the build directories exists
            if (!IFileSystem::Current().Exists(generateBuildPath))
            {
                Log::Verbose("Create Directory: " + generateBuildPath.ToString());
                IFileSystem::Current().CreateDirectory(generateBuildPath);
            }

            // Generate the build files
            auto buildFile = Path(Constants::GenerateBuildFileName);
            CreateBuildFile(generateBuildPath, buildFile);

            // Compile the build executable
            auto sourceFiles = std::vector<Path>({
                buildFile,
            });
            CompileBuildExecutable(generateBuildPath, sourceFiles);
        }

    private:
        static void CreateBuildFile(const Path& targetDirectory, const Path& targetFile)
        {
            Log::Verbose("Generate Build File: " + targetFile.ToString());

            // Build up the core build translation unit and write it to disk
            auto buildTranslationUnit = BuildTranslationUnit();
            auto buildFile = IFileSystem::Current().OpenWrite(targetDirectory + targetFile);
            buildTranslationUnit->Write(*buildFile);
        }

        static std::shared_ptr<const SyntaxTree> BuildTranslationUnit()
        {
            // Build up the core translation unit
            std::vector<std::shared_ptr<const Declaration>> declarations = {};

            // Add the main method
            declarations.push_back(BuildMainMethod());

            auto translationUnit = SyntaxFactory::CreateTranslationUnit(
                SyntaxFactory::CreateSyntaxList<Declaration>(std::move(declarations)),
                SyntaxFactory::CreateKeywordToken(SyntaxTokenType::EndOfFile));

            return std::make_shared<const SyntaxTree>(
                std::move(translationUnit));
        }

        static std::shared_ptr<const Declaration> BuildMainMethod()
        {
            std::vector<std::shared_ptr<const Statement>> statements = {};

            // #include <vector>
            // int main()
            auto runnerFunction =  SyntaxFactory::CreateFunctionDefinition(
                SyntaxFactory::CreateDeclarationSpecifierSequence(
                    SyntaxFactory::CreatePrimitiveDataTypeSpecifier(
                        PrimitiveDataType::Int,
                        SyntaxFactory::CreateKeywordToken(
                            SyntaxTokenType::Int,
                            {
                                SyntaxFactory::CreateTrivia("\n"),
                                SyntaxFactory::CreateTrivia("#include <vector>\n"),
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
                            {}),
                        SyntaxFactory::CreateSyntaxList<Statement>(
                            std::move(statements)),
                        SyntaxFactory::CreateKeywordToken(
                            SyntaxTokenType::CloseBrace,
                            {
                                SyntaxFactory::CreateTrivia("\n"),
                            },
                            {}))));

            return runnerFunction;
        }

        /// <summary>
        /// Compile the external build executable
        /// </summary>
        void CompileBuildExecutable(const Path& targetDirectory, const std::vector<Path>& sourceFiles)
        {
            Log::Verbose("Compiling Build Executable");

            try
            {
                // Determine the include paths
                std::unordered_set<std::string> includePaths;

                // Add all dependency packages modules references
                auto includeModules = std::vector<Path>();

                // Add the dependency static library closure to link if targeting an executable
                std::vector<Path> linkLibraries;

                // Build up arguments to build this individual recipe
                auto arguments = BuildArguments();
                arguments.TargetName = "SoupBuild";
                arguments.WorkingDirectory = targetDirectory;
                arguments.ObjectDirectory = Path("out/obj/Clang");
                arguments.BinaryDirectory = Path("out/bin/Clang");
                arguments.ModuleInterfaceSourceFile = Path();
                arguments.SourceFiles = sourceFiles;
                arguments.IncludeModules = std::move(includeModules);
                arguments.LinkLibraries = std::move(linkLibraries);
                arguments.IsIncremental = true;
                arguments.PreprocessorDefinitions = std::vector<std::string>({});
                arguments.TargetType = BuildTargetType::Executable;

                // Perform the build
                auto buildEngine = BuildEngine(_compiler);
                auto wasBuilt = buildEngine.Execute(arguments);
            }
            catch (std::exception& ex)
            {
                // Log the exception and convert to handled
                Log::Error(std::string("Build Failed: ") + ex.what());
                throw HandledException();
            }
        }

    private:
        std::shared_ptr<ICompiler> _compiler;
    };
}
