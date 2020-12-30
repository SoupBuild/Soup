// <copyright file="Program.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Generate
{
	/// <summary>
	/// The root of all evil
	/// </summary>
	class Program
	{
	public:
		Program() :
			_filter(nullptr)
		{
		}

		/// <summary>
		/// The main entry point of the program
		/// </summary>
		int Run(std::vector<std::string> args)
		{
			try
			{
				// Setup the filter
				auto defaultTypes =
					static_cast<uint32_t>(TraceEventFlag::Diagnostic) |
					static_cast<uint32_t>(TraceEventFlag::Information) |
					static_cast<uint32_t>(TraceEventFlag::Warning) |
					static_cast<uint32_t>(TraceEventFlag::Error) |
					static_cast<uint32_t>(TraceEventFlag::Critical);
				_filter = std::make_shared<EventTypeFilter>(
						static_cast<TraceEventFlag>(defaultTypes));

				// Setup the console listener
				Log::RegisterListener(
					std::make_shared<ConsoleTraceListener>(
						"Log",
						_filter,
						false,
						false));

				if (args.size() != 2)
				{
					Log::Error("Invalid parameters. Expected two parameters.");
					return -1;
				}

				auto soupTargetDirectory = Path(args[1]);

				// Setup the real services
				System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());
				System::ILibraryManager::Register(std::make_shared<System::WindowsDynamicLibraryManager>());

				Log::Diag("ProgramStart");
				auto buildGenerateEngine = Runtime::BuildGenerateEngine();
				buildGenerateEngine.Generate(soupTargetDirectory);

				return 0;
			}
			catch (const std::exception& ex)
			{
				Log::Error("Exception Caught: Exiting");
				Log::Error(ex.what());
				return -2;
			}
			catch (...)
			{
				Log::Error("Unknown exception encountered");
				return -3;
			}
		}

	private:
		std::shared_ptr<EventTypeFilter> _filter;
	};
}
