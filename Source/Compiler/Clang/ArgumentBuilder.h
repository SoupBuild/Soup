// <copyright file="ArgumentBuilder.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Compiler::Clang
{
    /// <summary>
    /// A helper class that builds the correct set of compiler arguments for a given
    /// set of options.
    /// </summary>
    export class ArgumentBuilder
    {
    public:
        static std::vector<std::string> BuildCompilerArguments(
            const std::string& sourceFile,
            const CompileArguments& args)
        {
            // Calculate object output file
            auto commandArgs = std::vector<std::string>();

            // Enable verbose output
            // commandArgs.Add("-v");

            // Disable ms compatibility (workaround for bug with inplicit types in pcm)
            commandArgs.push_back("-fno-ms-compatibility");

            // Allow public std visible during link time
            commandArgs.push_back("-Xclang");
            commandArgs.push_back("-flto-visibility-public-std");

            // Enable Header includes if needed
            if (args.GenerateIncludeTree)
            {
                commandArgs.push_back("-H");
            }

            // Set the language standard
            switch (args.Standard)
            {
                case LanguageStandard::CPP11:
                    commandArgs.push_back("-std=c++11");
                    break;
                case LanguageStandard::CPP14:
                    commandArgs.push_back("-std=c++14");
                    break;
                case LanguageStandard::CPP17:
                    commandArgs.push_back("-std=c++17");
                    break;
                default:
                    throw std::runtime_error("Unknown language standard.");
            }

            // // Set the include paths
            // for (var directory in args.IncludeDirectories)
            // {
            //     var include = directory;
            //     if (!Path.IsPathFullyQualified(directory))
            //     {
            //         include = Path.Combine(rootPath, directory);
            //     }

            //     commandArgs.Add($"-I\"{include}\"");
            // }

            // // Set the preprocessor definitions
            // foreach (var definition in args.PreprocessorDefinitions)
            // {
            //     commandArgs.Add($"-D{definition}");
            // }

            // // Ignore Standard Include Paths to prevent pulling in accidental headers
            // // commandArgs.Add("-X");

            // // Add in the std include paths

            // // Enable c++ exceptions
            // // commandArgs.Add("-EHs");

            // // Enable experimental features
            // if (args.Standard == LanguageStandard.Latest)
            // {
            //     commandArgs.Add("-fmodules-ts");
            // }

            // // Add the module references
            // foreach (var module in args.Modules)
            // {
            //     commandArgs.Add($"-fmodule-file=\"{Path.Combine(rootPath, module)}\"");
            // }

            if (args.ExportModule)
            {
                commandArgs.push_back("--precompile");

                // There must be only one source file
                if (args.SourceFiles.size() != 1)
                {
                    throw std::runtime_error("Export module expects only one source file.");
                }

                // Place the ifc in the output directory
                //var outputFile = "{Path.GetFileNameWithoutExtension(sourceFile)}.{ModuleFileExtension}";
                //commandArgs.AddRange(new string[] { "-o", outputFile });
            }
            else
            {
                // Only run preprocessor, compile and assemble
                commandArgs.push_back("-c");
            }

            // Lastly add the file
            commandArgs.push_back(sourceFile);

            return commandArgs;
        }
    };
}
