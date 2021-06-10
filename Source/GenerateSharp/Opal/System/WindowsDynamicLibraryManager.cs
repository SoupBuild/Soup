// <copyright file="WindowsDynamicLibraryManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ILibraryManager.h"
#include "WindowsDynamicLibrary.h"

namespace Opal::System
{
	/// <summary>
	/// A platform specific dynamic library manager
	/// </summary>
	export class WindowsDynamicLibraryManager : public ILibraryManager
	{
	public:
		std::shared_ptr<ILibrary> LoadDynamicLibrary(const Path& library) override final
		{
			// Get a handle to the DLL module.
			auto libraryHandle = ::LoadLibrary(TEXT(library.ToString().c_str()));

			// If the handle is valid, try to get the function address.
			if (libraryHandle == nullptr)
			{
				auto lastError = GetLastError();
				throw "ERROR Failed to get library handle" + std::to_string(lastError);
			}

			return std::make_shared<WindowsDynamicLibrary>(libraryHandle);
		}
	};
}
