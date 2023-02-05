// <copyright file="MacroManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "FileSystemState.h"

namespace Soup::Core
{
	/// <summary>
	/// The macro manager handles all things macro... It just replaces stuff.
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class MacroManager
	{
	private:
		FileSystemState& _fileSystemState;
		const std::map<std::string, std::string>& _macros;

	public:
		MacroManager(
			FileSystemState& fileSystemState,
			const std::map<std::string, std::string>& macros) :
			_fileSystemState(fileSystemState),
			_macros(macros)
		{
		}

		void ResolveMacros(std::vector<FileId>& value)
		{
			for(size_t i = 0; i < value.size(); i++)
			{
				Path file = _fileSystemState.GetFilePath(value[i]);
				auto resolvedFile = ResolveMacros(file);
				value[i] = _fileSystemState.ToFileId(resolvedFile);
			}
		}

		Path ResolveMacros(Path value)
		{
			// TODO: Is there a way to not process the path again?
			auto rawValue = std::move(value.ToString());
			return Path(ResolveMacros(std::move(rawValue)));
		}

		std::string ResolveMacros(std::string value)
		{
			for (auto& [macro, macroValue] : _macros)
			{
				for(size_t i = 0; ; i += macroValue.length())
				{
					// Find the next instance of the macro
					i = value.find(macro, i);

					// No more instances, early exit
					if(i == std::string::npos)
						break;

					// Erase the macro and insert the real value
					// TODO: Less than ideal, but gets the job done
					value.erase(i, macro.length());
					value.insert(i, macroValue);
				}
			}

			return value;
		}
	};
}
