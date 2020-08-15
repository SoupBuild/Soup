// <copyright file="ArgumentParser.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildOptions.h"
#include "InitializeOptions.h"
#include "InstallOptions.h"
#include "PackOptions.h"
#include "PublishOptions.h"
#include "RunOptions.h"
#include "VersionOptions.h"
#include "ViewOptions.h"

namespace Soup::Client
{
	/// <summary>
	/// Arguments parser custom made for Soup,
	/// TODO: Create awesome generated version
	/// </summary>
	class ArgumentsParser
	{
	public:
		static ArgumentsParser Parse(const std::vector<std::string>& args)
		{
			Log::Diag("ArgumentsParser::Parse");
			if (args.size() <= 1)
			{
				throw std::runtime_error("No arguments provided.");
			}

			// The first argument must be the requested command
			auto& commandType = args[1];

			// Copy the set of unused args to ensure we consume everythings
			std::vector<std::string> unusedArgs;
			std::copy(args.begin()+2, args.end(), std::back_inserter(unusedArgs));

			// Handle the individual commands and their expected arguments
			std::unique_ptr<SharedOptions> result = nullptr;
			if (commandType == "build")
			{
				Log::Diag("Parse build");

				auto options = std::make_unique<BuildOptions>();

				// Check if the optional index arguments exist
				auto argument = std::string();
				if (TryGetIndexArgument(unusedArgs, argument))
				{
					options->Path = std::move(argument);
				}

				options->Verbosity = CheckVerbosity(unusedArgs);

				options->SkipRun = IsFlagSet("skipRun", unusedArgs);
				options->Force = IsFlagSet("force", unusedArgs);

				auto flavorValue = std::string();
				if (TryGetValueArgument("flavor", unusedArgs, flavorValue))
				{
					options->Flavor = std::move(flavorValue);
				}

				auto systemValue = std::string();
				if (TryGetValueArgument("system", unusedArgs, systemValue))
				{
					options->System = std::move(systemValue);
				}

				auto architectureValue = std::string();
				if (TryGetValueArgument("architecture", unusedArgs, architectureValue))
				{
					options->Architecture = std::move(architectureValue);
				}

				result = std::move(options);
			}
			else if (commandType == "initialize")
			{
				Log::Diag("Parse initialize");

				auto options = std::make_unique<InitializeOptions>();
				options->Verbosity = CheckVerbosity(unusedArgs);

				result = std::move(options);
			}
			else if (commandType == "install")
			{
				Log::Diag("Parse install");

				auto options = std::make_unique<InstallOptions>();

				// Check for required index argument
				auto argument = std::string();
				if (TryGetIndexArgument(unusedArgs, argument))
				{
					options->Path = std::move(argument);
				}

				auto PackageReference = std::string();
				if (TryGetValueArgument("p", unusedArgs, PackageReference))
				{
					options->PackageReference = std::move(PackageReference);
				}

				options->Verbosity = CheckVerbosity(unusedArgs);

				result = std::move(options);
			}
			else if (commandType == "publish")
			{
				Log::Diag("Parse publish");

				auto options = std::make_unique<PublishOptions>();

				// Check for required index argument
				auto argument = std::string();
				if (TryGetIndexArgument(unusedArgs, argument))
				{
					options->Path = std::move(argument);
				}
				
				options->Verbosity = CheckVerbosity(unusedArgs);

				result = std::move(options);
			}
			else if (commandType == "run")
			{
				Log::Diag("Parse run");

				auto options = std::make_unique<RunOptions>();

				// Set defaults
				auto syntheticParams = std::vector<std::string>();
				options->Verbosity = CheckVerbosity(syntheticParams);

				// Split all parameters after the args flag to pass into the run command
				std::vector<std::string> runArgs;
				SplitArguments("args", unusedArgs, runArgs);

				// Check for required index argument
				auto argument = std::string();
				if (TryGetIndexArgument(unusedArgs, argument))
				{
					options->Path = std::move(argument);
				}

				// All remaining arguments are passed to the executable
				options->Arguments = std::move(runArgs);

				result = std::move(options);
			}
			else if (commandType == "version")
			{
				Log::Diag("Parse version");

				auto options = std::make_unique<VersionOptions>();
				options->Verbosity = CheckVerbosity(unusedArgs);

				result = std::move(options);
			}
			else
			{
				throw std::runtime_error("Unknown command argument: " + commandType);
			}

			if (!unusedArgs.empty())
			{
				std::stringstream message;
				message << "Unrecognized argument(s): ";
				for (int i = 0; i < unusedArgs.size(); i++)
				{
					if (i > 0)
					{
						message << ", ";
					}

					message << unusedArgs[i];
				}

				throw std::runtime_error(message.str());
			}

			return ArgumentsParser(
				std::move(result));
		}

	private:
		ArgumentsParser(std::unique_ptr<SharedOptions>&& result) :
			_result(std::move(result))
		{
		}

	public:
		ArgumentsParser() :
			_result(nullptr)
		{
		}

		template<typename T>
		bool IsA()
		{
			return typeid(*_result) == typeid(T);
		}

		template<typename T>
		auto ExtractResult() -> T
		{
			return *dynamic_cast<T*>(_result.get());
		}

	private:
		static bool TryGetIndexArgument(std::vector<std::string>& unusedArgs, std::string& argument)
		{
			// Check if the first argument is not a dash flag
			if (!unusedArgs.empty() && !unusedArgs[0].empty() && unusedArgs[0][0] != '-')
			{
				argument = std::move(unusedArgs[0]);
				unusedArgs.erase(unusedArgs.begin());
				return true;
			}
			else
			{
				return false;
			}
		}

		static bool IsFlagSet(const char* name, std::vector<std::string>& unusedArgs)
		{
			auto flagValue = std::string("-") + name;
			Log::Diag("IsFlagSet: " + flagValue);
			auto flagLocation = std::find(unusedArgs.begin(), unusedArgs.end(), flagValue);
			if (flagLocation != unusedArgs.end())
			{
				// Consume the flag value
				unusedArgs.erase(flagLocation);
				return true;
			}
			else
			{
				return false;
			}
		}

		static void SplitArguments(const char* name, std::vector<std::string>& unusedArgs, std::vector<std::string>& splitArgs)
		{
			auto flagValue = std::string("-") + name;
			Log::Diag("IsFlagSet: " + flagValue);
			auto flagLocation = std::find(unusedArgs.begin(), unusedArgs.end(), flagValue);
			if (flagLocation != unusedArgs.end())
			{
				// Consume the flag value
				auto argsStart = std::next(flagLocation);
				std::move(argsStart, unusedArgs.end(), std::back_inserter(splitArgs));
				unusedArgs.erase(flagLocation, unusedArgs.end());
			}
		}

		static bool TryGetValueArgument(
			const char* name,
			std::vector<std::string>& unusedArgs,
			std::string& value)
		{
			auto nameValue = std::string("-") + name;
			Log::Diag("TryGetValueArgument: " + nameValue);
			auto nameLocation = std::find(unusedArgs.begin(), unusedArgs.end(), nameValue);
			if (nameLocation != unusedArgs.end())
			{
				// Consume the flag value
				auto valueLocation = unusedArgs.erase(nameLocation);
				value = std::move(*valueLocation);
				unusedArgs.erase(valueLocation);
				return true;
			}
			else
			{
				return false;
			}
		}

		static TraceEventFlag CheckVerbosity(std::vector<std::string>& unusedArgs)
		{
			auto level = 
				static_cast<uint32_t>(TraceEventFlag::HighPriority) |
				static_cast<uint32_t>(TraceEventFlag::Warning) |
				static_cast<uint32_t>(TraceEventFlag::Error) |
				static_cast<uint32_t>(TraceEventFlag::Critical);
			if (IsFlagSet("v:d", unusedArgs))
			{
				level |=
					static_cast<uint32_t>(TraceEventFlag::Information) |
					static_cast<uint32_t>(TraceEventFlag::Diagnostic);
			}
			else if (IsFlagSet("v:n", unusedArgs))
			{
				level |= static_cast<uint32_t>(TraceEventFlag::Information);
			}
			else if (IsFlagSet("v:q", unusedArgs))
			{
				// Nothing else to add
			}
			else
			{
				// Default to quiet level
			}
			
			return static_cast<TraceEventFlag>(level);
		}

	private:
		std::unique_ptr<SharedOptions> _result;
	};
}